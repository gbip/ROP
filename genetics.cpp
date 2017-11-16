//
// Created by paul on 15/11/17.
//

#include <vector>
#include <random>
#include <algorithm>
#include <array>

/// La taille de la population.
const int POPULATION_SIZE = 50;

using Matrix = std::vector<std::vector<int>>;

/// Une tâche individuelle, avec id sa position dans le tableau.
/// Gène.
struct Task {
    Task(int id) : id(id){};
    int id;
};

/// Une solution possible (individu) est une suite de tâches.
/// Individu.
class Solution {
public :
    Solution() : order() {};

    /// Construis une solution aléatoire.
    Solution(const Matrix& mat) : order() {
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

    /// Evalue le score d'une solution.
    int score(const Matrix& initial_matrix, const Matrix& other_matrix) const{
        int result = 0;
        int first_task = this->order[0].id;
        result += initial_matrix[1][first_task];

        int prev = 0;
        for (int i = 1; i < this->order.size(); i++) {
            int task = this->order[i].id;
            result += other_matrix[prev][task];
            prev = task;
        }
        return result;
    }


    /// Chromosomes.
    std::vector<Task> order;
};


/// Représente une population d'individus.
class Population {
public :
    Population() : solutions() {};

    /// Construis une nouvelle population aléatoire.
    Population(const Matrix& mat) :solutions() {
        for (int i =0; i <POPULATION_SIZE; i++) {
            solutions[i] = Solution(mat);
        }
    }
    /// Individus.
    std::array<Solution,50> solutions;
};

