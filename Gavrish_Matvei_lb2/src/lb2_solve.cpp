#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <climits>
#include <algorithm>
#include <bitset>

using namespace std;

const int INF = 1000000000;
int n;
vector<vector<int>> graph;

string mask_to_binary(int mask, int bits) {
    return bitset<15>(mask).to_string().substr(15 - bits);
}

pair<int, vector<int>> alsh1() {
    cout << "\n=== ЗАПУСК АЛШ-1  ===" << endl;

    vector<bool> visited(n, false);
    int current = 0;
    visited[current] = true;
    vector<int> path = {current};
    int total_cost = 0;

    cout << "Старт в городе 0" << endl;

    for (int iter = 0; iter < n - 1; ++iter) {
        int best_next = -1;
        int min_dist = INF;

        cout << "\n[Шаг " << (iter + 1) << "] Текущий город: " << current 
             << ", посещено: ";
        for (int i = 0; i < n; ++i) 
            if (visited[i]) cout << i << " ";
        cout << endl;
        cout << "  Перебор кандидатов:" << endl;
        
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && graph[current][v] > 0) {
                cout << "    Город " << v << ": расстояние = " << graph[current][v];
                if (graph[current][v] < min_dist) {
                    min_dist = graph[current][v];
                    best_next = v;
                    cout << " - новый минимум!" << endl;
                } else {
                    cout << endl;
                }
            }
        }
        
        if (best_next == -1) {
            cout << "  Нет доступных городов - тупик!" << endl;
            return {INF, {}};
        }
        
        cout << "  Выбран город " << best_next << " (расстояние " << min_dist << ")" << endl;

        visited[best_next] = true;
        path.push_back(best_next);
        total_cost += min_dist;
        current = best_next;

        cout << "  Путь: ";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i > 0) cout << " -> ";
            cout << path[i];
        }
        cout << ", общая стоимость: " << total_cost << endl;
    }

    cout << "\nФиниш. Попытка вернуться в город 0..." << endl;
    if (graph[current][0] > 0) {
        total_cost += graph[current][0];
        path.push_back(0);
        cout << "  Успех! Ребро " << current << " -> 0 (вес " << graph[current][0] << ")" << endl;
        cout << "  Итоговый путь: ";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i > 0) cout << " -> ";
            cout << path[i];
        }
        cout << ", общая стоимость: " << total_cost << endl;
        return {total_cost, path};
    }
    cout << "  Нет пути из " << current << " в 0" << endl;
    return {INF, {}};
}

vector<vector<int>> memo;
vector<vector<int>> next_node;

int tsp_dp(int mask, int u, int depth = 0) {
    string indent(depth * 2, ' ');

    cout << indent << "Рекурсия. tsp_dp(mask=" << mask_to_binary(mask, n) 
         << ", u=" << u << ")" << endl;

    if (mask == (1 << n) - 1) {
        cout << indent << "  Итог. Все города посещены. Ребро " << u << " -> 0: ";
        if (graph[u][0] > 0) {
            cout << graph[u][0] << endl;
        } else {
            cout << "НЕТ ПУТИ (0)" << endl;
        }
        return (graph[u][0] > 0) ? graph[u][0] : INF;
    }
    
    if (memo[mask][u] != -1) {
        cout << indent << "  MEMO. Возвращаем кэшированное значение: " << memo[mask][u] << endl;
        return memo[mask][u];
    }

    int ans = INF;
    int best_nxt = -1;

    cout << indent << "  Перебор кандидатов из города " << u << ":" << endl;

    for (int v = 0; v < n; ++v) {
        if (!(mask & (1 << v)) && graph[u][v] > 0) {
            cout << indent << "    Кандидат " << v << " (ребро " << u << "->" << v 
                 << " = " << graph[u][v] << ")..." << endl;
            
            int res = graph[u][v] + tsp_dp(mask | (1 << v), v);

            cout << indent << "    Результат для кандидата " << v << ": ";
            if (res >= INF) {
                cout << "INF (тупик)" << endl;
            } else {
                cout << graph[u][v] << " + " << (res - graph[u][v]) << " = " << res << endl;
            }

            if (res < ans) {
                ans = res;
                best_nxt = v;
                cout << indent << "    Новый минимум! ans = " << ans << ", best_next = " << v << endl;
            }
        }
    }
    
    memo[mask][u] = ans;
    next_node[mask][u] = best_nxt;

    cout << indent << "  Сохранение в MEMO. memo[" << mask_to_binary(mask, n) << "][" << u << "] = ";
    if (ans >= INF) {
        cout << "INF" << endl;
    } else {
        cout << ans << " (следующий: " << best_nxt << ")" << endl;
    }

    return ans;
}

void solve_exact() {
    cout << "\n=== ЗАПУСК ДП (Точный метод) ===" << endl;
    cout << "Инициализация таблиц: " << (1 << n) << " масок × " << n << " вершин" << endl;
    
    memo.assign(1 << n, vector<int>(n, -1));
    next_node.assign(1 << n, vector<int>(n, -1));

    cout << "\nВызов tsp_dp(mask=" << mask_to_binary(1, n) << ", u=0)" << endl;
    cout << string(50, '-') << endl;

    int min_cost = tsp_dp(1, 0);

    cout << string(50, '-') << endl;
    cout << "Минимальная стоимость: ";
    if (min_cost >= INF) {
        cout << "INF (no path)" << endl;
    } else {
        cout << min_cost << endl;
        cout << "\nВосстановление пути:" << endl;
        
        vector<int> path = {0};
        int mask = 1;
        int u = 0;
        
        while (mask != (1 << n) - 1) {
            int v = next_node[mask][u];
            cout << "  mask=" << mask_to_binary(mask, n) << ", u=" << u 
                 << " следующий: " << v << endl;
            path.push_back(v);
            mask |= (1 << v);
            u = v;
        }
        path.push_back(0);

        cout << "  Финальный путь: ";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i > 0) cout << " -> ";
            cout << path[i];
        }
        cout << endl;
        
        for (size_t i = 0; i < path.size(); ++i) {
            if (i > 0) cout << " ";
            cout << path[i];
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        ifstream file(argv[1]);
        if (!file.is_open()) {
            cerr << "Cannot open file: " << argv[1] << endl;
            return 1;
        }
        file >> n;
        graph.assign(n, vector<int>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                file >> graph[i][j];
            }
        }
        file.close();
    } else {
        cin >> n;
        graph.assign(n, vector<int>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> graph[i][j];
            }
        }
    }
    
    pair<int, vector<int>> approx_result = alsh1();
    int approx_cost = approx_result.first;
    vector<int> approx_path = approx_result.second;
    
    if (approx_cost == INF) {
        cout << "no path" << endl;
    } else {
        cout << approx_cost << endl;
        for (size_t i = 0; i < approx_path.size(); ++i) {
            if (i > 0) cout << " ";
            cout << approx_path[i];
        }
        cout << endl;
    }
    cout << endl;

    solve_exact();

    return 0;
}