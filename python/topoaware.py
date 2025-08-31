# packages
import numpy as np
import pandas as pd
import gudhi as gd
from matplotlib import pyplot as plt

# functions

# input: SimplexTree class, vertex coordinates (nrows=npoints, ncols=ndims)
# output: collection of points, imput complex points + all barycenters of input complex simplices
def barycenters(simplex_tree, points):
	points_return = []
	for simplex in simplex_tree.get_simplices():
		points_return.append(np.mean([points[s] for s in simplex[0]],axis=0))
	return np.array(points_return)
