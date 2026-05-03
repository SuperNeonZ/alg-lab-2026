#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

const int INF = 1000000000;

string val(int x) {
    return (x >= INF) ? "inf" : to_string(x);
}

int main() {
    string s, t;
    cout << "Введите первую строку: ";
    cin >> s;
    cout << "Введите вторую строку: ";
    cin >> t;

    int n = s.length();
    int m = t.length();

    cout << "Введите номера проклятых элементов через пробел: ";
    vector<bool> is_cursed(n, false);
    string line;
    if (getline(cin >> ws, line)) {
        istringstream iss(line);
        int idx;
        while (iss >> idx) {
            if (idx >= 1 && idx <= n) {
                is_cursed[idx - 1] = true;
            }
        }
    }

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, INF));
    dp[0][0] = 0;

    for (int i = 1; i <= n; ++i) {
        if (!is_cursed[i-1]) {
            if (dp[i-1][0] != INF) dp[i][0] = dp[i-1][0] + 1;
        } else {
            dp[i][0] = INF; // Заблокировано
        }
        cout << "dp[" << i << "][0] = " << (dp[i][0] == INF ? "inf" : to_string(dp[i][0])) << endl;
    }

    for (int j = 1; j <= m; ++j) {
        dp[0][j] = j;
        cout << "dp[0][" << j << "] = " << dp[0][j] << endl;
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            cout << "\nСимволы: " << s[i-1] << " из первой строки, " 
                 << t[j-1] << " из второй строки" << endl;

            if (is_cursed[i-1]) {
                cout << "Элемент " << s[i-1] << " проклят" << endl;
            }

            int insert_cost = INF, delete_cost = INF, match_replace_cost = INF;

            if (dp[i][j-1] != INF) {   // Insert
                insert_cost = dp[i][j-1] + 1;
                cout << "Стоимость вставки: " << insert_cost << endl;
                dp[i][j] = min(dp[i][j], dp[i][j-1] + 1);
            } else {
                cout << "Стоимость вставки: inf" << endl;
            }

            if (!is_cursed[i-1]) {   // Delete
                if (dp[i-1][j] != INF) {
                    delete_cost = dp[i-1][j] + 1;
                    cout << "Стоимость удаления: " << delete_cost << endl;
                    dp[i][j] = min(dp[i][j], dp[i-1][j] + 1);
                } else {
                    cout << "Стоимость удаления: inf" << endl;
                }
            } else {
                cout << "Стоимость удаления: inf (проклятый символ)" << endl;
            }

            if (s[i-1] == t[j-1]) {   // Match
                if (dp[i-1][j-1] != INF) {
                    match_replace_cost = dp[i-1][j-1];
                    cout << "Символы совпали, стоимость совпадения: " << match_replace_cost << endl;
                    dp[i][j] = min(dp[i][j], dp[i-1][j-1]);
                } else {
                    cout << "Символы совпали, стоимость совпадения: inf" << endl;
                }
            } else {
                bool can_replace = !is_cursed[i-1] || (s[i-1] == 'z');
                if (can_replace) {   // Replace
                    if (dp[i-1][j-1] != INF) {
                        match_replace_cost = dp[i-1][j-1] + 1;
                        cout << "Символы не совпали, стоимость замены: " << match_replace_cost << endl;
                        dp[i][j] = min(dp[i][j], dp[i-1][j-1] + 1);
                    } else {
                        cout << "Символы не совпали, стоимость замены: inf" << endl;
                    }
                } else {
                    cout << "Символы не совпали, замена запрещена (проклятый символ не 'z')" << endl;
                }
            }

            cout << "Заносим минимальное значение " << (dp[i][j] == INF ? "inf" : to_string(dp[i][j])) 
                 << " в текущую ячейку матрицы" << endl;
        }
    }

    cout << "\nИтоговая матрица:\n";
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            if (dp[i][j] == INF) {
                cout << "inf ";
            } else {
                cout << dp[i][j] << " ";
            }
        }
        cout << endl;
    }

    if (dp[n][m] >= INF) {
        cout << "Вычислить расстояние Левенштейна невозможно" << endl; 
    } else {
        cout << "Расстояние Левенштейна: " << dp[n][m] << endl;
    }

    return 0;
}