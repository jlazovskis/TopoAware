namespace hvt {
	
	class simplicial_complex {
		private:
			// The list of points
			std::vector<std::vector<float>> points;

			// A list of neighbors for each point
			std::vector<std::vector<hvt::index>> neighbors;
			
			// The number of edges
			hvt::index num_edges;

	    public:
	    	// Constructor
	    	simplicial_complex() {};

	    	// Load point cloud
	    	// Taken from https://github.com/Ripser
	    	bool load_point_cloud( const std::string filename, const float threshold ) {
				
				// Check to make sure file can be read
    			std::ifstream dummy( filename.c_str() );
    			if( dummy.fail() ) {
					std::cerr << "couldn't open file " << filename << std::endl;
					return false;
    			}

    			// Read in the data
                this->points = points;
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
					if (!point.empty()) points.push_back(point);
					//assert(point.size() == points.front().size());
				}
			
    			return true;

	    	};


            // Print for testing
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

	};
}