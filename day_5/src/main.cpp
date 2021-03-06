// STL includes
#include <assert.h>  
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

int main(int argc, char const *argv[])
{
    auto t1 = std::chrono::high_resolution_clock::now();
    // Check if an input file argument was passed in
    if (argc < 2) {
        throw std::invalid_argument("An input file was not passed in!");
    }

    std::vector<unsigned int> seat_ids;
    size_t highest_id = 0;

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
            std::pair<unsigned int, unsigned int> row_range(0, 127);
            for (int i = 0; i < 7; i++) {
                const unsigned int split = ((row_range.second - row_range.first) / 2) + row_range.first;
                if (line.at(i) == 'F') {
                    row_range.second = split;
                }
                else if (line.at(i) == 'B') {
                    row_range.first = split + 1;
                }
                else {
                    throw std::runtime_error("Unexpected character during row binary partition!");
                }
            }

            // Check row ranges match
            assert(row_range.first == row_range.second);

            std::pair<unsigned int, unsigned int> col_range(0, 7);
            for (int i = 7; i < 10; i++) {
                const unsigned int split = ((col_range.second - col_range.first) / 2) + col_range.first;
                if (line.at(i) == 'L') {
                    col_range.second = split;
                }
                else if (line.at(i) == 'R') {
                    col_range.first = split + 1;
                }
                else {
                    throw std::runtime_error("Unexpected character during col binary partition!");
                }
            }

            // Check col ranges match
            assert(col_range.first == col_range.second);

            const unsigned int seat_id = row_range.first * 8 + col_range.first;
            seat_ids.push_back(seat_id);
            highest_id = seat_id > highest_id ? seat_id : highest_id;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Argh!" << std::endl;
    }

    std::cout << "Part 1: Highest Seat ID: " << highest_id << std::endl;

    std::sort(seat_ids.begin(), seat_ids.end());
    for (int i = 0; i < seat_ids.size() - 2; i++) {
        if ((seat_ids[i] + 1) != seat_ids[i + 1]) {
            std::cout << "Part 2: My Seat ID: " << (seat_ids[i] + 1) << std::endl;
            break;
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Duration: " << duration << " microseconds" << std::endl;

    return 0;
}