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

/// Ce type est présent pour éviter les caste de std::vector<int>::size_type en int.
using usize = std::vector<int>::size_type;

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
    Solution(const Matrix& mat);

    /// Evalue le score d'une solution.
    int score(const Matrix& initial_matrix, const Matrix& other_matrix) const;

    /// Crées deux enfants à partir de deux parents via la méthode du 2-pop, en choisissant un point de coupure au hasard.
    static std::pair<Solution,Solution> enfant(const Solution& sol1, const Solution& sol2);;

    /// Mute une solution en permuttant deux gènes.
    void mutate();
    /// Chromosomes.
    std::vector<Task> order;
};


/// Représente une population d'individus.
class Population {
public :
    Population() : solutions() {};

    /// Construis une nouvelle population aléatoire.
    Population(const Matrix& mat);

    /// Réalise une itération de l'algorithme de génétique.
    void iterate(const Matrix& initial_matrix, const Matrix& other_matrix);

    /// Individus.
    std::array<Solution,50> solutions;
};
