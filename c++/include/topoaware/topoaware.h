// This file is a header file for TopoAware 
// Author: Jānis Lazovskis, 2025

// external libraries
#include <gudhi/Simplex_tree.h>
#include <gudhi/sparsify_point_set.h>

namespace topoaware {

	// types
    typedef int64_t index;
	typedef double value;
	typedef std::vector< value > point;
	typedef std::vector< point > point_cloud;

	// functions
	int barycenters(Gudhi::Simplex_tree<> simplex_tree, point_cloud points){
		return 1;
	}
	
	// point_cloud barycentric_subdivision(point_cloud points, value radius, index max_dim){}

	// point_cloud sparsification(point_cloud points, value min_dist){}

	// point_cloud gridification(point_cloud points, value grid_interval, point grid_origin){}

	// point_cloud complement(point_cloud points, value grid_interval, index buffer){}

	// point_cloud thickening(point_cloud points, value grid_interval){}

}
