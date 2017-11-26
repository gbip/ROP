//
// Created by paul on 15/11/17.
//

#include "genetics.h"
#include <deque>
#include <iostream>
#include <set>


namespace genetics {

	std::random_device RD;
	std::mt19937 ENG(RD());

	int random_number(int min, int max) {
		std::uniform_int_distribution<> distr(min, max);
		return distr(ENG);
	}

	int Solution::score(const Matrix& intial_cost_matrix, const Matrix& transition_cost_matrix) const {
		int result = 0;
		int first_task = this->_order[0]._id;
		result += intial_cost_matrix[1][first_task];
		int prev = first_task;
		for(int i = 1; i < this->_solution_size; i++) {
			int task = this->_order[i]._id;
			result += transition_cost_matrix[task][prev];
			prev = task;
		}
		return result;
	}

	Solution::Solution(const int solution_size) : _order(), _solution_size(solution_size) {
		std::vector<int> tasks;
		for(int i = 0; i < solution_size; i++) {
			tasks.push_back(i);
		}
		std::random_shuffle(tasks.begin(), tasks.end());
		Solution result;
		for(int task : tasks) {
			this->_order.push_back(Task(task));
		}
	}

	std::pair<Solution, Solution> Solution::enfant(const Solution& sol1, const Solution& sol2) {
		int cp1, cp2;
		{
			int cut_point1 = random_number(0, sol1._solution_size - 1);
			int cut_point2 = random_number(0, sol1._solution_size - 1);
			cp1 = std::min(cut_point1, cut_point2);
			cp2 = std::max(cut_point1, cut_point2);
		}

		// Va contenir les tâches des enfants.
		std::deque<Task> temp_schedule1;
		std::deque<Task> temp_schedule2;
		// On stocke les éléments insérés.
		std::set<int> already_present1;
		std::set<int> already_present2;
		// On ajoute le morceau découpé dans notre solution temporaire.
		for(int i = cp1; i < cp2; i++) {
			auto elem1 = sol1._order[i];
			auto elem2 = sol2._order[i];
			already_present1.emplace(elem1._id);
			temp_schedule1.push_back(elem1);
			already_present2.emplace(elem2._id);
			temp_schedule2.push_back(elem2);
		}

		// On permutte les deux bouts du vecteur de tâches.
		std::vector<Task> sorted_task1;
		std::vector<Task> sorted_task2;

		for(int i = cp2; i < sol2._solution_size; i++) {
			sorted_task1.push_back(sol2._order[i]);
			sorted_task2.push_back(sol1._order[i]);
		}
		for(int i = 0; i < cp2; i++) {
			sorted_task1.push_back(sol2._order[i]);
			sorted_task2.push_back(sol1._order[i]);
		}

		// Insertion des nouveaux éléments à la fin de la nouvelle solution.
		for(int i = cp2; i < sol2._solution_size; i++) {
			for(auto t : sorted_task2) {
				if(already_present2.count(t._id) > 0) {
					continue;
				} else {
					temp_schedule2.push_back(t);
					already_present2.emplace(t._id);
					break;
				}
			}
			for(auto t : sorted_task1) {
				if(already_present1.count(t._id) > 0) {
					continue;
				} else {
					temp_schedule1.push_back(t);
					already_present1.emplace(t._id);
					break;
				}
			}
		}

		// Insertion de nouveaux éléments au début de cette solution.
		for(int i = 0; i < cp1; i++) {
			for(auto t : sorted_task2) {
				if(already_present2.count(t._id) > 0) {
					continue;
				} else {
					temp_schedule2.push_front(t);
					already_present2.emplace(t._id);
					break;
				}
			}
			for(auto t : sorted_task1) {
				if(already_present1.count(t._id) > 0) {
					continue;
				} else {
					temp_schedule1.push_front(t);
					already_present1.emplace(t._id);
					break;
				}
			}
		}


		// Assignation des solutions temporaires aux enfants.
		Solution res1, res2;

		for(auto t : temp_schedule1) {
			res1._order.push_back(t);
		}
		for(auto t : temp_schedule2) {
			res2._order.push_back(t);
		}
		res1._solution_size = sol1._solution_size;
		res2._solution_size = sol2._solution_size;
		return std::pair<Solution, Solution>(res1, res2);
	}

	void Solution::mutate() {
		int first = random_number(0, this->_solution_size - 1);
		int second = random_number(0, this->_solution_size - 1);

		auto first_gene = this->_order[first];
		auto second_gene = this->_order[second];

		this->_order[first] = second_gene;
		this->_order[second] = first_gene;
	}

	void Solution::afficher_solution() const {
		if(_solution_size != _order.size()) {
			std::cout << "Solution malformée !" << std::endl;
			std::cout << "Solution size : " << std::to_string(_solution_size) << " | "
			          << "_order.size() : " << std::to_string(_order.size()) << std::endl;
		}
		std::cout << "[";
		for(int i = 0; i < _solution_size; i++) {
			std::cout << std::to_string(_order[i]._id) << ",";
		}
		std::cout << "]" << std::endl;
	}

	Population::Population(const int solution_size) : _solutions() {
		for(int i = 0; i < POPULATION_SIZE; i++) {
			_solutions[i] = Solution(solution_size);
		}
	}

	void Population::iterate(const Matrix& intial_cost_matrix, const Matrix& transition_cost_matrix) {
		std::vector<Solution> candidats;

		// Création d'un pool d'enfants.
		for(int i = 0; i < POOL_SIZE; i++) {
			int first = random_number(0, POPULATION_SIZE - 1);
			int second = random_number(0, POPULATION_SIZE - 1);
			auto children = Solution::enfant(this->_solutions[first], this->_solutions[second]);
			candidats.push_back(children.first);
			candidats.push_back(children.second);
		}

		// Mutation des candidats.
		for(auto& sol : candidats) {
			int i = random_number(0, 100);
			if(i < MUTATION_RATE) {
				sol.mutate();
			}
		}

		// On rajoute les parents aux candidats.
		if(APPEND_PARENTS_TO_CHILDRENS) {
			for(auto p : this->_solutions) {
				candidats.push_back(p);
			}
		}


		// On trie le tableau de solution de la meilleur à la moins bonne.
		std::sort(candidats.begin(), candidats.end(), [intial_cost_matrix, transition_cost_matrix](Solution& a, Solution& b) -> bool {
			return a.score(intial_cost_matrix, transition_cost_matrix) < b.score(intial_cost_matrix, transition_cost_matrix);
		});

		// Création de la nouvelle population pour la génération suivante.
		for(int i = 0; i < POPULATION_SIZE; i++) {
			this->_solutions[i] = candidats[i];
		}
	}

	void Population::sort_solution(const Matrix& intial_cost_matrix, const Matrix& transition_cost_matrix) {
		std::cout << std::endl;
		std::sort(this->_solutions.begin(),
		          this->_solutions.end(),
		          [intial_cost_matrix, transition_cost_matrix](Solution& a, Solution& b) -> bool {
			          return a.score(intial_cost_matrix, transition_cost_matrix) > b.score(intial_cost_matrix, transition_cost_matrix);
			      });
	}
}