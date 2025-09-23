# Packages for computing
import numpy as np
import pandas as pd
import gudhi as gd
from gudhi import subsampling as subs

# Packages for visualization, timing
from matplotlib import pyplot as plt
from timeit import default_timer

# Functions

# input: SimplexTree class, vertex coordinates (nrows=npoints, ncols=ndims)
# output: collection of points, imput complex points + all barycenters of input complex simplices
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


# input: collection of points, grid size interval, grid origin
# output: collection of points, each at one of the grid points
def grid_points(points, grid_interval, grid_origin=[0]):

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
	points_return = {}
	dim = points[0].shape[0]
	for point in points:
		points_return |= {tuple([(p-grid_origin[i])//grid_interval + grid_origin[i] for i,p in enumerate(point)])}

	# return
	return points_return