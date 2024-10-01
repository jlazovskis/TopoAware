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
	std::cout << "hypervolume-t v0.1\n--------------------\n";

	// Start timer
	auto start = std::chrono::high_resolution_clock::now();

	// Initialize point cloud class
	hvt::point_cloud data_step0;

	//R// Convert input from R to C++
    int dim = data.size();
    int samples = data.nrows();
    for ( int i = 0; i < samples; i++ ) {
    	hvt::point point;
    	for ( int j = 0; j < dim; j++ ) {
        	Rcpp::NumericVector column = data[j];
    		point.push_back(column[i]);
    	}
    	data_step0.add_point(point);
    } 
	
	return data;
}