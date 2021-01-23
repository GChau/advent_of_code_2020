// STL includes
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

static bool validate_contents(
    const std::string& field,
    const std::string& content_str
) {
    bool valid_contents = false;
    if (field == "byr") {
        auto contents = std::stoi(content_str);
        valid_contents = (contents >= 1920 && contents <= 2002);
    }
    else if (field == "iyr") {
        auto contents = std::stoi(content_str);
        valid_contents = (contents >= 2010 && contents <= 2020);
    }
    else if (field == "eyr") {
        auto contents = std::stoi(content_str);
        valid_contents = (contents >= 2020 && contents <= 2030);
    }
    else if (field == "hgt") {
        const std::string units = content_str.substr(content_str.length() - 2, 2);
        auto length = std::stoi(content_str.substr(0, content_str.length() - 2));
        if (units == "cm") {
            valid_contents = (length >= 150 && length <= 193);
        }
        else if (units == "in") {
            valid_contents = (length >= 59 && length <= 76);
        }
    }
    else if (field == "hcl" || field == "ecl" || field == "pid") {
        valid_contents = true;
    }

    return valid_contents;
}

static void validate_passport(
    const std::unordered_map<std::string, std::string>& passport_components,
    std::unordered_map<std::string, std::regex>& mandatory_fields,
    size_t& valid_passports_p1,
    size_t& valid_passports_p2
) {
    std::set<std::string> matching_fields_p1;
    size_t valid_fields_p2 = 0;

    for (auto it = passport_components.cbegin(); it != passport_components.cend(); it++) {
        // Check if there are matching fields for part 1

        if (mandatory_fields.find(it->first) != mandatory_fields.end()) {
            // Match in field, add 1 to Part 1
            const std::string field = it->first;
            const std::string content_str = it->second;
            matching_fields_p1.insert(field);

            // Valid contents regex first
            std::smatch regex_result;
            std::regex_match(content_str, regex_result, mandatory_fields[field]);
            if (regex_result.size() > 0) {
                if (validate_contents(field, content_str)) {
                    valid_fields_p2++;
                }
            }
        }
    }

    if (matching_fields_p1.size() == 7) {
        valid_passports_p1++;
    }

    if (valid_fields_p2 == 7) {
        valid_passports_p2++;
    }
}

int main(int argc, char const *argv[])
{
    // Check if an input file argument was passed in
    if (argc < 2) {
        throw std::invalid_argument("An input file was not passed in!");
    }

    size_t valid_passports_p1 = 0;
    size_t valid_passports_p2 = 0;

    std::unordered_map<std::string, std::regex> mandatory_fields;
    mandatory_fields["byr"] = std::regex("\\d{4}");
    mandatory_fields["iyr"] = std::regex("\\d{4}");
    mandatory_fields["eyr"] = std::regex("\\d{4}");
    mandatory_fields["hgt"] = std::regex("\\d{2}(?:\\d{1}cm|in)");
    mandatory_fields["hcl"] = std::regex("#[0-9a-f]{6}");
    mandatory_fields["ecl"] = std::regex("(amb|blu|brn|gry|grn|hzl|oth)");
    mandatory_fields["pid"] = std::regex("[0-9]{9}");

    std::unordered_map<std::string, std::string> passport_components;

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

            // Check for empty line, if empty, next passport
            if (line.length() == 0 || line.front() == '\n') {

                validate_passport(
                    passport_components,
                    mandatory_fields,
                    valid_passports_p1,
                    valid_passports_p2
                );

                passport_components.clear();
                continue;
            }

            // Split line - part 2
            size_t pos = 0;
            const std::string delimiter = " ";
            const std::string component_delimiter = ":";
            while ((pos = line.find(delimiter)) != std::string::npos) {
                const std::string token = line.substr(0, pos);
                passport_components[token.substr(0, 3)] = token.substr(4, token.length() - 4);
                line.erase(0, pos + delimiter.length());
            }
            if (!line.empty()) {
                passport_components[line.substr(0, 3)] = line.substr(4, line.length() - 4);
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
    validate_passport(
        passport_components,
        mandatory_fields,
        valid_passports_p1,
        valid_passports_p2
    );

    std::cout << "Part 1: Number of valid passports: " << valid_passports_p1 << std::endl;
    std::cout << "Part 2: Number of valid passports: " << valid_passports_p2 << std::endl;

    return 0;
}