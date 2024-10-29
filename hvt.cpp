// hypervolume-t 0.1.0
// Created by Jānis Lazovskis 
// GPL-3 license 

#include <misc.h>
#include <point_cloud.h>
#include <split_points.h>
#include <sparsify_points.h>
#include <make_complement.h>

// Main runtime
int main (int argc, char** argv) {
	const char* filename_in = nullptr;
	const char* filename_out = nullptr;
	const char* filename_bcs = nullptr;
	hvt::value dist_aggregate = 0;
	hvt::value dist_barycenter = 0;
	hvt::value dist_sparsify = 0;
	int kdtree_splits = 1;

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

		// Number of times to do kd-tree splitting
		// The number of regions will be 2 to the exponent of this number
		} else if (arg == "--kdtree_splits") {
			std::string parameter = std::string(argv[++i]);
			size_t next_pos;
			kdtree_splits = std::stof(parameter, &next_pos);
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
	if ( dist_aggregate < 0 ) {
		std::cerr << "Distance for aggregating either unset or nonpositive" << std::endl;
		return 0;
	}
	if ( dist_barycenter < 0 ) {
		std::cerr << "Distance for barycentric subdivison either unset or nonpositive" << std::endl;
		return 0;
	}
	if ( dist_sparsify < 0 ) {
		std::cerr << "Distance for sparsification either unset or nonpositive" << std::endl;
		return 0;
	}

	// Start timer
	auto start = std::chrono::high_resolution_clock::now();
	
	// Initialize point cloud class
	hvt::point_cloud data_step0;

	// Read input file
	auto current = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast< std::chrono::seconds >(current - start);
	std::cout << "(time: " << duration.count() << " seconds) Loading input file... " << std::flush;
	bool read_successful;
	read_successful = data_step0.load_points( filename_in );
	if ( !read_successful ) {
		std::cerr << "error, can not open file " << filename_in << std::endl;
		return 0; 
	}
	else { std::cout << "done (" << data_step0.get_size() << " points)\n";}

	// Split into parts for aggregation
	hvt::point_cloud data_step1;
	if ( dist_aggregate > 0 ) {

		current = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast< std::chrono::seconds >(current - start);
		std::cout << "(time: " << duration.count() << " seconds) Splitting dataset " << kdtree_splits << " times...\n";
	
		// Initialize
		std::vector< hvt::point_cloud > kdtree_container;
		kdtree_container.push_back(data_step0);
	
		// Iterate requested number of times
    	for ( int step = 0; step<kdtree_splits; step++ ) {
			int step_dim = step % data_step0.get_dim();
			std::vector< hvt::point_cloud > current_kdtree_container;
	
			for ( hvt::point_cloud& subset : kdtree_container ) {
				hvt::point_cloud partA;
				hvt::point_cloud partB;
				subset.split_at_coordinate( step_dim, partA, partB );
				current_kdtree_container.push_back(partA);
				current_kdtree_container.push_back(partB);
			}
	
			// Add to container for next cycle
			kdtree_container.clear();
			for ( hvt::point_cloud pc : current_kdtree_container ) { kdtree_container.push_back(pc); }
		}
	
		// Aggregate
		current = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast< std::chrono::seconds >(current - start);
		std::cout << "(time: " << duration.count() << " seconds) Aggregating in parts with minimum distance " << dist_aggregate << "... " << std::endl; 
		std::vector< hvt::point_cloud > data_step1_container;
	
		#pragma omp parallel for 
		for ( hvt::point_cloud& pc : kdtree_container ) { 
			hvt::point_cloud pc_sparsified;
			hvt::sparsify_points( pc, pc_sparsified, dist_aggregate );
			data_step1_container.push_back(pc_sparsified);
		}
	
		// Recombine
		hvt::point_cloud data_step1_preaggregated;
		for ( hvt::point_cloud& pc : data_step1_container ) {
			std::vector< hvt::point > p;
			pc.get_points(p);
			data_step1_preaggregated.add_points(p);
		}
	
		// Aggregate again
		current = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast< std::chrono::seconds >(current - start);
		std::cout << "(time: " << duration.count() << " seconds) Aggregating all together: from " << data_step1_preaggregated.get_size() << " points"; 
		hvt::sparsify_points( data_step1_preaggregated, data_step1, dist_aggregate );
		std::cout << " to " << data_step1.get_size() << " points" << std::endl; 
	
	} else { data_step1 = data_step0; }

	// Add barycenters
	current = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast< std::chrono::seconds >(current - start);
	std::cout << "(time: " << duration.count() << " seconds) Adding barycenters to all pairs and triples within " << dist_barycenter << "... " << std::flush; 
	data_step1.find_neighbors( dist_barycenter );
	hvt::point_cloud data_step2;
	std::vector<int> points_added;
	hvt::split_points( data_step1, data_step2, points_added );
	std::cout << " done (" << data_step2.get_size() << " points = " << points_added[0] << " from pairs, " << points_added[1] << " from triples)\n";

	// Export barycenters
	if ( filename_bcs != NULL ) {
		std::cout << "Exporting barycentric subdivision to file... " << std::flush; 
		const bool bcs_flag = false;
		data_step1.export_points( filename_bcs, bcs_flag );
		std::cout << "done" << std::endl;
	}

	// Sparsify
	current = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast< std::chrono::seconds >(current - start);
	std::cout << "(time: " << duration.count() << " seconds) Sparsifying with minimum distance " << dist_sparsify << "... " << std::flush; 
	hvt::point_cloud data_step3;
	hvt::sparsify_points( data_step2, data_step3, dist_sparsify );
	std::cout << " done (" << data_step3.get_size() << " points)\n"; 		

	// Export
	current = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast< std::chrono::seconds >(current - start);
	std::cout << "(time: " << duration.count() << " seconds) Exporting sparsified point cloud to file... " << std::flush; 
	const bool header_flag = false;
	data_step3.export_points( filename_out, header_flag );
	std::cout << "done" << std::endl;

	// Exit
	current = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast< std::chrono::seconds >(current - start);
	std::cout << "--------------------\nFinished in " << duration.count() << " seconds" << std::endl;
	return 1;
}