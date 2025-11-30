# Packages
library(devtools)
library(dplyr)

# rgudhi fork
install_github("jlazovskis/rgudhi-subsampling@export")

# Functions
standardize_points <- function(input) {

	# Convert from data frame if necessary
	if (class(input) == "data.frame"){
		output <- apply(input, 1, as.numeric)
		output <- split(input, seq(ncol(points_list)))
	}
	else {
		stopifnot(class(input) == "list")
		output <- input
	}

	# Ensure names are null
	names(output) <- NULL

	# Return
	return( output )
}


# For the following functions, points is either:
#  * a list of points in Rn, or 
#  * a data frame with nrows=npoints, and ncols=ndimensions

barycenters <- function(simplex_tree, points){

	# Standardize input
	points_std <- standardize_points(points)

	# Initiate output list
	points_split <- list()

	# Iterate over simplices
	for (i in 1:nrow(simplex_tree$get_simplices())) {
		indices <- lapply(simplex_tree$get_simplices()[i, ][[1]], function(x) x+1)
		simplex_vertices <- do.call(rbind, points_std[indices[[1]]])
		new_point <- apply(simplex_vertices, 2, mean)
		points_split <- append(points_split, list(new_point))
	}
 
	return( points_split )
}


barycentric_subdivision <- function(points, radius, max_dim=2){
	complex_vr_container <- RipsComplex$new(data=points, max_edge_length=radius)
	complex_vr <- complex_vr_container$create_simplex_tree(max_dimension=max_dim)

	return( barycenters(complex_vr, points) )
}


sparsification <- function(points, min_dist){

	points_list <- standardize_points(points)

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
