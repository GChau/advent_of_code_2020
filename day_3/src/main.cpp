// STL includes
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

const char TREE = '#';
const char OPEN_SPACE = '.';

int main(int argc, char const *argv[])
{
    // Check if an input file argument was passed in
    if (argc < 2) {
        throw std::invalid_argument("An input file was not passed in!");
    }

    size_t number_of_trees_encountered_part_1 = 0;
    std::vector<size_t> number_of_trees_encountered_part_2(5, 0);

    try {
        std::cout << "Reading input file now: " << argv[1] << std::endl;

        // Stream input file
        std::ifstream input_file(argv[1]);

        // Let's get the number of lines
        std::string line;
        unsigned int line_count = 0;

        // Part 1 components
        unsigned int part_1_index = 0;
      
        // Part 2 components
        std::vector<unsigned int> part_2_index(5, 0);
        const std::vector<unsigned int> x_slope_offset{ 1, 3, 5, 7, 1 };
        const std::vector<unsigned int> y_slope_mod{ 1, 1, 1, 1, 2 };

        // Read first line
        std::getline(input_file, line);
        part_1_index += 3;
        line_count++;
        std::cout << " line.length(): " << line.length() << std::endl;

        for (size_t i = 0; i < part_2_index.size(); i++) {
            part_2_index[i] += x_slope_offset[i];
        }

        while (std::getline(input_file, line)) {
            // For Linux compatiblity
            if (line.at(line.length() - 1) == '\r') {
                line.erase(line.length() - 1);
            }

            // Part 1
            if (line.at(part_1_index) == TREE) {
                number_of_trees_encountered_part_1++;
            }
            part_1_index += 3;
            part_1_index %= line.length();

            // Part 2
            for (size_t i = 0; i < number_of_trees_encountered_part_2.size(); i++) {
                if (line_count % y_slope_mod[i] == 0) {

                    if (line.at(part_2_index[i]) == TREE) {
                        number_of_trees_encountered_part_2[i]++;
                    }
                    part_2_index[i] += x_slope_offset[i];
                    part_2_index[i] %= line.length();
                }
            }

            line_count++;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
    }


    std::cout << "Part 1: Number of trees encountered: " << number_of_trees_encountered_part_1 << std::endl;

    size_t product = 1;
    for (size_t i = 0; i < number_of_trees_encountered_part_2.size(); i++) {
        std::cout << "Part 2: Number of trees encountered, slope " << i << ": " << number_of_trees_encountered_part_2[i] << std::endl;
        product *= number_of_trees_encountered_part_2[i];
    }
    std::cout << "Part 2: Number of trees encountered product: " << product << std::endl;

    return 0;
}