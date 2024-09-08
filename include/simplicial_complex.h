namespace hvt {

   	// Taken from https://github.com/Ripser
	class euclidean_distance_matrix {
		private:
			// The points
			std::vector<std::vector<float>> points;
	
		public:
			// The matrix
			euclidean_distance_matrix() {};
	
			euclidean_distance_matrix(std::vector<std::vector<float>>&& _points)
	    		: points(std::move(_points)) {
				for (auto p : points) { assert(p.size() == points.front().size()); }
			}
		
			// The method to access elements of the matrix
			float operator()(const hvt::index i, const hvt::index j) const {
				assert(i < points.size());
				assert(j < points.size());
				return std::sqrt(
					std::inner_product(
		    			points[i].begin(),
		    			points[i].end(),
		    			points[j].begin(),
		    			float(),
		    			std::plus<float>(),
		    			[](float u, float v) {
		    				return (u - v) * (u - v);
		    			}
		    		)
		    	);
	
		  	}
	
        	// For testing
        	void print_me() {
            	this->points = points;
            	for ( int col = 0; col < points.size(); col++ ) {
                	for (const float& value : points[col]) { 
                    	std::cout << value << ",";
                	}
            	std::cout << "\n";
            	}
        	std::cout << std::endl;
        	};
		
			// Number of points
			size_t size() const { return points.size(); }
	};
	
	class simplicial_complex {
		private:
			// A temporary list of points
			std::vector<std::vector<float>> temp_points;

			// A list of neighbors for each point
			std::vector<std::vector<hvt::index>> neighbors;
			
			// The number of edges
			hvt::index num_edges;
		
	    public:
			// A distance matrix
			hvt::euclidean_distance_matrix mat;

	    	// Constructor
	    	simplicial_complex() {};

	    	// Load point cloud
	    	bool load_point_cloud( const std::string filename ) {
				
				// Check to make sure file can be read
		    	// Taken from https://bitbucket.org/phat-code/phat/
    			std::ifstream dummy( filename.c_str() );
    			if( dummy.fail() ) {
					std::cerr << "couldn't open file " << filename << std::endl;
					return false;
    			}

    			// Read in the data
		    	// Taken from https://github.com/Ripser
                this->temp_points = temp_points;
				std::string line;
                std::ifstream input_stream( filename.c_str() );

				float value;
				while (std::getline(input_stream, line)) {
					std::vector<float> point;
					std::istringstream s(line);
					while (s >> value) {
						point.push_back(value);
						s.ignore();
					}
					if (!point.empty()) temp_points.push_back(point);
				}
    			return true;
	    	};

	    	// Compute neighbors
	    	// Taken from https://github.com/Ripser
	    	bool get_neighbors( const float threshold ) {
                this->mat = mat;
                this->temp_points = temp_points;
                mat = hvt::euclidean_distance_matrix(std::move(temp_points));

                return true;
	    	};



            // // For testing
            // void print_me() {
            // 	this->mat = mat;
            //     this->mat.points = points;
            //     for ( int col = 0; col < points.size(); col++ ) {
            //         for (const float& value : points[col]) { 
            //             std::cout << value << ",";
            //         }
            //     std::cout << "\n";
            //     }
            // std::cout << std::endl;
            // };

	};
}