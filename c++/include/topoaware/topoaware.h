// This file is a header file for TopoAware 
// Author: Jānis Lazovskis, 2025

// external libraries
#include <cmath>
#include <gudhi/Rips_complex.h>
#include <gudhi/Simplex_tree.h>
#include <gudhi/distance_functions.h>
#include <gudhi/sparsify_point_set.h>
#include <CGAL/Epick_d.h>

namespace topoaware
	{

	// types for topoaware
    typedef int64_t index;
	typedef double value;
	typedef std::vector< value > point;

	// types for CGAL
	typedef CGAL::Epick_d< CGAL::Dynamic_dimension_tag > kernel;
	typedef typename kernel::Point_d Point_d;

	// compute quotient for float division
	index quotient(value numerator, value denominator) {
		return (index)((numerator - std::fmod(numerator, denominator)) / denominator);
	}

	// class
	class point_cloud
		{
		private:

			// vector of points
			std::vector< point > points; 

			// dimension
			index dim;

		public:

			void set_points( std::vector< point >& vector_in ) {
				points = vector_in;
				dim = points[0].size();
			}
			void get_points( std::vector< point >& vector_out ) {
				vector_out = points;
			}
			void add_point( point point_in ) {
				points.push_back(point_in);
			}
			void print_points() {
				for ( point p : points ) {
					for ( value v : p ) {
						std::cout << v << " ";
					}
				std::cout << std::endl;
				}
			}

			// constructor
			point_cloud() {};

			// perform barycentric subdivision on point cloud
			void barycentric_subdivision(value radius, index max_dim){
				using Simplex_tree = Gudhi::Simplex_tree<Gudhi::Simplex_tree_options_default>;
  				using Filtration_value = Simplex_tree::Filtration_value;
  				using Rips_complex = Gudhi::rips_complex::Rips_complex<Filtration_value>;

				// construct vr complex
				Rips_complex rips_complex_from_points(points, radius, Gudhi::Euclidean_distance());
  				Simplex_tree stree;
  				rips_complex_from_points.create_complex(stree, max_dim);

  				// get barycenters and set
				std::vector< point > data_new;
				for (auto simplex : stree.complex_simplex_range()) {
					point new_vertex(dim, 0.0);
					index vertex_num = 0;
    				for (auto vertex : stree.simplex_vertex_range(simplex)) {
    					vertex_num++;
    					for (index cur_dim=0; cur_dim<dim; cur_dim++) {
	      					new_vertex[cur_dim] += points[vertex][cur_dim];
	      				}
    				}
    				for (index cur_dim=0; cur_dim<dim; cur_dim++) {
    					new_vertex[cur_dim] /= vertex_num;
    				}
    				data_new.push_back(new_vertex);
				}
				set_points(data_new);
			};

			// sparsify point cloud
			void sparsification(value min_dist){

				// create the point cloud as a CGAL kernel
				std::vector<Point_d> points_asCGAL;
				for ( auto p : points ) {
					Point_d current_point(dim, p.begin(), p.end());
					points_asCGAL.push_back(current_point);
				}

				// sparsify and set
				std::vector< point > data_new;
				kernel new_kernel;
				Gudhi::subsampling::sparsify_point_set(new_kernel, points_asCGAL, min_dist*min_dist, std::back_inserter(data_new));
				set_points(data_new);
			};

			// align point cloud to grid
			void gridification(value grid_interval, point grid_origin){

				// create set container 
				std::set<point> data_set;

				// add elements from points to set
				for ( point p : points ){
					point np;
				    for (int d=0; d<dim; d++){
				    	np.push_back(grid_origin[d] + grid_interval*topoaware::quotient(p[d]-grid_origin[d], grid_interval));
				    }
				    data_set.insert(np);
				}

				// create new point container and replace
				std::vector< point > data_new;
				for (auto it = data_set.begin(); it != data_set.end(); ++it) {
					data_new.push_back(*it);
			    }
				set_points(data_new);
			};

			// only when point cloud is on grid
			// construct ``complement'' of point cloud
			void complement(value grid_interval, index buffer){};

			// only when point cloud is on grid
			// construct thickening of point cloud
			void thickening(value grid_interval){};
	};

}
