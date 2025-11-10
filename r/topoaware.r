# Functions
barycenters <- function(simplex_tree, points){
	return( 1 )
}


barycentric_subdivision <- function(points, radius, max_dim=2){
	complex_vr_container <- RipsComplex$new(data=points, max_edge_length=radius)
	complex_vr <- complex_vr_container$create_simplex_tree(max_dimension=max_dim)

	return( barycenters(complex_vr, points) )
}


sparsification <- function(points, min_dist){
	return( 1 )
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
