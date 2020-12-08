#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

const int SUM = 2020;

int main(int argc, char const *argv[])
{
    if (argc < 1) {
        throw std::invalid_argument("An input file was not passed in!");
    }

    // Keep track of our entries seen here and init to false
    std::vector<bool> entries_seen(SUM, false);
    unsigned int line_count = 1;

    try {
        std::cout << "Reading input file now: " << argv[1] << std::endl;
        // Stream input file
        std::ifstream input_file(argv[1]);

        // Walk through file line by line
        std::string line;
        int entry;
        while (std::getline(input_file, line)) {
            std::istringstream iss(line);

            // Check entry parses to int successfully
            if (!(iss >> entry)) {
                std::cerr << "Failed to parse entry as integer on line " << line_count << std::endl;
                throw std::runtime_error("Failed to parse entry as integer");
            }

            std::cout << "Line, Entry: " << line_count << ", " << entry << std::endl;
            if (entry <= 0 || entry > SUM + 1) {
                std::cerr << "Entry " << entry << " is outside of range on line " << line_count << std::endl;
                continue;
            }

            // Check entries
            entries_seen[entry - 1] = true;
            const int compliment = SUM - entry;

            // If the compliment is in the vector, return true
            // -1  for offset
            if (entries_seen[compliment - 1]) {
                std::cout << "Pair: " << entry << ", " << compliment << std::endl;
                std::cout << "Multiplied: " << entry * compliment << std::endl;
                return 0;
            }

            line_count++;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
    }

    std::cout << "Did not find a sum pair that totalled to " << SUM << "!" << std::endl;

    return 0;
}