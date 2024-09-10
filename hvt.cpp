// hypervolume-t 
// Created by Jānis Lazovskis 
// GPL-3 license 

#include <misc.h>
#include <point_cloud.h>

// Main runtime
int main (int argc, char** argv) {
	const char* filename_in = nullptr;
	const char* filename_out = nullptr;
	hvt::value dist_barycenter;
	hvt::value dist_sparsify;

	// Read arguments
	for (int i = 1; i < argc; ++i) {
		const std::string arg(argv[i]);

		// Input file
		if (arg == "--input") {
			filename_in = argv[++i];
		}

		// Output file
		if (arg == "--output") {
			filename_out = argv[++i];
		}

		// Threshold for adding barycenters
		if (arg == "--bdist") {
			std::string parameter = std::string(argv[++i]);
			size_t next_pos;
			dist_barycenter = std::stof(parameter, &next_pos);

		// Minimum distance for sparsifying
		} else if (arg == "--sdist") {
			std::string parameter = std::string(argv[++i]);
			size_t next_pos;
			dist_sparsify = std::stof(parameter, &next_pos);
		}

	}

	// Initialize point cloud class
	hvt::point_cloud data_step0;

	// Read input file
	bool read_successful;
	read_successful = data_step0.load_points( filename_in );
	if ( !read_successful ) { std::cerr << "Error opening file " << filename_in << std::endl; return 0; }
	else { std::cout << "File loaded with " << data_step0.get_size() << " points" << std::endl;}

	// Find neighbors
	data_step0.find_neighbors( dist_barycenter );

	// Add barycenters
	hvt::point_cloud data_step1;
	std::vector<int> points_added;
	bool enrich_successful;
	enrich_successful = data_step1.split_points( data_step0, points_added );
	if( !enrich_successful ) {
		std::cerr << "Error adding barycenters at threshold " << dist_barycenter << std::endl;
		return 0;
	}
	else {
		std::cout << "New point cloud created with " << data_step1.get_size() << " points by adding barycenters ";
		std::cout << "(" << points_added[0] << " by splitting pairs, " << points_added[1] << " by splitting triples)" << std::endl; 
	}

	// TESTING
	// data_step0.print_me();

	// Sparsify
	hvt::point_cloud data_step2;
	bool sparsify_successful;
	sparsify_successful = data_step2.sparsify_points( data_step1, dist_sparsify );
	if( !sparsify_successful ) {
		std::cerr << "Error sparsifying at minimum distance " << dist_sparsify << std::endl;
		return 0;
	}

	// Export
	bool write_successful;
	write_successful = data_step2.export_csv( filename_out );
	if( !write_successful ) {
		std::cerr << "Error writing to file " << filename_out << std::endl;
		return 0;
	}

	return 1;
}