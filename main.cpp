#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <iterator>

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
    std::cout << "Entrer le chemin du fichier à utiliser:" << std::endl;
    std::string result;
    std::getline(std::cin,result);
    return result;
}

/// Lis un fichier en mémoire depuis le chemin donné en argument.
std::vector<std::string> get_file_lines(const std::string filename) {
    std::vector<std::string> result;
    std::ifstream stream(filename, std::ios::in);
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
std::pair<Matrix,Matrix> get_working_matrix() {
    /// TODO : FIXME
    const std::string filename("../PROB401.TXT");
    if (!verify_file(filename)) {
        throw std::runtime_error("Fichier invalide");
    }
    auto file = get_file_lines(filename);
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
        if (line.size() < task_numbers) {
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
        if (line.size() < task_numbers) {
            throw std::runtime_error("Matrice malformée.");
        }
        result.push_back(line);
    }

    return std::pair<Matrix,Matrix>(result,other);
}


int main() {
    print_matrix(get_working_matrix().second);
    return 0;
}