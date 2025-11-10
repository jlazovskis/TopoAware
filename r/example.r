# Install packaes (as necessary)
install.packages("devtools")
devtools::install_github("LMJL-Alea/rgudhi")

# Package for computing
library(rgudhi)

# Packages and functions for TopoAware
source("topoaware.r")

# Load test data
data_raw <- read.csv("../examples/generated_data.csv")

# Generate barycentric subdivision
bs = barycentric_subdivision(points=as.matrix(data_raw), radius=.1, max_dim=2)