// This file is a header file for TopoAware 
// Author: Jānis Lazovskis, 2025

// external libraries
#include <cassert>
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
			std::vector< point > points; 	// vector of points
			index dim;						// dimension

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

				// initialize 
				assert (grid_origin.size() == points[0].size());
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
				std::vector<point> data_new;
				for (auto it = data_set.begin(); it != data_set.end(); ++it) {
					data_new.push_back(*it);
			    }
				set_points(data_new);
			};

			// construct complement of a grid
			void complement(value grid_interval, index buffer){

				// get boundaries and ranges
				point bdry_min = points[0];
				point bdry_max = points[0];
				for ( point p : points ){
				    for (int d=0; d<dim; d++){
				    	if ( p[d] < bdry_min[d] ){ bdry_min[d]=p[d]; }
				    	if ( p[d] > bdry_max[d] ){ bdry_max[d]=p[d]; }
				    }
				}
				std::vector<index> ranges;
			    for (int d=0; d<dim; d++){
			    	ranges.push_back(std::round(1+(bdry_max[d]-bdry_min[d])/grid_interval));
			    }

			    // get max index
			    index ind_max = 1;
			    for (int d=0; d<dim; d++){ ind_max = ind_max*(2*buffer + ranges[d]); }
				
			    // get indices that should be skipped
				std::set<index> data_indices;
				for ( point p : points ){
					index data_index = 0;
					index data_coeff = 1;
				    for (int d=0; d<dim; d++){
						data_index += std::round(buffer + (p[d]-bdry_min[d])/grid_interval)*data_coeff;
						data_coeff *= ( ranges[d]+2*buffer );
					}
					data_indices.insert(data_index);
				}

				// construct new data without indices to skip
				std::vector<point> data_new;
				index ind_current = 0;
				auto it = data_indices.begin();
				index ind_skip = *it;
				while ( ind_current<ind_max ){
					while ( ind_current<ind_skip ){
						point np;
						index coeff_mod = 1;
						index coeff_denom = 1;
				    	for (int d=0; d<dim; d++){
				    		coeff_mod *= ranges[d]+2*buffer;
				    		np.push_back( (bdry_min[d]-buffer*grid_interval) + ((ind_current % coeff_mod) / coeff_denom)*grid_interval );
				    		coeff_denom *= ranges[d]+2*buffer;
				    	}
			    		data_new.push_back(np);
						ind_current++;
					}
					it++;
					if ( it == data_indices.end() ){ ind_skip = ind_max; }
					else { ind_skip = *it; }
					ind_current++;
				}

				// replace
				set_points(data_new);
			};

			// construct thickening of point cloud
			void thickening(value grid_interval){

				// initialize containers
				std::set<point> data_set;
				std::vector<point> directions = {{-1},{0},{1}};
			    for (int d=1; d<dim; d++){
			    	std::vector<point> temp_directions;
			    	for (point dir : directions){
			    		point tp1 = dir; tp1.push_back(-1); temp_directions.push_back(tp1);
			    		point tp2 = dir; tp2.push_back(0); temp_directions.push_back(tp2);
			    		point tp3 = dir; tp3.push_back(1); temp_directions.push_back(tp3);
			    	}
			    	directions = temp_directions;
			    }

				// iterate over points and dimensions
				for (point p : points){
					for (point dir : directions){
		    			point np;
			    		for (int d=0; d<dim; d++){
			    			np.push_back(p[d] + dir[d]*grid_interval);
			    		}
			    		data_set.insert(np);
					}
				}

				// create new point container and replace
				std::vector<point> data_new;
				for (auto it = data_set.begin(); it != data_set.end(); ++it) {
					data_new.push_back(*it);
			    }
				set_points(data_new);				
			};

	};

}
