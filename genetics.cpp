//
// Created by paul on 15/11/17.
//

#include "genetics.h"
#include <iostream>

std::random_device RD;
std::mt19937 ENG(RD());

int random_number(int min, int max) {
    //std::cout << std::to_string(min) << " | " << std::to_string(max) << std::endl;
    std::uniform_int_distribution<> distr(min, max);
    return distr(ENG);
}

int Solution::score(const Matrix &initial_matrix, const Matrix &other_matrix) const {
    int result = 0;
    int first_task = this->_order[0]._id;
    result += initial_matrix[1][first_task];

    int prev = 0;
    for (int i = 1; i < (int) this->_order.size(); i++) {
        int task = this->_order[i]._id;
        result += other_matrix[prev][task];
        prev = task;
    }
    return result;
}

Solution::Solution(const int solution_size) : _order(), _solution_size(solution_size) {
    std::vector<int> tasks;
    for (int i = 0; i < solution_size; i++) {
        tasks.push_back(i);
    }
    std::random_shuffle(tasks.begin(), tasks.end());
    Solution result;
    for (int task : tasks) {
        this->_order.push_back(Task(task));
    }
}

std::pair<Solution, Solution> Solution::enfant(const Solution &sol1, const Solution &sol2) {
    Solution res1,res2;
    int cut_point = random_number(0,sol1._solution_size -1);

    for (int i =0; i < (int) cut_point; i++) {
        res1._order.push_back(sol1._order[i]);
        res2._order.push_back(sol2._order[i]);
    }

    for (int i = cut_point; i < (int64_t) sol1._order.size(); i++) {
        res1._order.push_back(sol2._order[i]);
        res2._order.push_back(sol1._order[i]);
    }
    res1._solution_size = sol1._solution_size;
    res2._solution_size = sol2._solution_size;
    return std::pair<Solution,Solution>(res1,res2);
}

void Solution::mutate() {
    int first = random_number(0, this->_solution_size-1);
    int second = random_number(0, this->_solution_size-1);

    auto first_gene = this->_order[first];
    auto second_gene = this->_order[second];

    this->_order[first] = second_gene;
    this->_order[second] = first_gene;

}

Population::Population(const int solution_size) : _solutions() {
    for (int i =0; i <POPULATION_SIZE; i++) {
        _solutions[i] = Solution(solution_size);
    }
}

void Population::iterate(const Matrix &initial_matrix, const Matrix &other_matrix) {
    std::vector<Solution> candidats;

    for (int i = 0; i < POOL_SIZE; i++) {
        int first = random_number(0,POPULATION_SIZE - 1);
        int second = random_number(0,POPULATION_SIZE - 1);
        auto children = Solution::enfant(this->_solutions[first], this->_solutions[second]);
        candidats.push_back(children.first);
        candidats.push_back(children.second);
    }

    for (auto &sol : candidats) {
        int i = random_number(0,100);
        if (i < 10) {
            sol.mutate();
        }
    }

    // On trie le tableau de solution de la meilleur à la moins bonne.
    std::sort(candidats.begin(), candidats.end(),
              [initial_matrix, other_matrix](const Solution &a, const Solution &b) -> bool {
                  return a.score(initial_matrix, other_matrix) > b.score(initial_matrix, other_matrix);
              });

    for (int i = 0; i < POPULATION_SIZE; i++) {
        this->_solutions[i] = candidats[i];
    }
}

void Population::sort_solution(const Matrix &initial_matrix, const Matrix &other_matrix) {
    std::sort(this->_solutions.begin(), this->_solutions.end(),
              [initial_matrix, other_matrix](const Solution &a, const Solution &b) -> bool {
                  return a.score(initial_matrix, other_matrix) > b.score(initial_matrix, other_matrix);
              });
}
