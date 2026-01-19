// This file is a header file for TopoAware 
// Author: Jānis Lazovskis, 2025

// external libraries
#include <gudhi/Rips_complex.h>
#include <gudhi/Simplex_tree.h>
#include <gudhi/distance_functions.h>
#include <gudhi/sparsify_point_set.h>

namespace topoaware
	{

	// types
    typedef int64_t index;
	typedef double value;
	typedef std::vector< value > point;

	// class
	class point_cloud
		{
		private:

			// vector of points
			std::vector< point > points; 

		public:

			void set_points( std::vector< point >& );

			void get_points( std::vector< point >& );

			// constructor
			point_cloud() {};

			// add barycentric subdivision to point cloud
			void barycentric_subdivision(value radius, index max_dim){
				using Simplex_tree = Gudhi::Simplex_tree<Gudhi::Simplex_tree_options_default>;
  				using Filtration_value = Simplex_tree::Filtration_value;
  				using Rips_complex = Gudhi::rips_complex::Rips_complex<Filtration_value>;

				// construct vr complex
				Rips_complex rips_complex_from_points(points, radius, Gudhi::Euclidean_distance());
  				Simplex_tree stree;
  				rips_complex_from_points.create_complex(stree, max_dim);

  				// get barycenters and return
  		
			};

			// sparsify point cloud
			void sparsification(value min_dist){};

			// align point cloud to grid
			void gridification(value grid_interval, point grid_origin){};

			// only when point cloud is on grid
			// construct ``complement'' of point cloud
			void complement(value grid_interval, index buffer){};

			// only when point cloud is on grid
			// construct thickening of point cloud
			void thickening(value grid_interval){};
	};

}
