// STL includes
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

template <typename T = std::string>
typename T convertType(
    const std::string& arg
) {
    if constexpr (std::is_same_v<typename T, int>)
        return std::stoi(arg);
    else
        return arg;
}

template <typename T>
typename T parseString(
    std::string& line,
    const std::string& delimiter
) {
    typename T parsed_entity = convertType<typename T>(line.substr(0, line.find(delimiter)));
    line.erase(0, line.find(delimiter) + delimiter.length());
    return parsed_entity;
}

bool validate_password(
    const int min_occur,
    const int max_occur,
    const std::string& character,
    const std::string& password
) {
    int occurrences = 0;

    // Iterate through password to count occurrences
    const char* char_ptr = character.c_str();
    for (const char& c : password) {
        if (c == *char_ptr) {
            occurrences++;
        }
    }

    if (occurrences >= min_occur && occurrences <= max_occur) {
        return true;
    }

    return false;
}

int main(int argc, char const *argv[])
{
    // Check if an input file argument was passed in
    if (argc < 2) {
        throw std::invalid_argument("An input file was not passed in!");
    }

    // Used for debugging culprit lines in file
    unsigned int line_count = 1;
    unsigned int valid_passwords = 0;

    try {
        std::cout << "Reading input file now: " << argv[1] << std::endl;
        // Stream input file
        std::ifstream input_file(argv[1]);

        // Walk through file line by line
        std::string line;
        while (std::getline(input_file, line)) {

            std::cout << "Line, Entry: " << line_count << ", " << line << std::endl;

            // Check entry line parses successfully
            // Expect it in the format "L-U C: <PASSWORD>"
            // where:   L is the minimum number of occurrences of C
            //          U is the maximum number of occurrences of C
            //          C is the character that must appear in the password
            //          <PASSWORD> is the password to validate
            std::vector<std::string> delimiters { "-", " ", ": "};
            const auto min_occur = parseString<int>(line, delimiters[0]);
            const auto max_occur = parseString<int>(line, delimiters[1]);
            const auto character = parseString<std::string>(line, delimiters[2]);
            std::cout << "min_occur, max_occur, character, password: " << min_occur << ", " << max_occur << ", " << character << ", " << line << std::endl;

            if (validate_password(min_occur, max_occur, character, line)) {
                valid_passwords++;
            }

            line_count++;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
    }

    std::cout << "Number of valid passwords: " << valid_passwords << std::endl;

    return 0;
}