// hypervolume-t 0.1.0
// Created by Jānis Lazovskis 
// GPL-3 license 

#include <misc.h>
#include <point_cloud.h>
#include <split_points.h>
#include <sparsify_points.h>

// Main runtime
int main (int argc, char** argv) {
	const char* filename_in = nullptr;
	const char* filename_out = nullptr;
	const char* filename_bcs = nullptr;
	hvt::value dist_aggregate = 0;
	hvt::value dist_barycenter = 0;
	hvt::value dist_sparsify = 0;

	// Read arguments
	for (int i = 1; i < argc; ++i) {
		const std::string arg(argv[i]);

		// Input file
		if (arg == "--input") {
			filename_in = argv[++i];

		// Output file
		} else if (arg == "--output") {
			filename_out = argv[++i];

		// Barycentric subdivison output file
		} else if (arg == "--export_bcs") {
			filename_bcs = argv[++i];

		// Threshold for aggregating points
		// Every point will be at least this far from other points
		} else if (arg == "--adist") {
			std::string parameter = std::string(argv[++i]);
			size_t next_pos;
			dist_aggregate = std::stof(parameter, &next_pos);

		// Threshold for adding barycenters
		// For every pair of points less than this distance, a new point will be added
		} else if (arg == "--bdist") {
			std::string parameter = std::string(argv[++i]);
			size_t next_pos;
			dist_barycenter = std::stof(parameter, &next_pos);

		// Minimum distance for sparsifying
		// Every point will be at least this far from other points
		} else if (arg == "--sdist") {
			std::string parameter = std::string(argv[++i]);
			size_t next_pos;
			dist_sparsify = std::stof(parameter, &next_pos);

		}
	}

	// Check arguments
	std::cout << "hypervolume-t 0.1.0\n--------------------\n";
	if ( filename_in == NULL ) {
		std::cerr << "Missing input file" << std::endl;
		return 0;
	}
	if ( filename_out == NULL ) {
		std::cerr << "Missing output file" << std::endl;
			return 0;
	}
	if ( dist_aggregate <= 0 ) {
		std::cerr << "Distance for aggregating either unset or nonpositive" << std::endl;
		return 0;
	}
	if ( dist_barycenter <= 0 ) {
		std::cerr << "Distance for barycentric subdivison either unset or nonpositive" << std::endl;
		return 0;
	}
	if ( dist_sparsify <= 0 ) {
		std::cerr << "Distance for sparsification either unset or nonpositive" << std::endl;
		return 0;
	}

	// Start timer
	auto start = std::chrono::high_resolution_clock::now();
	
	// Initialize point cloud class
	hvt::point_cloud data_step0;

	// Read input file
	std::cout << "Loading input file... " << std::flush;
	bool read_successful;
	read_successful = data_step0.load_points( filename_in );
	if ( !read_successful ) {
		std::cerr << "error, can not open file " << filename_in << std::endl;
		return 0; 
	}
	else { std::cout << "done (" << data_step0.get_size() << " points)\n";}

	// Sparsify
	std::cout << "Aggregating with minimum distance " << dist_aggregate << "... " << std::flush; 
	hvt::point_cloud data_step1;
	hvt::sparsify_points( data_step0, data_step1, dist_aggregate );
	std::cout << " done (" << data_step1.get_size() << " points)\n"; 

	// Find neighbors
	data_step1.find_neighbors( dist_barycenter );

	// Add barycenters
	std::cout << "Adding barycenters to all pairs and triples within " << dist_barycenter << "... " << std::flush; 
	hvt::point_cloud data_step2;
	std::vector<int> points_added;
	hvt::split_points( data_step1, data_step2, points_added, dist_barycenter );
	std::cout << " done (" << data_step2.get_size() << " points = " << points_added[0] << " from pairs, " << points_added[1] << " from triples)\n";

	// Export barycenters
	if ( filename_bcs != NULL ) {
		std::cout << "Exporting barycentric subdivision to file... " << std::flush; 
		data_step1.export_points( filename_bcs );
		std::cout << "done" << std::endl;
	}

	// Sparsify
	std::cout << "Sparsifying with minimum distance " << dist_sparsify << "... " << std::flush; 
	hvt::point_cloud data_step3;
	hvt::sparsify_points( data_step2, data_step3, dist_sparsify );
	std::cout << " done (" << data_step3.get_size() << " points)\n"; 		

	// Export
	std::cout << "Exporting sparsified point cloud to file... " << std::flush; 
	data_step3.export_points( filename_out );
	std::cout << "done" << std::endl;

	// Exit
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast< std::chrono::seconds >(stop - start);
	std::cout << "--------------------\nFinished in " << duration.count() << " seconds" << std::endl;
	return 1;
}