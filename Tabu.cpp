//
// Created by paul on 19/11/17.
//

#include <algorithm>
#include "Tabu.h"

namespace tabu {


    std::random_device RD;
    std::mt19937 ENG(RD());

    int random_number(int min, int max) {
        std::uniform_int_distribution<> distr(min, max);
        return distr(ENG);
    }

    int factorial(int n)
    {
        return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
    }

    Solution::Solution(const int solution_size) : _solution(), _solution_size(solution_size) {
        std::vector<int> tasks;
        for(int i = 0; i < solution_size; i++) {
            tasks.push_back(i);
        }
        std::random_shuffle(tasks.begin(), tasks.end());
        Solution result;
        for(int task : tasks) {
            this->_solution.push_back(task);
        }
    }

    int Solution::score(const Matrix& intial_cost_matrix, const Matrix& transition_cost_matrix) const{
        int result = 0;
        int first_task = this->_solution[0];
        result += intial_cost_matrix[1][first_task];
        int prev = first_task;
        for(int i = 1; i < this->_solution_size; i++) {
            int task = this->_solution[i];
            result += transition_cost_matrix[task][prev];
            prev = task;
        }
        return result;
    }

    Solution Solution::two_opt(int pos1, int pos2) {
        auto copie = _solution;
        copie[pos1] = _solution[pos2];
        copie[pos2] = _solution[pos1];
        return Solution(copie,_solution_size);
    }

    std::pair<std::vector<Solution>,std::vector<Permutation>> Solution::generate_neighbors(const int neighborood_size,std::deque<Permutation> tabou) {
        std::vector<Solution> neighborood;
        std::vector<Permutation> permutations;
        for (int i =0; i <neighborood_size;i++) {
            int pos1 = random_number(0,_solution_size - 1);
            int pos2 = random_number(0,_solution_size - 1);
            Permutation permutation(std::min(pos1,pos2),std::max(pos1,pos2));
            bool forbiden_permutation = std::find(tabou.begin(), tabou.end(), permutation) != tabou.end();
            if (!forbiden_permutation) {
                Solution variant = this->two_opt(pos1, pos2);
                neighborood.push_back(variant);
                permutations.push_back(permutation);
            }
            else {
                i--;
            }
        }
        std::pair<std::vector<Solution>,std::vector<Permutation>> result(neighborood,permutations);
        return result;
    }


    Tabu::Tabu() : _best(),_current(), _current_score(0), _best_score(0),_forbidden() {

    }

    Tabu::Tabu(int solution_size): _best(solution_size), _current(_best), _current_score(0), _best_score(0),_forbidden() {}

    void Tabu::do_iteration(const Matrix& intial_cost_matrix, const Matrix& transition_cost_matrix) {
        _current_score = _current.score(intial_cost_matrix,transition_cost_matrix);
        _best_score = _best.score(intial_cost_matrix,transition_cost_matrix);
        auto neighbors = _current.generate_neighbors(NEIGHBOROOD_SIZE,_forbidden);

        // Rajout des tabous.
        for (auto p : neighbors.second) {
            _forbidden.push_front(p);
        }
        _forbidden.resize(MEMORY_SIZE);

        // Recherche d'une nouvelle meilleur solution.
        for (auto n : neighbors.first) {
            int score = n.score(intial_cost_matrix,transition_cost_matrix);
            if (score>_best_score) {
                _best=n;
                _best_score=score;
            }
            else if ( score > _current_score) {
                _current=n;
                _current_score=score;
            }
        }
    }
}

