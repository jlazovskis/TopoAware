# Packages for computing
import numpy as np
import pandas as pd
import gudhi as gd
from gudhi import subsampling as subs

# Packages for visualization, timing
from matplotlib import pyplot as plt
from timeit import default_timer



# input: SimplexTree class, vertex coordinates (nrows=npoints, ncols=ndims)
# output: collection of points (npoints x ndims), imput complex points + all barycenters of input complex simplices
def barycenters(simplex_tree, points):
	
	# check
	if type(points) == type([]):
		assert len(points)>0 ; "Empy list passed"
	elif type(points) == type(np.array([])):
		assert points.shape[0]>0 ; "Empy list passed"

	# compute
	points_return = []
	for simplex in simplex_tree.get_simplices():
		points_return.append(np.mean([points[s] for s in simplex[0]],axis=0))

	# return
	return np.array(points_return)



# input: collection of points (npoints x ndims), distance at which to construct simplices, maximum dimension (inclusive)
# output: collection of points (npoints x ndims)
def barycentric_subdivision(points, radius, max_dim=2):

	return 1



# input: collection of points (npoints x ndims), distance at which to sparsify
# output: collection of points (npoints x ndims)
def sparsification(points, min_dist):

	return 1



# input: collection of points (npoints x ndims), grid size interval, grid origin
# output: collection of points (npoints x ndims), each at one of the grid points
def gridification(points, grid_interval, grid_origin=[0]):

	# check
	dim_p = 0
	if type(points) == type([]):
		assert len(points)>0 ; "Empy list passed"
		dim_p = len(points[0])
	elif type(points) == type(np.array([])):
		assert points.shape[0]>0 ; "Empy list passed"
		dim_p = points[0].shape[0]
	assert grid_interval > 0 ; "Grid interval must be positive"
	if ((type(grid_origin)==type(())) or (type(grid_origin)==type(()))):
		assert len(grid_origin) == dim_p ; "Length (and type) of grid_origin must match length of elements of points" 
	elif type(grid_origin)==type(np.array([])):
		assert grid_origin.shape[0] == dim_p ; "Length (and type) of grid_origin must match length of elements of points" 

	# compute
	points_return = set()
	dim = points[0].shape[0]
	for point in points:
		new_point = [float(grid_interval * ((p-grid_origin[i])//grid_interval) + grid_origin[i]) for i,p in enumerate(point)]
		points_return |= set([tuple(new_point)])

	# return
	return points_return



# input: collection of points (npoints x ndims), grid interval (inferred if not given)
# output: collection of points (npoints x ndims)
def complement(points, grid_interval=0):

	return 1



# input: collection of points (npoints x ndims), grid interval (inferred if not given)
# output: collection of points (npoints x ndims)
# Comment: The expected input to this function is a grid. For example, the output of the gridification function 
def thickening(points, grid_interval=0):

	# Get grid interval
	if grid_interval == 0:
		assert False, "Must input grid interval, inferring interval not yet implemented"

	# Get dimensions and prepare vectors
	dim_p = 0
	if type(points) == type([]):
		assert len(points)>0 ; "Empy list passed"
		dim_p = len(points[0])
	elif type(points) == type(np.array([])):
		assert points.shape[0]>0 ; "Empy list passed"
		dim_p = points[0].shape[0]
	dim_vectors = [np.array([grid_interval/2 if i==j else 0 for i in range(dim_p)]) for j in range(dim_p)]

	# Multiply in as many dimensions
	points_return = []
	coeff_dict = {'0':-1, '1':0, '2':1}
	for i in range(3**dim_p):
		coeffs = [coeff_dict[dim_val] for dim_val in np.base_repr(i,base=3).zfill(dim_p)]
		shift = np.sum([coeffs[dim]*dim_vectors[dim] for dim in range(dim_p)],axis=0)
		for p in points:
			points_return.append(np.array(p)+shift)

	# Remove duplicates
	points_return_clean = subs.sparsify_point_set(points=np.array(points_return), min_squared_dist=(grid_interval/10)**2)

	return points_return_clean