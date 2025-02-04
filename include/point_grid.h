// This file is part of TopoAware 
// Header file for the point_grid class

namespace tpaw {

	class point_grid {
		private:
			// The points (as a flat vector with coordinates)
			std::vector< tpaw::point > points;
			std::vector< tpaw::point > points_complement;

			// True / False indicators for all possible points (as a flat binary vector)
			std::vector<bool> grid;
			std::vector<bool> grid_complement;

			// The associated point cloud
			tpaw::point_cloud point_cloud;

			// The step size
			float step_size = 0;

			// The number of steps in each dimension
			std::vector< int > steps_bydim;

		public:
			// Constructor
			point_grid() {};

			// Constructor from point cloud
			void construct_from_point_cloud( tpaw::point_cloud initial_point_cloud, float requested_step_size ) {
				// Set values
				point_cloud = initial_point_cloud;
				step_size = requested_step_size;

				// Info about initial point cloud
				const int dim = point_cloud.get_dim();
				const int size = point_cloud.get_size();
		
				// Find range of all dimensions
				std::vector< std::pair< tpaw::value, tpaw::value > > ranges;
				for ( int d=0; d<dim; d++ ) {
					std::vector< tpaw::value > slice;
					point_cloud.get_slice(d, slice);
		    		const auto [min, max] = std::minmax_element(begin(slice), end(slice));
		    		std::pair<  tpaw::value, tpaw::value > current_range;
		    		current_range.first = *min-2*step_size;
		    		current_range.second = *max+5*step_size;
		    		ranges.push_back(current_range);
				}
		
				// Get number of steps in each dimension
				std::vector< int > steps;
				for ( int d=0; d<dim; d++ ) {
					const auto dv = tpaw::fdiv(abs(ranges[d].second - ranges[d].first), step_size);
					steps.push_back(dv[0]);
				}
				steps_bydim = steps;
				int total_points = 1;
				for ( int q : steps ) {
					total_points = total_points*q;
				}

				// Create binary array for whether or not points should exist
				std::vector< bool > point_tracker(total_points, false);
				std::vector< bool > point_tracker_complement(total_points, true);
		
				// Iterate through points in initial point cloud and drop them
				for ( int i=0; i<size; i ++ ) {
					tpaw::point current_point;
					initial_point_cloud.get_point(i, current_point);
					std::vector< int > current_coordinate;
					for ( int d=0; d<dim; d++ ) {
						const auto dv = tpaw::fdiv(abs(current_point[d] - ranges[d].first), step_size);
						current_coordinate.push_back(dv[0]);
					}
					int current_location = 1;
					for ( int d=0; d<dim; d++ ) {
						int current_summand = current_coordinate[d];
						for ( int dd=0; dd<d; dd++ ) {
							current_summand = current_summand*steps_bydim[dd];
						}
						current_location += current_summand;
					}
		
					// Remove all around point
					std::vector<int> current_locations;
					current_locations.push_back(current_location);
					current_locations.push_back(current_location+1);
					int current_coeff = 1;
					for ( int d=0; d<(dim-1); d++ ) {
						std::vector<int> temp_locations;
						current_coeff = current_coeff*steps_bydim[d];
						for (int loc : current_locations) { temp_locations.push_back(loc+current_coeff); }
						for (int loc : temp_locations) { current_locations.push_back(loc); }
					}
					for ( int i : current_locations ) {
						if ( i < total_points ) {
							point_tracker[i] = (true || point_tracker[i]);
							point_tracker_complement[i] = (false && point_tracker_complement[i]);
						}
					}
				}
		
				// Record data to point grid
				grid = point_tracker;
				grid_complement = point_tracker_complement;
		
				// Initialize the points in the positions that have not been dropped
				int count = 0;
				for ( int i=0; i<total_points; i++ ) {
					tpaw::point current_point;
					int multiplier = 1;
					for ( int d=0; d<dim; d++ ) {
						const auto coeff = std::div(i%(multiplier*steps_bydim[d]), multiplier);
						current_point.push_back(ranges[d].first + coeff.quot*step_size);
						multiplier = multiplier*steps_bydim[d];
					}
					if ( grid[i] ) { points.push_back(current_point); }
					else  { points_complement.push_back(current_point); }		
				}

			};

			// Set associated point_cloud
			void set_point_cloud(tpaw::point_cloud& my_cloud) { point_cloud = my_cloud; };

			// Get associated point_cloud
			void get_point_cloud(tpaw::point_cloud& my_cloud) { my_cloud = point_cloud; };

			// Add a point to the end of the list of points
			void add_point( tpaw::point new_point ) { points.push_back(new_point); };

			// Set associated grids
			void set_point_grids(std::vector<bool>& my_grid) {
				grid = my_grid; 
				grid_complement = my_grid; 
				for ( int i=0; i<grid.size(); i++ ) { grid_complement[i] = !grid_complement[i]; }
			};

			// Get associated grid
			void get_point_grid(std::vector<bool>& my_grid) { my_grid = grid; };

			// Set step size
			void set_step_size(float my_step_size) { step_size = my_step_size; };

			// Get size
			int get_size() { return grid.size(); }
			int get_nonzero_size() { int i = 0; for ( bool b : grid ) { if ( b ) { i++; } } return i; }

			// Construct a True / False indicator for a component starting from a given (indexed) location
			void get_component( std::vector<bool>& vector_to_fill, int start_index ) {

				// Check that everything is fine
				assert(vector_to_fill.size() == grid.size());
				assert(start_index >= 0);
				assert(start_index < grid.size());

				// Mark current location as being in the component
				vector_to_fill[start_index] = true;

				// Go to every neighboring location
				const int dim = point_cloud.get_dim();
				std::vector<int> neighbors{start_index-1, start_index, start_index+1};
				int current_coeff = 1;
				for ( int d=0; d<(dim-1); d++ ) {
					std::vector<int> temp_neighbors;
					current_coeff = current_coeff*steps_bydim[d];
					for (int loc : neighbors) { temp_neighbors.push_back(loc+current_coeff); temp_neighbors.push_back(loc-current_coeff); }
					for (int loc : temp_neighbors) { neighbors.push_back(loc); }
				}
				for ( int i : neighbors ) {
					if ( (i >= 0) && (i < grid.size()) ) {
						if ( !vector_to_fill[start_index] ) {
							get_component(vector_to_fill, i);
						}
					}
				}
			};

			// Export points by calling the same function for the associated class
			void export_points( const std::string filename, bool header_bool, bool complement_bool ) {
				tpaw::point_cloud pc;
				if ( complement_bool ) { pc.add_points(points); }
				else { pc.add_points(points_complement); }
				pc.export_points(filename, header_bool); 
	
			};

	};

}