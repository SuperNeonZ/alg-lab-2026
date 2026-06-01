#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

using namespace std;

int main() {
    int n;
    cout << "Количество городов: ";
    cin >> n;

    cin.ignore();

    string filename;
    cout << "Имя файла (Enter для input.txt): ";
    getline(cin, filename);

    if (filename.empty()) {
        filename = "input.txt";
    }

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    
    uniform_int_distribution<int> weight_dist(1, 50);
    uniform_real_distribution<double> edge_prob(0.0, 1.0);

    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось создать файл " << filename << endl;
        return 1;
    }

    file << n << "\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                file << 0;
            } else if (edge_prob(gen) > 0.2) {
                file << weight_dist(gen);
            } else {
                file << 0;
            }
            if (j < n - 1) file << " ";
        }
        file << "\n";
    }

    file.close();
    cout << "Матрица сохранена в: " << filename << endl;
    return 0;
}