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
    const int NEIGHBOROOD_SIZE = 1000;

    ///La taille de la mémoire de l'algorithme.
    const unsigned long MEMORY_SIZE = 10*NEIGHBOROOD_SIZE;

    /// Le nombre d'itérations sans améliorations qui entrainent la fin de l'algorithme
    const int ITERATIONS_BEFORE_STOP = 15;

    /// La vitesse de croissance du voisinnage quand on stagne.
    const float GROWING_RATE = 0.5;

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
        std::pair<std::vector<Solution>,std::vector<Solution>> generate_neighbors(const int neighborood_size,
                                                                                     std::deque<Solution> tabou);
        /// Les données sur la solution.
        std::vector<int> _solution;
        int _solution_size;
    };

    class Tabu {
    public :
        Tabu();

        /// Initialise une recherche avec tabou
        Tabu(int solution_size);

        void do_iterations(const Matrix &intial_cost_matrix, const Matrix &transition_cost_matrix);

        bool verify_stop_conidition();

        Solution _best;
        Solution _current;


        /// Le score de la solution sélectionnée pour cette itération.
        int _current_score;
        int _best_score;
        int _iterations_without_ameliorations;
        int _neighborood_size;

        /// La liste de solutions interdites.
        std::deque<Solution> _forbidden;
    };
}


#endif //ROP_TABU_H
