#include <array>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include "Tabou.h"
#include "genetics.h"

// Décommenter pour activer le mode "debug"
//#define DEBUG

/// Le nombre d'itérations pour l'algorithme de génétique.
const int GEN_ITERATIONS = 500;

using Matrix = std::vector<std::vector<int>>;

/// Affiche une matrice.
void print_matrix(const Matrix& matrix) {
	for(auto line : matrix) {
		std::cout << "[";
		for(auto i : line) {
			std::cout << std::to_string(i) << " ";
		}
		std::cout << "]" << std::endl;
	}
}

/// Vérifie si un nom de fichier est valide.
bool verify_file(const std::string& filename) {
	std::ifstream stream(filename, std::ios::in);
	if(!stream.good()) {
		std::cout << "Erreur : veuillez spécifier un fichier valide." << std::endl;
		return false;
	}
	return true;
}

/// Récupères le nom du fichier à utiliser
std::string get_filename() {
#ifndef DEBUG
	std::cout << "Entrer le chemin du fichier à utiliser:" << std::endl;
	std::string result;
	std::getline(std::cin, result);
	return result;
#else
	std::string result("../PROB401.TXT");
	return result;
#endif
}

/// Retourne le stream représentant un fichier.
std::ifstream get_file_stream(std::string filename) {
	if(!verify_file(filename)) {
		throw std::runtime_error("Fichier invalide");
	} else {
		return std::ifstream(filename, std::ios::in);
	}
}

/// Lis un fichier en mémoire depuis le chemin donné en argument.
std::vector<std::string> get_file_lines(const std::string filename) {
	std::vector<std::string> result;
	auto stream = get_file_stream(filename);
	for(std::string line; getline(stream, line);) {
		result.push_back(line);
	}
	return result;
}

/// Sépares une chaîne de caractères par des blocs d'espace en un vecteur d'entier.
std::vector<std::string> split(std::string const& input) {
	std::istringstream buffer(input);
	std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), std::istream_iterator<std::string>());
	return ret;
}

/// Concatènes les lignes d'une chaîne de caractère en une seule chaîne.
std::string concat(const std::vector<std::string>& lines) {
	std::string result;

	for(auto str : lines) {
		result = result + std::string(" ") + str;
	}
	return result;
}

/// Convertie une chaîne de caractères en un tableau d'entier.
std::vector<int> string_to_int(const std::string& str) {
	auto splitted_str = split(str);
	std::vector<int> result;
	for(auto c : splitted_str) {
		result.push_back(std::stoi(c));
	}
	return result;
}

/// Renvoies la taille des données
int get_matrix_size(std::vector<std::string>& data) {
	return std::stoi(split(data[0])[0]);
}

/// Renvoies la matrice des coûts de transition et la matrice des temps initiaux.
/// La première matrice est celle des coûtes de transition (la grosse).
/// La seconde matrice est celle des coûts/temps initiaux pour chaques tâches (le tableau 2x2).
std::pair<Matrix, Matrix> get_working_matrix(std::vector<std::string> file) {
	int size = get_matrix_size(file);

	Matrix initial_costs;
	Matrix mat;
	std::vector<int> data = string_to_int(concat(file));
	data.erase(data.begin());

	// Le +2 est là car on a 2 tableau.
	for(int i = 0; i < size + 2; i++) {
		std::vector<int> line;
		for(int k = 0; k < size; k++) {
			line.push_back(data[k + i * size]);
		}
		if((int)line.size() < size) {
			throw std::runtime_error("Matrice malformée");
		} else {
			if(i < 2) {
				initial_costs.push_back(line);
			} else {
				mat.push_back(line);
			}
		}
	}

	return std::pair<Matrix, Matrix>(mat, initial_costs);
}

/// Applique l'algorithme de génétique à notre problème.
void do_genetics(std::pair<Matrix, Matrix> matrix, int size) {
	genetics::Population genetics(size);
	for(int i = 0; i < GEN_ITERATIONS; i++) {
		genetics.iterate(matrix.second, matrix.first);
	}

	genetics.sort_solution(matrix.second, matrix.first);

	std::cout << "Solution - GENETIQUE : " << std::endl;
	for(auto i : genetics._solutions[0]._order) {
		std::cout << std::to_string(i._id) << " | ";
	}
	std::cout << std::endl;
	std::cout << "Pour un score de : " << std::to_string(genetics._solutions[0].score(matrix.second, matrix.first)) << std::endl;
}

/// Applique l'algorithme de la recherche avec tabou à notre problème.
void do_tabu(std::pair<Matrix, Matrix> matrix, int size) {
	tabu::Tabou tabu(size);
	tabu.do_iterations(matrix.second, matrix.first);
	std::cout << "Solution - TABOU : " << std::endl;
	for(auto i : tabu._best._solution) {
		std::cout << std::to_string(i) << "|";
	}
	std::cout << std::endl << "Pour un score de : " << std::to_string(tabu._best_score) << std::endl;
}

int main() {
	std::srand(std::time(0));
	std::string filename = get_filename();
	auto file = get_file_lines(filename);
	const auto matrix = get_working_matrix(file);
	int c_max = 0;
	for(auto t : matrix.second[0]) {
		c_max += t;
	}

	std::cout << "CMAX : " << std::to_string(c_max) << std::endl;

	// On fais 10 itérations de l'algo de génétique et on garde le meilleur résultat.
	for(int k = 0; k < 10; k++) {
		do_genetics(matrix, get_matrix_size(file));
	}
	do_tabu(matrix, get_matrix_size(file));
	return 0;
}