// This is an example file for TopoAware 
// Author: Jānis Lazovskis, 2026

// Example compilation: g++ -o example -I ../include/ -I ../../../gudhi.3.11.0/include/ read_and_construct.cpp

#include <fstream>
#include <topoaware/topoaware.h>

int main() {

	// Prepare example data
	std::vector< topoaware::point > generated_data;
	std::ifstream input_stream( "../../examples/generated_data.csv" );
    std::string cur_line;
    int line_counter = 0;

    // Skip header
    std::getline(input_stream, cur_line);
    while (std::getline(input_stream, cur_line) && line_counter < 10)
    {
        // skip empty lines
        if (cur_line.empty()) continue;

        std::stringstream ss(cur_line);
        std::string cur_value;
        topoaware::point cur_point;

        while (std::getline(ss, cur_value, ',')) {        
            cur_point.push_back(std::stod(cur_value));
        }
        generated_data.push_back(cur_point);
        line_counter++;
    }
    input_stream.close();

    // Initiate point cloud class
    topoaware::point_cloud data;
    data.set_points(generated_data);
    data.print_points();

    std::cout << "------------" << std::endl;

    // Barycentric subdivision
    // data.barycentric_subdivision( 0.2, 2 );
    // data.print_points();

    // Sparsification
    // data.sparsification( 0.2 );
    // data.print_points();

    // Gridification
    data.gridification(1.1, {0.1,0.2});
    data.print_points();

    return 1;
}