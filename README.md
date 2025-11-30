# TopoAware
Topologically aware constructions for ecological hypervolumes 

## About
Topological tools to construct hypervolumes for use in ecology.
The underlying methods of building related and topologically bounded point clouds are not restricted to use in ecology, though the motivation comes from ecology.
The code in this repo is meant to be self-contained and easy to use.
See the [Python notebook](https://github.com/jlazovskis/TopoAware/blob/main/python/example.ipynb) for a visual example of the intended use cases
* [preprint](https://arxiv.org/abs/XXXX.YYYYY): Background on mathematical methods, stability results, Nov 2025
* [poster](https://www.jlazovskis.com/talks/2025-10-08-ires.pdf): Interdisciplinary Research and Education Symposium, University of Aberdeen, Oct 2025

Ideas are borrowed from, and the code relies on:
* [hypervolume](https://github.com/bblonder/hypervolume): The orginal program to generate hypervolumes via kernel density estimators
* [gudhi](https://gudhi.inria.fr): Methods to construct simplicial complexes and sparsify point clouds

## Usage

`TopoAware` may be used by either downloading the individual file(s) in the language intended to be used, or by cloning the full repository, and then using the files. 

## Todo
The code in this repository is actively being developed by [@jlazovskis](https://github.com/jlazovskis).

- Python tasks
  - [x] Python code
  - [x] Python example notebook
  - [ ] Computing and comparing persistence in notebook
- R tasks
  - [ ] R code
  - [ ] R example file
  - [x] Full import of `subsampling` module into `rgudhi` (as [fork of rgudhi](https://github.com/jlazovskis/rgudhi-subsampling))
- C++ tasks
  - [ ] C++ header files
  - [ ] C++ example files
  - [ ] Set up C++ example files to be compiled with cmake