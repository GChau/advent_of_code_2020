// STL includes
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

template <typename T = std::string>
T convertType(
    const std::string& arg
) {
    if constexpr (std::is_same_v<T, int>)
        return std::stoi(arg);
    else
        return arg;
}

template <typename T>
T parseString(
    std::string& line,
    const std::string& delimiter
) {
    T parsed_entity = convertType<T>(line.substr(0, line.find(delimiter)));
    line.erase(0, line.find(delimiter) + delimiter.length());
    return parsed_entity;
}

// Expect it in the format "L-U C: <PASSWORD>"
// where:   L is the minimum number of occurrences of C
//          U is the maximum number of occurrences of C
//          C is the character that must appear in the password
//          <PASSWORD> is the password to validate
bool validate_password_part_1(
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

// Expect it in the format "X-Y C: <PASSWORD>"
// where:   X is the first index in the password to check
//          Y is the second index in the password to check
//          C is the character that must appear in the password
//          <PASSWORD> is the password to validate
bool validate_password_part_2(
    const int index_1,
    const int index_2,
    const std::string& character,
    const std::string& password
) {
    if (index_1 > password.length() || index_2 > password.length()) {
        throw std::invalid_argument("Bogus line! Indices are out of password bounds!");
    }

    const char* char_ptr = character.c_str();

    // If only one matches (XOR), return true.
    if ((*char_ptr == password[index_1 - 1]) ^ (*char_ptr == password[index_2 - 1])) {
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
    unsigned int valid_passwords_part_1 = 0;
    unsigned int valid_passwords_part_2 = 0;

    try {
        std::cout << "Reading input file now: " << argv[1] << std::endl;
        // Stream input file
        std::ifstream input_file(argv[1]);

        // Walk through file line by line
        std::string line;
        while (std::getline(input_file, line)) {

            std::cout << "Line, Entry: " << line_count << ", " << line << std::endl;

            // Check entry line parses successfully
            // Expect it in the format "X-Y C: <PASSWORD>"
            // where:   X is an int
            //          Y is an int
            //          C is the character that must appear in the password
            //          <PASSWORD> is the password to validate
            std::vector<std::string> delimiters { "-", " ", ": "};
            const auto x = parseString<int>(line, delimiters[0]);
            const auto y = parseString<int>(line, delimiters[1]);
            const auto character = parseString<std::string>(line, delimiters[2]);
            std::cout << "x, y, character, password: " << x << ", " << y << ", " << character << ", " << line << std::endl;

            if (validate_password_part_1(x, y, character, line)) {
                valid_passwords_part_1++;
            }
            if (validate_password_part_2(x, y, character, line)) {
                valid_passwords_part_2++;
            }

            line_count++;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
    }

    std::cout << "Part 1: Number of valid passwords: " << valid_passwords_part_1 << std::endl;
    std::cout << "Part 2: Number of valid passwords: " << valid_passwords_part_2 << std::endl;

    return 0;
}