// This file is part of hypervolume-t 

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
	typedef float value;
    typedef int64_t index;

	// From Ripser
	typedef std::pair<hvt::index, hvt::value> index_diameter_t;
	hvt::index get_index(const index_diameter_t& i) { return i.first; }
	hvt::value get_diameter(const index_diameter_t& i) { return i.second; }

}