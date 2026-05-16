#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string p, t;
    cout << "Введите строку p: ";
    cin >> p;
    cout << "Введите строку t: ";
    cin >> t;

    int n = t.length();
    int m = p.length();

    vector<int> pi(m, 0);
    for (int i = 1; i < m; i++) {
        int j = pi[i - 1];
        while (j > 0 && p[i] != p[j])
            j = pi[j - 1];
        if (p[i] == p[j])
            j++;
        pi[i] = j;
    }

    cout << "\nЗначения префикс-функции для \"" << p << "\": ";
    for (size_t k = 0; k < pi.size(); ++k) {
        cout << pi[k];
        if (k != pi.size() - 1) cout << " ";
    }
    cout << "\n\n";

    vector<int> positions;
    int i = 0;
    int j = 0;

    while (i < n) {
        if (t[i] == p[j]) {
            cout << "t[" << i << "] = " << t[i] << " и p[" << j << "] = " << p[j] << " совпали\n";
            i++;
            j++;
            cout << "Идем вперед по префикс-функции, новый j: " << j << "; i = " << i << "\n";

            if (j == m) {
                cout << "Подстрока " << p << " найдена на индексе " << (i - j) << "\n";
                positions.push_back(i - j);
                j = pi[j - 1];
                cout << "(Успех) Идем назад по префикс-функции, новый j: " << j << "\n";
            }
        } else {
            cout << "t[" << i << "] = " << t[i] << " и p[" << j << "] = " << p[j] << " не совпали\n";
            if (j > 0) {
                j = pi[j - 1];
                cout << "Идем назад по префикс-функции, новый j: " << j << "\n";
            } else {
                i++;
                cout << "Следующий i = " << i << "\n";
            }
        }
    }

    cout << "\nРезультат: ";
    if (positions.empty()) {
        cout << "-1";
    } else {
        for (size_t k = 0; k < positions.size(); k++) {
            if (k > 0) cout << ",";
            cout << positions[k];
        }
    }
    cout << "\n";

    return 0;
}