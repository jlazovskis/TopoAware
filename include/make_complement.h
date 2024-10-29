// This file is part of hypervolume-t 
// Header file for making the complement of a point cloud

namespace hvt {

	void make_complement(
			hvt::point_cloud initial_point_cloud,
			hvt::point_cloud& target_point_cloud,
			hvt::value step_size,
			std::vector<int>& points_added) {
		std::vector<int> new_point_count;

		// Info about initial point cloud
		const int dim = initial_point_cloud.get_dim();
		const int size = initial_point_cloud.get_size();

		// Find range of all dimensions
		std::vector< std::pair< hvt::value, hvt::value > > ranges;
		for ( int d=0; d<=dim; d++ ) {
			std::vector< hvt::value > slice;
			initial_point_cloud.get_slice(d, slice);
		    const auto [min, max] = std::minmax_element(begin(slice), end(slice));
		    ranges.push_back(std::make_pair(min,max));
		}

		// Get number of steps in each dimension
		std::vector< int > steps_bydim;
		for ( int d=0; d<=dim; d++ ) {
			const auto dv = std::div(abs(ranges[d].second - ranges[d].first), step_size);
			steps_bydim.push_back(dv.quot);
		}
		int total_points = 1;
		for ( int q : steps_bydim ) {
			total_points = total_points*q;
		}

		// Create binary array for whether or not points should exist
		std::vector< std::vector< bool >(dim, false) >(total_points) point_tracker;

		// Iterate through points in initial point cloud and drop them
		for ( int i=0; i<size; i ++ ) {
			hvt::point current_point;
			initial_point_cloud.get_point(i, current_point);
			std::vector< int > current_coordinate;
			for ( int d=0; d<=dim; d++ ) {
				const auto dv = std::div(abs(current_point[d] - ranges[d].first), step_size);
				current_coordinate.push_back(dv.quot);
			}
			int current_location = 1;
			for ( int d=0; d<=dim; d++ ) {
				int current_summand = current_coordinate[d];
				for ( int dd=0; dd<d; dd++ ) {
					current_summand = current_summand*steps_bydim[dd];
				}
				current_location += current_summand;
			}
			point_tracker[current_location] = (true || point_tracker[current_location]);
		}

		// Initialize the points in the positions that have not been dropped
		target_point_cloud.add_point();
		for ( int i=0; i<total_points; i++ ) {
			if ( point_tracker[i] ) {
				hvt::point current_point;
				
				

			}
		}

	};
}