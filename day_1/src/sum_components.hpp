#include <tuple>
#include <vector>

class sum_components {
    public:
        void insert_permutation(
            std::tuple<unsigned int, unsigned int>&& permutation
        ) {
            sum_components_permutations_.emplace_back(permutation);
        }

        std::vector<std::tuple<unsigned int, unsigned int>> get_permutations(
        ) const {
            return sum_components_permutations_;
        }

    private:
        std::vector<std::tuple<unsigned int, unsigned int>> sum_components_permutations_;
};