// This file is part of TopoAware 
// Header file for all includes, special types, math functions

// stl
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <chrono>
#include <cmath>
#include <numeric>

// external libraries
#include <gudhi/sparsify_point_set.h>
#include <CGAL/Epick_d.h>

// tpaw
namespace tpaw {
    typedef int64_t index;
	typedef double value;
	typedef std::vector< tpaw::value > point;

	// A pair type to hold an index and a distance, meant for neighbors of a point
	// Adapted from https://github.com/Ripser
	typedef std::pair<tpaw::index, tpaw::value> index_diameter_t;
	typedef std::vector<tpaw::index_diameter_t> neighbors;
	tpaw::index get_index(const index_diameter_t& i) { return i.first; }
	tpaw::value get_diameter(const index_diameter_t& i) { return i.second; }

	// Function that returns the average of points in an input list of points
	void point_average( tpaw::point base, std::vector<tpaw::point> neighbor_points, tpaw::point& target) {

		// Get number of points and dimension
		const int num_points = neighbor_points.size()+1;
		const int dim = base.size();
		assert (dim > 0);

		// Get number of points
		const int denominator = neighbor_points.size() + 1;
		assert (denominator > 1);

		// Place base point values in target point
		for (const tpaw::value value : base) {
			target.push_back(value); 
		}

		// Add neighboring point values to target point coordinates
		for ( const tpaw::point point : neighbor_points) { 
			for ( int i = 0; i < dim; i++ ) {
				target[i] += point[i];
			}
		}

		// Divide each coordinate by number of points
		for ( int i = 0; i < dim; i++ ) {
			target[i] = target[i]/num_points;
		}
	};

	// Function that does float division, returns the quotient and remainder as objects
	std::vector<tpaw::value> fdiv(tpaw::value numerator, tpaw::value denominator) {
		std::vector<tpaw::value> return_array;
		return_array.push_back(int(numerator/denominator));
		return_array.push_back(std::fmod(numerator,denominator));
		return return_array;
	};
}