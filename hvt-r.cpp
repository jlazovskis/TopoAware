// hypervolume-t (port to R)
// Created by Jānis Lazovskis 
// GPL-3 license 

#include <include/misc.h>
#include <include/point_cloud.h>

#include <Rcpp.h>
// [[Rcpp::export]]

// Main runtime
Rcpp::DataFrame hypervolume_t(
		Rcpp::DataFrame data,       // The input data frame
		hvt::value dist_barycenter, // Distance below which all pairs and triples will be subdivided
		hvt::value dist_sparsify)   // Minimum distance between all sparsified points
	{

	// TODO: Check arguments
	std::cout << "hypervolume-t v0.1\n--------------------\n";

	// Start timer
	auto start = std::chrono::high_resolution_clock::now();

	// Initialize point cloud class
	hvt::point_cloud data_step0;

	//R// Convert input from R to C++
    const int dim = data.size();
    const int samples = data.nrows();
    for ( int i = 0; i < samples; i++ ) {
    	hvt::point point;
    	for ( int j = 0; j < dim; j++ ) {
        	Rcpp::NumericVector column = data[j];
    		point.push_back(column[i]);
    	}
    	data_step0.add_point(point);
    } 
	
	// Find neighbors
	data_step0.find_neighbors( dist_barycenter );

	// Add barycenters
	std::cout << "Adding barycenters to all pairs and triples within " << dist_barycenter << "... " << std::flush; 
	hvt::point_cloud data_step1;
	std::vector<int> points_added;
	data_step1.split_points( data_step0, points_added, dist_sparsify );
	std::cout << " done (" << data_step1.get_size() << " points = " << points_added[0] << " from pairs, " << points_added[1] << " from triples)\n";

	// Sparsify
	std::cout << "Sparsifying with minimum distance " << dist_sparsify << "... " << std::flush; 
	hvt::point_cloud data_step2;
	data_step2.sparsify_points( data_step1, dist_sparsify );
	std::cout << " done (" << data_step2.get_size() << " points)\n"; 		


	//R// Convert output from C++ to R
	//R// Step 1: Create container to hold columns
	std::vector< Rcpp::NumericVector > data_out_container;
   	for ( int j = 0; j < dim; j++ ) {
   		Rcpp::NumericVector column;
   		data_out_container.push_back(column);
   	}

   	//R// Step 2: Go through all points and add values to columns
    const int samples_out = data_step2.get_size();
	for ( int i = 0; i < samples_out; i++ ) {
		hvt::point current_point;
		data_step2.get_point(i,current_point);
   		for ( int j = 0; j < dim; j++ ) {
   			data_out_container[j].push_back(current_point[j]);
   		}
   	}

   	//R// Step 3: Make R data type and add columns
	Rcpp::DataFrame data_out;
   	for ( int j = 0; j < dim; j++ ) {
   		data_out.push_back(data_out_container[j], "x"+std::to_string(j));
   	}

	// Exit
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast< std::chrono::seconds >(stop - start);
	std::cout << "--------------------\nFinished in " << duration.count() << " seconds" << std::endl;

   	//R// Return final dataframe
	return data_out;
}