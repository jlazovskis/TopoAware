// hypervolume-t 
// Created by Jānis Lazovskis 
// GPL-3 license 

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
}

#include <simplicial_complex.h>

static const size_t num_coefficient_bits = 8;


int main (int argc, char** argv) {
	const char* filename = nullptr;
	float threshold;

	// Read arguments
	for (int i = 1; i < argc; ++i) {
		const std::string arg(argv[i]);

		// Threshold
		if (arg == "--threshold") {
			std::string parameter = std::string(argv[++i]);
			size_t next_pos;
			threshold = std::stof(parameter, &next_pos);

		// Main file
		} else {
			filename = argv[i];
		}
	}


	// Initialize distance matrix class
	hvt::simplicial_complex cpx;

	// Read input file
    bool read_successful;
	read_successful = cpx.load_point_cloud(filename);
    if( !read_successful ) {
        std::cerr << "Error opening file " << filename << std::endl;
        return 0;
    }

    // Find neighbors
    bool compute_successful;
	compute_successful = cpx.get_neighbors(threshold);
    if( !compute_successful ) {
        std::cerr << "Error computing neighbors" << std::endl;
        return 0;
    }

	// Testing
	cpx.mat.print_me();

	return 1;
}