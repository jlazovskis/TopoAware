// This file is part of hypervolume-t 
// Header file for brycentric subdivision of a point cloud

namespace hvt {

	// Add barycenters from another point cloud
	void split_points(
			hvt::point_cloud initial_point_cloud,
			hvt::point_cloud& target_point_cloud,
			std::vector<int>& points_added, 
			const hvt::value sparsification_dist ) {
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
			target_point_cloud.add_point(point);
		
			// Iterate through neighbors
			for ( int j = 0; j < current_neighbors.size() ; j++ ) {
				const hvt::index index_j = get_index(current_neighbors[j]);
				// const float dist_ij = initial_point_cloud.get_dist(i,index_j);
		
				// Add midpoint of every two points
				std::vector< hvt::point > neighboring_points;
				hvt::point current_point_b;
				initial_point_cloud.get_point(index_j,current_point_b);
				neighboring_points.push_back(current_point_b);
		
				// Declare, compute, record new point
				hvt::point new_point;
				hvt::point_average( current_point, neighboring_points, new_point );
				target_point_cloud.add_point(new_point);
				counter1 += 1;
		
				// Iterate once again for triples
				for ( int k = j+1 ; k < current_neighbors.size() ; k++ ) {
					const hvt::index index_k = get_index(current_neighbors[k]);
					const float dist_jk = initial_point_cloud.get_dist(index_j,index_k);
		

					// Add another neighboring point
					hvt::point current_point_c;
					initial_point_cloud.get_point(index_k,current_point_c);
					neighboring_points.push_back(current_point_c);
					
					// Declare, compute, record new point
					hvt::point new_point2;
					hvt::point_average( current_point, neighboring_points, new_point2 );
					target_point_cloud.add_point(new_point2);
					counter2 += 1;
					
					// Drop last element two have two starting points for next triple
					neighboring_points.pop_back();

				}
			}
		}
		
		// Record progress and finish
		new_point_count.push_back(counter1);
		new_point_count.push_back(counter2);
		points_added = new_point_count;

	};

}