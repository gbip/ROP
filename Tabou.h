//
// Created by paul on 19/11/17.
//

#ifndef ROP_TABU_H
#define ROP_TABU_H

#include <queue>
#include <vector>

namespace tabu {

	using Matrix = std::vector<std::vector<int>>;
	using Permutation = std::pair<int, int>;

	/// La taille du voisinnage.
	const int NEIGHBOROOD_SIZE = 25;

	/// La taille de la mémoire de l'algorithme.
	const unsigned long MEMORY_SIZE = 100 * NEIGHBOROOD_SIZE;

	/// Le nombre d'itérations sans améliorations qui entrainent la fin de l'algorithme
	const int ITERATIONS_BEFORE_STOP = 75;

	/// La vitesse de croissance du voisinnage quand on stagne.
	const float GROWING_RATE = 0.07;

	/// Le nombre de réinitialisations
	const int MAX_REINITIALIZATION = 25;

	/// Représente une solution au problème.
	class Solution {
	public:
		Solution() : _solution({}), _solution_size(0){};

		/// Constructeur par copie.
		Solution(std::vector<int> sol, int size) : _solution(sol), _solution_size(size){};

		/// Crée une solution aléatoire.
		Solution(const int solution_size);

		/// Evalue le score d'une solution.
		int score(const Matrix& intial_cost_matrix, const Matrix& transition_cost_matrix) const;

		/// Permute deux éléments d'une solution et renvoie la solution résultante (2-opt)
		Solution two_opt(int pos1, int pos2);

		/// Génére les voisins d'une solution. Renvoies une liste de voisins.
		std::vector<Solution> generate_neighbors(const int neighborood_size, std::deque<Solution> tabou);
		/// Les données sur la solution.
		std::vector<int> _solution;
		int _solution_size;
	};

	/// Le coeur de l'agorithme de RT.
	class Tabou {
	public:
		Tabou();

		/// Initialise une recherche avec tabou
		Tabou(int solution_size);

		/// Itères jusqu'à ce que verify_stop_condition renvoies vrai.
		void do_iterations(const Matrix& intial_cost_matrix, const Matrix& transition_cost_matrix);

		/// Renvoie vrai quand l'algorithme doit s'arrêter.
		bool verify_stop_condition();

		/// La meilleur solution.
		Solution _best;

		/// La solution courante.
		Solution _current;

		/// Le score de la solution sélectionnée pour cette itération.
		int _current_score;

		/// Le score de la meilleure solution.
		int _best_score;

		/// Le nombre d'itération sans améliorations.
		int _iterations_without_ameliorations;

		/// La taille du voisinnage.
		int _neighborood_size;

		/// Indique si l'algorithme a été réinitialisé.
		int _reinitializations;

		/// La taille des solutions.
		int _solution_size;

		/// La liste de solutions interdites.
		std::deque<Solution> _forbidden;
	};
}


#endif // ROP_TABU_H
