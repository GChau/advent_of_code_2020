// STL includes
#include <fstream>
#include <iostream>
#include <set>
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

    size_t valid_passports = 0;

    std::vector<std::string> mandatory_fields{
        "byr",
        "iyr",
        "eyr",
        "hgt",
        "hcl",
        "ecl",
        "pid"
    };

    std::vector<std::string> optional_fields{
        "cid"
    };

    std::set<std::string> matching_fields;

    try {
        std::cout << "Reading input file now: " << argv[1] << std::endl;

        // Stream input file
        std::ifstream input_file(argv[1]);

        std::string line;
        while (std::getline(input_file, line)) {
            // For Linux compatiblity
            if (line.length() != 0 && line.at(line.length() - 1) == '\r') {
                line.erase(line.length() - 1);
            }

            if (line.length() == 0 || line.front() == '\n') {
                if (matching_fields.size() == 7) {
                    valid_passports++;
                }
                matching_fields.clear();
                continue;
            }


            // Part 1
            for (auto& mandatory_field : mandatory_fields) {
                if (line.find(mandatory_field) != std::string::npos) {
                    matching_fields.insert(mandatory_field);
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Argh!" << std::endl;
    }

    // Final check
    if (matching_fields.size() == 7) {
        valid_passports++;
    }

    std::cout << "Part 1: Number of valid passports: " << valid_passports << std::endl;

    return 0;
}