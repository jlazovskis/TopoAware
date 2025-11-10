# TopoAware
Topologically aware constructions for ecological hypervolumes
* [preprint](https://arxiv.org/abs/XXXX.YYYYY): Background on mathematical methods, 
* [poster](https://www.jlazovskis.com/talks/2025-10-08-ires.pdf): From the "Interdisciplinary Research and Education Symposium" at the University of Aberdeen, October 2025 

## About
The purpose of this software is to use topological tools to construct hypervolumes for use in ecology.
The underlying methods of building related and topologically bounded point clouds are not restricted to use in ecology.
Ideas are borrowed from:
* [hypervolume](https://github.com/bblonder/hypervolume): The orginal program to generate hypervolumes via kernel density estimators
* [gudhi](https://gudhi.inria.fr): Methods to construct simplicial complexes and sparsify point clouds

## Usage

`TopoAware` may be used by either downloading the individual file(s) in the language intended to be used, or by cloning the full repository, and then using the files. 

## Changelog
The code in this repository is actively being developed by @jlazovskis.

- Python tasks
  - [x] Python code
  - [x] Python example notebook
- R tasks
  - [ ] R code
  - [ ] R example file
  - [ ] Full import of `subsampling` module into `rgudhi` (currently relies on [fork of rgudhi](https://github.com/jlazovskis/rgudhi-subsampling))
- C++ tasks
  - [ ] C++ header files
  - [ ] C++ example files
  - [ ] Set up C++ example files to be compiled with cmake