// STL includes
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

// Custom includes
#include "sum_components.hpp"

const int SUM = 2020;

bool calculate_match(
    std::set<unsigned int>& seen_expense,
    std::unordered_map<unsigned int, sum_components>& expenses_atlas,
    const unsigned int expense
) {
    // Check if compliment exists
    const unsigned int compliment = SUM - expense;

    auto comp_it = expenses_atlas.find(compliment);
    if (comp_it != expenses_atlas.end()) {
        // If compliment exists, check if sum component permutation exists
        if (comp_it->second.get_permutations().size()) {
            auto sum_component = comp_it->second.get_permutations().back();
            std::cout << "Triple: " << expense << ", " << std::get<0>(sum_component) << ", " << std::get<1>(sum_component) << " (" << compliment << ")" << std::endl;
            std::cout << "Multiplied: " << expense * std::get<0>(sum_component) * std::get<1>(sum_component) << std::endl;
            return true;
        }
    }

    for (auto it = seen_expense.cbegin(); it != seen_expense.cend(); it++) {
        const unsigned int new_permutation = expense + *it;
        if (new_permutation < 2020) {
            if (expenses_atlas.find(new_permutation) == expenses_atlas.cend()) {
                expenses_atlas[new_permutation] = sum_components();
            }
            std::cout << "New permutation: " << new_permutation << ": " << expense << " + " << *it << std::endl;
            expenses_atlas[new_permutation].insert_permutation(std::make_tuple(expense, *it));
        }
    }

    // Insert entry into the set
    if (expenses_atlas.find(expense) == expenses_atlas.cend()) {
        expenses_atlas[expense] = sum_components();
    }

    seen_expense.insert(expense);

    return false;
}

int main(int argc, char const *argv[])
{
    if (argc < 1) {
        throw std::invalid_argument("An input file was not passed in!");
    }

    // Keep track of our expenses seen here and init to 0
    std::set <unsigned int> seen_expenses;
    std::unordered_map<unsigned int, sum_components> expenses_atlas;
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

            if (calculate_match(seen_expenses, expenses_atlas, entry)) {
                return 0;
            }

            line_count++;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
    }

    std::cout << "Did not find a sum triple that totalled to " << SUM << "!" << std::endl;

    return 0;
}