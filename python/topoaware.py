# Packages for computing
import numpy as np
import gudhi as gd
from gudhi import subsampling as subs


# input: GUDHI SimplexTree class, vertex coordinates (nrows=npoints, ncols=ndims)
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

	# construct vr complex
	complex_vr_container = gd.RipsComplex(points=points, max_edge_length=radius)
	complex_vr = complex_vr_container.create_simplex_tree(max_dimension=max_dim)

	# get barycenters and return
	return barycenters(complex_vr, points)



# input: collection of points (npoints x ndims), distance at which to sparsify
# output: collection of points (npoints x ndims)
def sparsification(points, min_dist):

	# sparsify
	points_return = subs.sparsify_point_set(points=points, min_squared_dist=(min_dist)**2)

	# return numpy array
	return np.array(points_return)



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

	# return numpy array
	return np.array(list(points_return))



# input: collection of points (npoints x ndims), grid interval (inferred if not given)
# output: collection of points (npoints x ndims)
def complement(points, grid_interval=0, buffer=2):

	# Get grid interval
	if grid_interval == 0:
		assert False, "Must input grid interval, inferring interval not yet implemented"

	# Get dimensions and extremal values
	dim_p = 0
	max_vals = []
	min_vals = []
	if type(points) == type([]):
		assert len(points)>0 ; "Empy list passed"
		dim_p = len(points[0])
		max_vals = [np.max(np.array(points).T[dim]) for dim in range(dim_p)]
		min_vals = [np.min(np.array(points).T[dim]) for dim in range(dim_p)]
	elif type(points) == type(np.array([])):
		assert points.shape[0]>0 ; "Empy list passed"
		dim_p = points[0].shape[0]
		max_vals = [np.max(points.T[dim]) for dim in range(dim_p)]
		min_vals = [np.min(points.T[dim]) for dim in range(dim_p)]
	ranges = [np.rint((max_vals[dim]-min_vals[dim])/grid_interval).astype(int)+1+2*buffer for dim in range(dim_p)]

	# Iterate over input
	mask_array = np.ones(tuple(ranges),dtype=bool)
	for p in points:
		loc = [np.rint((p[dim]-min_vals[dim])/grid_interval).astype(int)+buffer for dim in range(dim_p)]
		mask_array[tuple(loc)] = False

	# Construct output
	points_return = []
	min_vals = np.array([min_vals[dim]-grid_interval*buffer for dim in range(dim_p)])
	for loc in zip(*np.nonzero(mask_array)):
		points_return.append(np.array(loc)*grid_interval + min_vals)

	# Return as numpy array
	return np.array(points_return)



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

	# Return as numpy array
	return np.array(points_return_clean)