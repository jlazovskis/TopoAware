// This file is a header file for TopoAware 
// Author: Jānis Lazovskis, 2025

// external libraries
#include <gudhi/sparsify_point_set.h>

namespace tpaw {

	// types
    typedef int64_t index;
	typedef double value;
	typedef std::vector< tpaw::value > point;
	typedef std::vector< tpaw::point > point_cloud;

	// function
	tpaw::point_cloud barycenters(gudhi::simplex_tree simplex_tree, tpaw::point_cloud points){

	}

	// function
	tpaw::point_cloud grid_points(tpaw::point_cloud points, tpaw::value grid_interval, tpaw::point grid_origin){
		
	}


}
