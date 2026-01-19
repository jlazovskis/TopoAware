// This is an example file for TopoAware 
// Author: Jānis Lazovskis, 2026

#include <fstream>
#include <topoaware/topoaware.h>

int main() {

	// Prepare example data
	std::vector< topoaware::point > generated_data;
	std::ifstream input_stream( "../../examples/generated_data.csv" );
    std::string cur_line;

    // Read example data
    int line_counter=0;
    while( getline( input_stream, cur_line ) ) {
    	topoaware::point cur_point;
        if( line_counter > 0 && cur_line != "" && cur_line[ 0 ] != '#' ) {
            std::stringstream ss( cur_line );
        	std::string cur_val;
        	std::getline( ss, cur_val, ',');
	        cur_point.push_back( std::stod(cur_val) );
		generated_data.push_back(cur_point);
    	}
    	line_counter++;
    }

    // Initiate point cloud class
    topoaware::point_cloud data;

    return 1;
}