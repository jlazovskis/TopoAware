# hypervolume-t
Ecological hypervolumes using topology

## About
The goal of this project is to use topological tools, in terms of computational efficiency and theoretical guarantees, to construct hypervolumes for use in ecology. Hypervolumes are usually constructed as kernel density estimators, but those are often less interesting topologically at the expense of knowing more information (precisley "filling in" the holes of missing data). This project aims to retain toological information of the input sample while still providing the user with more information about the space in which the sample lies. Ideas are borrowed from:
* [hypervolume](https://github.com/bblonder/hypervolume): The orginal program to generate hypervolumes via kernel density estimators
* [ripser](https://github.com/Ripser): Efficient constructions of simplicial complexes
* [gudhi](https://gudhi.inria.fr) : sparsification methods

## Usage
 First compile the program, using your favorite compiler. Example below with `g++`.

    g++ -o hvt -I include hvt.cpp

Then run the program on a data set, indicating the distance `bdist` at which to add barycenters for 1-simplices and 2-simplices, and the distance `sdist` at which to sparsify by maintaining this minimum distance between any two points. Example with provided data below.

    ./hvt --input examples/test_shape.csv --output examples/test_out.csv --bdist 0.05 --sdist 0.5
