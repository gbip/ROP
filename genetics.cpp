//
// Created by paul on 15/11/17.
//

#include "genetics.h"

int Solution::score(const Matrix &initial_matrix, const Matrix &other_matrix) const {
    int result = 0;
    int first_task = this->order[0].id;
    result += initial_matrix[1][first_task];

    int prev = 0;
    for (int i = 1; i < (int) this->order.size(); i++) {
        int task = this->order[i].id;
        result += other_matrix[prev][task];
        prev = task;
    }
    return result;
}

Solution::Solution(const Matrix &mat) : order() {
    // Cast de long en int <=> ok ici (car on a des matrices petites).
    int solution_size = (int) (mat.size());

    std::vector<int> tasks;
    for (int i = 0; i < solution_size; i++) {
        tasks.push_back(i);
    }
    std::random_shuffle(tasks.begin(), tasks.end());
    Solution result;
    for (int task : tasks) {
        this->order.push_back(Task(task));
    }
}

std::pair<Solution, Solution> Solution::enfant(const Solution &sol1, const Solution &sol2) {
    Solution res1,res2;
    usize cut_point = std::rand() %  sol1.order.size();

    for (usize i =0; i < cut_point; i++) {
        res1.order.push_back(sol1.order[i]);
        res2.order.push_back(sol2.order[i]);
    }

    for (std::vector<int>::size_type i = cut_point; i < sol1.order.size(); i++) {
        res1.order.push_back(sol2.order[i]);
        res2.order.push_back(sol1.order[i]);
    }
    return std::pair<Solution,Solution>(res1,res2);
}

void Solution::mutate() {
    int first = std::rand() % POPULATION_SIZE;
    int second = std::rand() % POPULATION_SIZE;

    auto first_gene = this->order[first];
    auto second_gene = this->order[second];

    this->order[first] = second_gene;
    this->order[second] = first_gene;

}

Population::Population(const Matrix &mat) :solutions() {
    for (int i =0; i <POPULATION_SIZE; i++) {
        solutions[i] = Solution(mat);
    }
}

void Population::iterate(const Matrix &initial_matrix, const Matrix &other_matrix) {
    std::vector<Solution> candidats;

    for (int i = 0; i < 100; i++) {
        int first = std::rand() % POPULATION_SIZE;
        int second = std::rand() % POPULATION_SIZE;
        auto children = Solution::enfant(this->solutions[first], this->solutions[second]);
        candidats.push_back(children.first);
        candidats.push_back(children.second);
    }

    for (auto &sol : candidats) {
        int i = std::rand() % 100;
        if (i < 10) {
            sol.mutate();
        }
    }

    // On trie le tableau de solution de la meilleur à la moins bonne.
    std::sort(candidats.begin(), candidats.end(),
              [initial_matrix, other_matrix](const Solution &a, const Solution &b) -> bool {
                  return a.score(initial_matrix, other_matrix) > b.score(initial_matrix, other_matrix);
              });

    for (usize i = 0; i < POPULATION_SIZE; i++) {
        this->solutions[i] = candidats[i];
    }
}
