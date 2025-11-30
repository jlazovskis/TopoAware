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

	# Return output
	return( output )
}


# For the following functions, points is either:
#  * a list of points in Rn, or 
#  * a data frame with nrows=npoints, and ncols=ndimensions

barycenters <- function(simplex_tree, points){

	# Standardize input
	points_std <- standardize_points(points)

	# Construct output
	output <- st$get_simplices()$simplex # Access simplex indices
		%>% lapply(function(x) x+1) # From 0-indexed to 1-indexed
		%>% lapply(function(x) do.call(rbind,lapply(x,function(y) X[[y]]))) # Get points
		%>% lapply(function(x) apply(x,2,mean)) # Take average

	return( output )
}


barycentric_subdivision <- function(points, radius, max_dim=2){

	# Standardize input
	points_std <- standardize_points(points)

	# Contrcut VR complex	
	complex_vr_container <- RipsComplex$new(data=points_std, max_edge_length=radius)
	complex_vr <- complex_vr_container$create_simplex_tree(max_dimension=max_dim)

	# Return list of points
	return( barycenters(complex_vr, points_std) )
}


sparsification <- function(points, min_dist){

	output <- standardize_points(points) # Standardize input
		%>% sparsify_point_set((min_dist)**2) # Apply Python function

	# Return output
	return( output )
}


gridification <- function(points, grid_interval, grid_origin){

	output <- standardize_points(points) # Standardize input
		%>% lapply(function(x) x-grid_origin) # Subtract origin
		%>% lapply(function(x) sapply(x,function(y) y %/% grid_interval)) # Divide by grid interval
		%>% unique() # Take only unique values

	# Return output
	return( output )
}


complement <- function(grid, grid_interval=0, buffer=2){

	# Get min and max values
	mins <- apply(do.call(rbind, grid), 2, min)
	maxs <- apply(do.call(rbind, grid), 2, max)

	# Create a full grid
	ranges <- sapply(maxs-mins, function(x) x %/% grid_interval + buffer*2)
	mask_array <- array(rep(TRUE,prod(ranges)),ranges)

	# Iterate over input grid to remove existing values
	for (i in 1:length(grid)) {
		coords <- apply(grid[i][[1]]-mins, function(x) x %/% grid_interval + buffer)
		mask_array <- do.call(`[<-`, c(list(arr), as.list(coords), list(FALSE)))
	}

	# Construct output
	output <- which(mask_array==TRUE, arr.ind=TRUE) # Get correct indices
		%>% apply(1, function(x) do.call(rbind,lapply(x, function(y) y*grid_interval))) # Get values
		%>% apply(1, identity, simplify=FALSE) # Make as list of numeric

	return( output )
}


thickening <- function(points, grid_interval=0){
	return( 1 )
}
