// This is an example file for TopoAware 
// Author: Jānis Lazovskis, 2026

// Example compilation: g++ -o example -I ../include/ -I ../../../gudhi.3.11.0/include/ example.cpp

#include <fstream>
#include <topoaware/topoaware.h>

int main() {

	// Prepare example data
	std::vector< topoaware::point > generated_data;
	std::ifstream input_stream( "../../examples/generated_data.csv" );
    std::string cur_line;

    // Skip header
    std::getline(input_stream, cur_line);
    while (std::getline(input_stream, cur_line)) {

        // skip empty lines
        if (cur_line.empty()) continue;

        std::stringstream ss(cur_line);
        std::string cur_value;
        topoaware::point cur_point;

        while (std::getline(ss, cur_value, ',')) {        
            cur_point.push_back(std::stod(cur_value));
        }
        generated_data.push_back(cur_point);
    }
    input_stream.close();

    // Initiate point cloud class
    std::cout << "Initiating point cloud class" << std::endl;
    topoaware::point_cloud data;
    data.set_points(generated_data);

    // Barycentric subdivision
    std::cout << "Constructing barycentric subdivison... ";
    data.barycentric_subdivision( 0.2, 2 );
    data.export_points("1_barycentric_subdivision.csv");
    std::cout << "exported to 1_barycentric_subdivision.csv\n";

    // Sparsification
    std::cout << "Constructing sparsification... ";
    data.sparsification( 0.2 );
    data.export_points("2_sparsification.csv");
    std::cout << "exported to 2_sparsification.csv\n";

    // Gridification
    std::cout << "Constructing gridification... ";
    data.gridification(1.1, {0.1,0.2});
    data.export_points("3_gridification.csv");
    std::cout << "exported to 3_gridification.csv\n";

    // Copy of point cloud
    topoaware::point_cloud data_grid;
    std::vector<topoaware::point> points_copy;
    data.get_points(points_copy);
    data_grid.set_points(points_copy);
    
    // Complement
    std::cout << "Constructing complement of gridification... ";
    data.complement(1.1, 2);
    data.export_points("4a_complement.csv");
    std::cout << "exported to 4_complement.csv\n";

    // Thickening
    std::cout << "Constructing thickening of gridification... ";
    data_grid.thickening(.55);
    data_grid.export_points("4b_thickening.csv");
    std::cout << "exported to 4b_thickening.csv\n";

    // Exit message
    std::cout << "All done, exiting" << std::endl;
    return 1;
}