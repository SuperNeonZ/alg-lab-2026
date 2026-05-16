#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> compute_pi(const string& P) {
    int m = P.length();
    vector<int> pi(m, 0);
    for (int i = 1; i < m; i++) {
        int j = pi[i - 1];
        while (j > 0 && P[i] != P[j])
            j = pi[j - 1];
        if (P[i] == P[j])
            j++;
        pi[i] = j;
    }
    return pi;
}

int main() {
    string A, B;
    cout << "Введите строку A: ";
    cin >> A;
    cout << "Введите строку B: ";
    cin >> B;

    if (A.length() != B.length()) {
        cout << "\nДлины строк не совпадают. Циклический сдвиг невозможен.\n";
        cout << "\nРезультат: " << -1;
        return 0;
    }
    if (A.empty()) {
        cout << "\nРезультат: " << 0;
        return 0;
    }

    string S = A + A;
    int n = S.length();
    int m = B.length();

    vector<int> pi = compute_pi(B);

    cout << "\nЗначения префикс-функции для \"" << B << "\": ";
    for (size_t k = 0; k < pi.size(); ++k) {
        cout << pi[k];
        if (k != pi.size() - 1) cout << " ";
    }
    cout << "\n\n";

    int i = 0;
    int j = 0;

    while (i < n) {
        if (S[i] == B[j]) {
            cout << "S[" << i << "] = " << S[i] << " и B[" << j << "] = " << B[j] << " совпали\n";
            i++;
            j++;
            cout << "Идем вперед по префикс-функции, новый j: " << j << "; i = " << i << "\n";

            if (j == m) {
                int shift_idx = i - j;
                cout << "Найдена полная подстрока \"" << B << "\" в S на индексе " << shift_idx << "\n";
                cout << "\nРезультат: " << shift_idx << "\n";
                return 0;
            }
        } else {
            cout << "S[" << i << "] = " << S[i] << " и B[" << j << "] = " << B[j] << " не совпали\n";
            if (j > 0) {
                j = pi[j - 1];
                cout << "Идем назад по префикс-функции, новый j: " << j << "\n";
            } else {
                i++;
                cout << "Увеличиваем i = " << i << "\n";
            }
        }
    }

    cout << "\nРезультат: " << -1;
    return 0;
}