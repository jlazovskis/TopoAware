// This file is part of hypervolume-t 
// Header file for the point_cloud class

namespace hvt {

	class point_cloud {

		// Dimension and point types
		typedef CGAL::Epick_d< CGAL::Dynamic_dimension_tag > kernel;
		typedef typename kernel::Point_d Point_d;	

		private:
			// The points
			std::vector< hvt::point > points;

			// A list of neighbors for each point
			std::vector< hvt::neighbors > all_neighbors;

			// Distance between pairs of points at given indices
			// Modified from from https://github.com/Ripser
			float dist(const hvt::index i, const hvt::index j) const {
				assert(i < points.size());
				assert(j < points.size());
				return std::sqrt(
					std::inner_product(
						points[i].begin(),
						points[i].end(),
						points[j].begin(),
						float(),
						std::plus<float>(),
						[](float u, float v) {
							return (u - v) * (u - v);
						}
					)
				);
		  	}

		public:
			// Constructor
			point_cloud() {};

			// Input / output
			// Modified from https://bitbucket.org/phat-code/phat
			bool load_points( const std::string filename ) {
				// Check to make sure file can be read
				std::ifstream dummy( filename.c_str() );
				if( dummy.fail() ) {
					std::cerr << "couldn't open file " << filename << std::endl;
					return false;
				}

				// Read in the data (from https://github.com/Ripser)
				this->points = points;
				std::string line;
				std::ifstream input_stream( filename.c_str() );

				hvt::value value;
				while (std::getline(input_stream, line)) {
					hvt::point point;
					std::istringstream s(line);
					while (s >> value) {
						point.push_back(value);
						s.ignore();
					}
					if (!point.empty()) points.push_back(point);
				}
				return true;
			};

			bool export_points( const std::string filename ) {
  				
				// Set up file for writing
  				std::ofstream outfile;
  				outfile.open(filename);

  				// Write headers
  				outfile << "x0";
				for ( int i = 1; i < get_dim(); i++ ) {
  					outfile << ",x" << i;
				}
				outfile << "\n";

				// Write values
				for ( const hvt::point& point : points ) { 
  					outfile << point[0];
					for ( int i = 1; i < get_dim(); i++ ) {
	  					outfile << "," << point[i];
	  				}
	  				outfile << "\n";
	  			}

	  			// Close file and exit
				outfile.close();
				return true;
			}

			// Add barycenters from another point cloud
			bool split_points( hvt::point_cloud initial_point_cloud, std::vector<int>& points_added,  const hvt::value sparsification_dist ) {
				std::vector<int> new_point_count;

				// Check that the input point cloud has points initialized
				const int size = initial_point_cloud.get_size();
				assert (size > 0);

				// Populate the points
				int counter1 = 0;
				int counter2 = 0;
				for ( hvt::index i = 0; i < size; i++ ) {
	
					// Temporary containers
					hvt::point current_point;
					hvt::neighbors current_neighbors;
					initial_point_cloud.get_point(i,current_point);
					initial_point_cloud.get_neighbors(i,current_neighbors);

					// Add same point
					hvt::point point;
					for (const hvt::value& value : current_point) { 
						point.push_back(value);
					}
					points.push_back(point);

					// Iterate through neighbors
					for ( int j = 0; j < current_neighbors.size() ; j++ ) {
						const hvt::index index_j = get_index(current_neighbors[j]);
						const float dist_ij = initial_point_cloud.dist(i,index_j);

						// Add midpoint of every two points
						std::vector< hvt::point > neighboring_points;
						hvt::point current_point_b;
						initial_point_cloud.get_point(index_j,current_point_b);
						neighboring_points.push_back(current_point_b);

						// Declare, compute, record new point
						hvt::point new_point;
						hvt::point_average( current_point, neighboring_points, new_point );
						points.push_back(new_point);
						counter1 += 1;
					
						// Iterate once again for triples
						for ( int k = j+1 ; k < current_neighbors.size() ; k++ ) {
							const hvt::index index_k = get_index(current_neighbors[k]);
							const float dist_jk = initial_point_cloud.dist(index_j,index_k);

							// Check if third point is not too close to existing points
							if (dist_ij > dist_jk/3) {
								if (initial_point_cloud.dist(i,index_k) > dist_ij/3) {
									if (initial_point_cloud.dist(index_j,index_k) > dist_ij/3) {

										// Add another neighboring point
										hvt::point current_point_c;
										initial_point_cloud.get_point(index_k,current_point_c);
										neighboring_points.push_back(current_point_c);
			
										// Declare, compute, record new point
										hvt::point new_point2;
										hvt::point_average( current_point, neighboring_points, new_point2 );
										points.push_back(new_point2);
										counter2 += 1;
			
										// Drop last element two have two starting points for next triple
										neighboring_points.pop_back();
									}
								}
							}
						}
					}
				}

				// Record progress and finish
				new_point_count.push_back(counter1);
				new_point_count.push_back(counter2);
				points_added = new_point_count;
				return true;
			};

			// Sparsify points of another point cloud 
			// Adapted from https://gudhi.inria.fr/doc/latest/example_sparsify_point_set_8cpp-example.html
			bool sparsify_points( hvt::point_cloud initial_point_cloud, const hvt::value distance ) {

				// Check that the input point cloud has points initialized
				assert (initial_point_cloud.get_size() > 0);

				// Get dimension and number of points
				const hvt::index dim = initial_point_cloud.get_dim();
				const hvt::index size = initial_point_cloud.get_size();

				// Create the point cloud as a CGAL kernel
				std::vector<Point_d> initial_point_cloud_asCGAL;
  				for (int i = 0; i < size; i++) {
					hvt::point old_point;
  					const Point_d new_point = initial_point_cloud.get_point_asCGAL(i, old_point, dim);
  					initial_point_cloud_asCGAL.push_back(new_point);
  				}	

  				// Sparsify point cloud
  				kernel new_kernel;
  				Gudhi::subsampling::sparsify_point_set(new_kernel, initial_point_cloud_asCGAL, distance, std::back_inserter(points));

  				// Success
				return true;
			};

			// Compute neighbors at a certain distance (adapted from https://github.com/Ripser)
			// Only find neighbors at higher indices, to skip duplicates later
			void find_neighbors( const hvt::value threshold ) {

				// Get container for data
				this->all_neighbors = all_neighbors;

				// Iterate
				for ( hvt::index i = 0; i < points.size(); ++i ) {

					// Make temporary neighbor vector
                    std::vector< hvt::index_diameter_t > temp_nb;
					for (hvt::index j = i+1; j < points.size(); ++j) {
						hvt::value d = dist(i, j);
						if (d <= threshold) {
							// std::cout << i << " " << j << "\n";
							temp_nb.push_back({j, d});
						}
					}

					// Register the neighbors
					all_neighbors.push_back(temp_nb);
				}
			};

			// Dimension of points in point cloud
			hvt::index get_dim() {
				return points[0].size();
			}

			// Number of points in point cloud
			hvt::index get_size() {
				return points.size();
			}

			// Point at a given index
			// Sets input reference to requested point
			void get_point( const hvt::index index, hvt::point& my_point ) {
				my_point = points[index];
			}

			// Point at a given index, as CGAL point
			Point_d get_point_asCGAL( const hvt::index index, hvt::point& my_point, hvt::index my_dim ) {
				my_point = points[index];
				return Point_d(my_dim, my_point.begin(), my_point.end());
			}

			// Neighbors of a point at a given index
			// Sets input reference to requested neighbor
			void get_neighbors( const hvt::index index, hvt::neighbors& neighbors ) {
				neighbors = all_neighbors[index];
			}

	};
}