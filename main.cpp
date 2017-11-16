#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <iterator>
#include <ctime>

#include "genetics.h"

#define DEBUG

using Matrix = std::vector<std::vector<int>>;

/// Affiche une matrice.
void print_matrix(const Matrix &matrix) {
    for (auto line : matrix) {
        std::cout << "[";
        for (auto i : line) {
            std::cout << std::to_string(i) <<" ";
        }
        std::cout << "]" <<std::endl;
    }
}

/// Vérifie si un nom de fichier est valide.
bool verify_file(const std::string &filename) {
    std::ifstream stream(filename, std::ios::in);
    if (!stream.good()) {
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
    std::getline(std::cin,result);
    return result;
#else
    std::string result("../PROB401.TXT");
    return result;
#endif
}

/// Retourne le stream représentant un fichier.
std::ifstream get_file_stream(std::string filename) {
    if (!verify_file(filename)) {
        throw std::runtime_error("Fichier invalide");
    }
    else {
        return std::ifstream(filename, std::ios::in);
    }
}

/// Lis un fichier en mémoire depuis le chemin donné en argument.
std::vector<std::string> get_file_lines(const std::string filename) {
    std::vector<std::string> result;
    auto stream = get_file_stream(filename);
    for (std::string line; getline(stream, line);) {
        result.push_back(line);
    }
    return result;
}

/// Sépares une chaîne de caractères par des blocs d'espace en un vecteur d'entier.
std::vector<std::string> split(std::string const &input) {
    std::istringstream buffer(input);
    std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)),
                                 std::istream_iterator<std::string>());
    return ret;
}

/// Renvoies la taille des données
int get_matrix_size(std::vector<std::string>& data) {
    return std::stoi(split(data[0])[0]);
}

/// Renvoies la matrice des coûts de transition et la matrice des temps initiaux.
/// La première matrice est celle des coûtes de transition (la grosse).
/// La seconde matrice est celle des coûts/temps initiaux pour chaques tâches (le tableau 2x2).
std::pair<Matrix,Matrix> get_working_matrix(std::vector<std::string> file) {
    /// TODO : FIXME
    auto task_numbers = get_matrix_size(file);

    Matrix other;
    // Parsing des coûts initiaux et du temps initial.
    for (int i =1; i <3; i++) {
        std::vector<int> line;
        auto l = file[i];
        auto integers = split(l);
        for (std::string str : integers) {
            line.push_back(std::stoi(str));
        }
        if ((int) line.size() < task_numbers) {
            throw std::runtime_error("Matrice malformée.");
        }
        other.push_back(line);
    }

    Matrix result;
    file.erase(file.begin(),file.begin()+3);

    // Parsing de la matrice principale (coût de changement de tâche).
    for (auto l : file) {
        auto integers = split(l);
        std::vector<int> line;
        for (std::string str : integers) {
            line.push_back(std::stoi(str));
        }
        if ((int) line.size() < task_numbers) {
            throw std::runtime_error("Matrice malformée.");
        }
        result.push_back(line);
    }

    return std::pair<Matrix,Matrix>(result,other);
}


int main() {
    std::srand(std::time(0));
    std::string filename = get_filename();
    auto file = get_file_lines(filename);
    const auto matrix = get_working_matrix(file);

    Population genetics(get_matrix_size(file));

    for (int i =0; i < 10000; i++) {
        genetics.iterate(matrix.second, matrix.first);
    }

    genetics.sort_solution(matrix.second,matrix.first);

    std::cout << "Solution : " << std::endl;
    for (auto i : genetics._solutions[0]._order) {
        std::cout << std::to_string(i._id) << " | ";
    }
    std::cout << std::endl;
    std::cout << "Pour un score de : " << std::to_string(genetics._solutions[0].score(matrix.second,matrix.first));


    return 0;
}