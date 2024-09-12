// This file is part of hypervolume-t 
// Header file for all includes, special types, math functions

// stl
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cmath>
#include <numeric>

// hvt
namespace hvt {
    typedef int64_t index;
	typedef double value;
	typedef std::vector< hvt::value > point;

	// A pair type to hold an index and a distance, meant for neighbors of a point
	// Adapted from https://github.com/Ripser
	typedef std::pair<hvt::index, hvt::value> index_diameter_t;
	typedef std::vector<hvt::index_diameter_t> neighbors;
	hvt::index get_index(const index_diameter_t& i) { return i.first; }
	hvt::value get_diameter(const index_diameter_t& i) { return i.second; }

	// Function that returns the average of points in an input list of points
	void point_average( hvt::point base, std::vector<hvt::point> neighbor_points, hvt::point& target) {

		// Get dimension 
		int dim = base.size();
		assert (dim > 0);

		// Get number of points
		int denominator = neighbor_points.size() + 1;
		assert (denominator > 1);

		// Place base point values in target point
		for (const hvt::value value : base) {
			target.push_back(value); 
		}

		// Add neighboring point values to target point coordinates
		for ( const hvt::point point : neighbor_points) { 
			for ( int i = 0; i < dim; i++ ) {
				target[i] += point[i];
			}
		}

		// Divide each coordinate by number of points
		for ( int i = 0; i < dim; i++ ) {
			target[i] = target[i]/dim;
		}
	};
}