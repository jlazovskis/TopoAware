// This file is part of hypervolume-t 
// Header file for sparsifying points of a point cloud

namespace hvt {

	// Dimension and point types
	typedef CGAL::Epick_d< CGAL::Dynamic_dimension_tag > kernel;
	typedef typename kernel::Point_d Point_d;	

	// Adapted from https://gudhi.inria.fr/doc/latest/example_sparsify_point_set_8cpp-example.html
	void sparsify_points(
			hvt::point_cloud initial_point_cloud,
			hvt::point_cloud& target_point_cloud,
			const hvt::value distance ) {
		
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
		std::vector<hvt::point> new_points;
		kernel new_kernel;
		Gudhi::subsampling::sparsify_point_set(new_kernel, initial_point_cloud_asCGAL, distance, std::back_inserter(new_points));
 		
		// Give target point cloud the new points
		target_point_cloud.add_points(new_points);

	};

}