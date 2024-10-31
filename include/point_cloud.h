// This file is part of hypervolume-t 
// Header file for the point_cloud classs

namespace hvt {

	// Dimension and point types
	typedef CGAL::Epick_d< CGAL::Dynamic_dimension_tag > kernel;
	typedef typename kernel::Point_d Point_d;	

	class point_cloud {

		private:
			// The points
			std::vector< hvt::point > points;

			// A list of neighbors for each point
			std::vector< hvt::neighbors > all_neighbors;

			// Distance between pairs of points at given indices
			// Modified from from https://github.com/Ripser
			float dist( const hvt::index i, const hvt::index j ) const {
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
				std::setprecision(12) ;

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

			void export_points( const std::string filename, bool header ) {
  				
				// Set up file for writing
  				std::ofstream outfile;
  				outfile.open(filename);

  				// Write header
  				if ( header ) {
	  				outfile << "x0";
					for ( int i = 1; i < get_dim(); i++ ) {
  						outfile << ",x" << i;
					}
					outfile << "\n";
				}

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
			}

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

			// Split an input point point cloud in two parts, based on the order of a given coordinate
			void split_at_coordinate( const int dim_index, hvt::point_cloud& first, hvt::point_cloud& second ) {

				// Get slice
       			hvt::point target;
       			get_slice(dim_index, target);

  				// Adapted from https://stackoverflow.com/a/12399290
       			const int target_size = points.size();
  				std::vector<size_t> idx(target_size);
  				iota(idx.begin(), idx.end(), 0);
  				std::stable_sort(
  					idx.begin(),
  					idx.end(),
       				[&target](size_t i1, size_t i2) {return target[i1] < target[i2];}
       			);

       			// Add to new point clouds
       			int i = 0;
       			while (i < target_size/2) {
       				first.add_point(points[i]);
       				i++;
       			}
       			while (i < target_size) {
       				second.add_point(points[i]);
       				i++;
       			}

			}

			// Add a point to the end of the list of points
			void add_point( hvt::point new_point ) {
				points.push_back(new_point);
			}

			// Set all points at once
			void add_points( std::vector< hvt::point >& new_points ) {
				for ( hvt::point p : new_points ) {
					points.push_back(p);
				}
			}

			// Dimension of points in point cloud
			int get_dim() {
				return points[0].size();
			}

			// Number of points in point cloud
			int get_size() {
				return points.size();
			}

			// Point at a given index
			// Sets input reference to requested point
			void get_point( const hvt::index index, hvt::point& my_point ) {
				my_point = points[index];
			}

			// All points
			void get_points( std::vector< hvt::point>& my_points ) {
				my_points = points;
			}

			// Point at a given index, as CGAL point
			Point_d get_point_asCGAL( const hvt::index index, hvt::point& my_point, hvt::index my_dim ) {
				my_point = points[index];
				return Point_d(my_dim, my_point.begin(), my_point.end());
			}

			// Coordinate of all points at a given dimension
			// Sets input reference to requested slice
			void get_slice( const int dim_index, hvt::point& my_point ) {

				// Check that the requested dimension index is admissible
				assert ( dim_index >= 0 );
				assert ( dim_index < points[0].size() );

				for ( hvt::point p : points ) { 
					my_point.push_back( p[dim_index] );
				}
			}

			// Neighbors of a point at a given index
			// Sets input reference to requested neighbor
			void get_neighbors( const hvt::index index, hvt::neighbors& neighbors ) {
				neighbors = all_neighbors[index];
			}

			// Distance among points, input as indices
			float get_dist( hvt::index i, hvt::index j ) {
				return dist(i,j);
			}

	};
}