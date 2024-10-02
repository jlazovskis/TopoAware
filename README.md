# hypervolume-t
Ecological hypervolumes using topology

## About
The goal of this project is to use topological tools, in terms of computational efficiency and theoretical guarantees, to construct hypervolumes for use in ecology. Hypervolumes are usually constructed as kernel density estimators, but those are often less interesting topologically at the expense of knowing more information (precisley "filling in" the holes of missing data). This project aims to retain toological information of the input sample while still providing the user with more information about the space in which the sample lies.

![Example of how the program hypervolume-t works, comparing three states of points](examples/2d_visual.png "Input, subdivision, sparsification")

Ideas are borrowed from:
* [hypervolume](https://github.com/bblonder/hypervolume): The orginal program to generate hypervolumes via kernel density estimators
* [ripser](https://github.com/Ripser): Efficient constructions of simplicial complexes
* [gudhi](https://gudhi.inria.fr) : Sparsification methods

## Usage

`hypervolume-t` can be used directly with the provided releases, or can be compiled from the source code.

### Relases

TODO

### Compiling

`hypervolume-t` depends on other C++ libraries, which need to be set up before compiling:
* [boost](https://www.boost.org) : C++ Libraries
* [Eigen](https://eigen.tuxfamily.org) : Template library for linear algebra
* [CGAL](https://www.cgal.org) : Coputational Geometry Algorithms Library
* [gudhi](https://gudhi.inria.fr) : Sparsification methods

Oly once these libraries are present on you machine will it be possible to compile `hypervolume-t`, both for C++ and R.

#### Compiling in C++

Compile `hvt`, for example with `g++`.

    g++ -o hvt -I include hvt.cpp

Then run the program on a data set, indicating the distance `bdist` at which to add barycenters for 1-simplices and 2-simplices, and the distance `sdist` at which to sparsify by maintaining this minimum distance between any two points. Example with provided data below.

    ./hvt --input examples/test_shape.csv --output examples/test_out.csv --bdist 0.5 --sdist 0.05

In general, `sdist < bdist`, as having the sparsification distance larger than the barycentric subdivision distance voids the need for barycentric subdivision. That is, just sparsifying the input data set would yield the same result as first subdividing, then sparsifying.

#### Compiling in R

The `Rcpp` library is necessary to compile from source code in R. 

    > library('Rcpp')
    > sourceCpp('hvt-r.cpp')
    > df <- read.csv('examples/2d_input.csv')
    > df2 <- hypervolume_t(data=df, dist_barycenter=0.6, dist_sparsify=0.01)

# TODO

* Move examples to new subsection, align variable names