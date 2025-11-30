# Packages
library(devtools)
library(dplyr)

# rgudhi fork
install_github("jlazovskis/rgudhi-subsampling@export")

# Functions
barycenters <- function(simplex_tree, points){
	return( 1 )
}


barycentric_subdivision <- function(points, radius, max_dim=2){
	complex_vr_container <- RipsComplex$new(data=points, max_edge_length=radius)
	complex_vr <- complex_vr_container$create_simplex_tree(max_dimension=max_dim)

	return( barycenters(complex_vr, points) )
}

# Points is either:
#  * a list of points in Rn, or 
#  * a data frame with nrows=npoints, and ncols=ndimensions
sparsification <- function(points, min_dist){

	# Convert from data frame if necessary
	if (class(points) == "data.frame"){
		points_list <- apply(points, 1, as.numeric)
		points_list <- split(points_list, seq(ncol(points_list)))
	}
	else {
		stopifnot(class(points) == "list")
		points_list <- points
	}

	# Ensure names are null
	names(points_list) <- NULL

	# Return Python function
	return( sparsify_point_set(points_list, (min_dist)**2) )
}


gridification <- function(points, grid_interval, grid_origin){
	return( 1 )
}


complement <- function(points, grid_interval=0, buffer=2){
	return( 1 )
}


thickening <- function(points, grid_interval=0){
	return( 1 )
}
