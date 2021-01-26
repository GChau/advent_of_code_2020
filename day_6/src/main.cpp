// STL includes
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

static void process_line(
    const std::string& line,
    const bool skip_part_2,
    std::set<char>& yes_answers,
    std::set<char>& yes_answers_common
) {
    std::set<char> yes_answers_common_temp;
    const bool new_group = yes_answers_common.empty();

    for (const char& c : line) {
        // Part 1
        yes_answers.insert(c);

        // If a new group, we use the first person's answer as the baseline for common answers
        if (new_group && !skip_part_2) {
            yes_answers_common.insert(c);
        }
        else {
            yes_answers_common_temp.insert(c);
        }
    }

    // Part 2
    if (!new_group) {
        std::set<char> yes_answer_intersection;
        std::set_intersection(
            yes_answers_common.begin(), yes_answers_common.end(),
            yes_answers_common_temp.begin(), yes_answers_common_temp.end(),
            std::inserter(yes_answer_intersection, yes_answer_intersection.begin())
        );
        yes_answers_common = std::move(yes_answer_intersection);
    }
}

int main(int argc, char const *argv[])
{
    auto t1 = std::chrono::high_resolution_clock::now();
    // Check if an input file argument was passed in
    if (argc < 2) {
        throw std::invalid_argument("An input file was not passed in!");
    }

    size_t yes_count_p1 = 0;
    size_t yes_count_p2 = 0;

    try {
        std::cout << "Reading input file now: " << argv[1] << std::endl;

        // Stream input file
        std::ifstream input_file(argv[1]);

        // Part 1
        std::set<char> yes_answers;

        // Part 2
        // yes_answers_common will represent the running intersection of common answers
        // The idea being that each adjacent answer should share a common answer with the previous answer
        // If there is a break (i.e. adjacent answers share nothing in common), there are no common answers
        std::set<char> yes_answers_common;
        bool skip_part_2 = false;

        std::string line;
        while (std::getline(input_file, line)) {
            // For Linux compatiblity
            if (line.length() != 0 && line.at(line.length() - 1) == '\r') {
                line.erase(line.length() - 1);
            }

            // Check for empty line, if empty, next group
            if (line.length() == 0 || line.front() == '\n') {
                yes_count_p1 += yes_answers.size();
                yes_count_p2 += yes_answers_common.size();

                yes_answers.clear();
                yes_answers_common.clear();
                skip_part_2 = false;
                continue;
            }

            process_line(line, skip_part_2, yes_answers, yes_answers_common);
            skip_part_2 = yes_answers_common.empty();
        }

        // Final process
        process_line(line, skip_part_2, yes_answers, yes_answers_common);

        yes_count_p1 += yes_answers.size();
        yes_count_p2 += yes_answers_common.size();
    }
    catch (const std::exception& e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Argh!" << std::endl;
    }

    std::cout << "Part 1: Yes Count: " << yes_count_p1 << std::endl;
    std::cout << "Part 2: Yes Count: " << yes_count_p2 << std::endl;

    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Duration: " << duration << " microseconds" << std::endl;

    return 0;
}