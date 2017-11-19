//
// Created by paul on 15/11/17.
//

#include <algorithm>
#include <array>
#include <random>
#include <vector>

namespace genetics {
/// La taille de la population.
    const int POPULATION_SIZE = 200;

/// Taille du pool d'enfants génerés.
    const int POOL_SIZE = POPULATION_SIZE * 5;

/// Taux de mutation en pourcent. Ex : 40 <=> 40% de mutation.
    const int MUTATION_RATE = 70;

/// si cette variable est vraie, alors on considérera aussi les parents de la génération n pour créer la génération n+1.
    const bool APPEND_PARENTS_TO_CHILDRENS = true;

    using Matrix = std::vector<std::vector<int>>;

/// Une tâche individuelle, avec id sa position dans le tableau.
/// Gène.
    struct Task {
        Task(int id) : _id(id) {};
        int _id;
    };

/// Une solution possible (individu) est une suite de tâches.
/// Individu.
    class Solution {
    public:
        Solution() : _order(), _solution_size() {};

        /// Construis une solution aléatoire.
        Solution(const int solution_size);

        /// Evalue le score d'une solution.
        int score(const Matrix &initial_matrix, const Matrix &other_matrix);

        /// Crées deux enfants à partir de deux parents via la méthode OX1.
        static std::pair<Solution, Solution> enfant(const Solution &sol1, const Solution &sol2);


        /// Mute une solution en permuttant deux gènes.
        void mutate();

        /// Affiche une solution sur la sortie console.
        void afficher_solution() const;

        /// Chromosomes.
        std::vector<Task> _order;

        int _solution_size;
    };


/// Représente une population d'individus.
    class Population {
    public:
        Population() : _solutions() {};

        /// Construis une nouvelle population aléatoire.
        Population(const int solution_size);

        /// Réalise une itération de l'algorithme de génétique.
        void iterate(const Matrix &initial_matrix, const Matrix &other_matrix);

        /// Trie les solutions selon leur score.
        void sort_solution(const Matrix &initial_matrix, const Matrix &other_matrix);

        /// Individus.
        std::array<Solution, POPULATION_SIZE> _solutions;
    };
}