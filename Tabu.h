//
// Created by paul on 19/11/17.
//

#ifndef ROP_TABU_H
#define ROP_TABU_H

#include <vector>
#include <queue>

namespace tabu {
    using Matrix = std::vector<std::vector<int>>;
    using Permutation = std::pair<int,int>;

    /// La taille du voisinnage.
    const int NEIGHBOROOD_SIZE = 50000;


    ///La taille de la mémoire de l'algorithme.
    const int MEMORY_SIZE = 5*NEIGHBOROOD_SIZE;


    class Solution {
    public :
        Solution() : _solution({}),_solution_size(0) {};

        /// Constructeur par copie.
        Solution(std::vector<int> sol, int size): _solution(sol), _solution_size(size) {};

        /// Crée une solution aléatoire.
        Solution(const int solution_size);

        /// Evalue le score d'une solution.
        int score(const Matrix& intial_cost_matrix, const Matrix& transition_cost_matrix) const;

        /// Permute deux éléments d'une solution et renvoie la solution résultante (2-opt)
        Solution two_opt(int pos1, int pos2);

        /// Génére les voisins d'une solution
        std::pair<std::vector<Solution>,std::vector<Permutation>> generate_neighbors(const int neighborood_size,std::deque<Permutation> tabou);

        std::vector<int> _solution;
        int _solution_size;
    };

    class Tabu {
    public :
        Tabu();

        /// Initialise une recherche avec tabou
        Tabu(int solution_size);

        void do_iteration(const Matrix& intial_cost_matrix, const Matrix& transition_cost_matrix);

        Solution _best;
        Solution _current;

        int _current_score;
        int _best_score;
        std::deque<Permutation> _forbidden;
    };
}


#endif //ROP_TABU_H
