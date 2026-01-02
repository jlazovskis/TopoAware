// This file is a header file for TopoAware 
// Author: Jānis Lazovskis, 2025

// external libraries
#include <gudhi/sparsify_point_set.h>

namespace tpaw {

	// types
    typedef int64_t index;
	typedef double value;
	typedef std::vector< value > point;
	typedef std::vector< point > point_cloud;

	// functions
	// tpaw::point_cloud barycenters(gudhi::simplex_tree simplex_tree, tpaw::point_cloud points){
	point_cloud barycentric_subdivision(point_cloud points, value radius, index max_dim){}

	point_cloud sparsification(point_cloud points, value min_dist){}

	point_cloud gridification(point_cloud points, value grid_interval, point grid_origin){}

	point_cloud complement(point_cloud points, value grid_interval, index buffer){}

	point_cloud thickening(point_cloud points, value grid_interval){}

}
