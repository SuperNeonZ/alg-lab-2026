#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

int N;
int grid[25][25];

struct Move {
    int r, c, w;
};

bool can_place(int r, int c, int w) {
    if (r + w - 1 > N || c + w - 1 > N) return false;

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[r + i][c + j] == 1) return false;
        }
    }
    return true;
}

void set_square(int r, int c, int w, int val) {
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < w; ++j) {
            grid[r + i][c + j] = val;
        }
    }
}

int main() {
    if (!(cin >> N)) return 0;

    bool verbose = (N <= 9);

    cout << "Program start: N = " << N << endl;
    cout << "==================================================" << endl;

    auto start_time = chrono::high_resolution_clock::now();

    for (int limit = 1; limit <= N * N; ++limit) {

        cout << ">>> Searching with limit = " << limit << "..." << endl;
        
        for(int i = 1; i <= N; ++i)
            for(int j = 1; j <= N; ++j)
                grid[i][j] = 0;
        
        vector<Move> stack;
        stack.reserve(limit);
        
        int filled_area = 0;
        int start_row = 1;

        while (true) {
            if (filled_area == N * N) {
                cout << endl;
                cout << "==================================================" << endl;
                cout << "All cells are filled" << endl;
                cout << "The program has completed its work" << endl;
                cout << "==================================================" << endl;
                cout << endl;

                cout << "Minimum number of squares:" << endl;
                cout << limit << endl;

                for (const auto& m : stack) {
                    cout << m.r << " " << m.c << " " << m.w << endl;
                }

                auto end_time = chrono::high_resolution_clock::now();
                chrono::duration<double> diff = end_time - start_time;
                cout << "Execution speed: " << diff.count() << " sec" << endl;

                return 0;
            }

            int remaining_moves = limit - (int)stack.size();
            int remaining_area = N * N - filled_area;
            bool backtrack = false;

            if (remaining_moves == 0 || remaining_area > remaining_moves * (N - 1) * (N - 1)) {
                backtrack = true;

                if (verbose) {
                     if (remaining_moves == 0) cout << "  Limit reached" << endl;
                     else cout << "  Pruning: Not enough space" << endl;
                }
            }

            if (!backtrack) {
                int r = -1, c = -1;
                for (int i = start_row; i <= N; ++i) {
                    for (int j = 1; j <= N; ++j) {
                        if (grid[i][j] == 0) {
                            r = i; c = j;
                            goto found;
                        }
                    }
                }
                found:;

                if (r == -1) {
                    backtrack = true;
                } else {
                    int min_w = 1;
                    if (stack.empty()) min_w = (N + 1) / 2;

                    int max_s = min(N - r + 1, N - c + 1);
                    max_s = min(max_s, N - 1);

                    bool placed = false;
                    
                    for (int w = max_s; w >= min_w; --w) {
                        if (can_place(r, c, w)) {
                            set_square(r, c, w, 1);
                            stack.push_back({r, c, w});
                            filled_area += w * w;
                            
                            start_row = r;
                            placed = true;

                            if (verbose) {
                                cout << "  + Place " << w << "x" << w 
                                     << " at (" << r << ", " << c << ")" << endl;
                            }

                            break;
                        }
                    }
                    if (!placed){
                        backtrack = true;
                        cout << "  Unable to place square in (" 
                             << r << ", " << c << ")" << endl;
                    }
                }
            }

            if (backtrack) {
                if (stack.empty()) { // Следующий limit
                    cout << "  Backtrack. The stack is empty, moving on to the next limit" << endl;
                    break;
                }

                bool found_alternative = false;
                
                while (!stack.empty()) {
                    Move last = stack.back();
                    stack.pop_back();
                    
                    set_square(last.r, last.c, last.w, 0);
                    filled_area -= last.w * last.w;
                    
                    start_row = last.r;

                    if (verbose) {
                        cout << "  - Remove " << last.w << "x" << last.w 
                             << " form (" << last.r << ", " << last.c << ")" << endl;
                    }
                    
                    int next_w = last.w - 1;
                    int min_w_for_level = (stack.empty()) ? ((N + 1) / 2) : 1;

                    bool placed_smaller = false;
                    for (int w = next_w; w >= min_w_for_level; --w) {
                        if (can_place(last.r, last.c, w)) {
                            set_square(last.r, last.c, w, 1);
                            stack.push_back({last.r, last.c, w});
                            filled_area += w * w;
                            placed_smaller = true;

                            if (verbose) {
                                cout << "  Reduce the size to " << w << "x" << w << endl;
                                cout << "  Placing a square " << w << "x" << w 
                                     << " in position (" << last.r << ", " << last.c << ")" << endl;
                            }

                            break;
                        }
                    }
                    
                    if (placed_smaller) {
                        found_alternative = true;
                        break;
                    }
                }
                
                if (!found_alternative && stack.empty()) {
                    cout << "  Backtrack. All options exhausted for limit = " << limit << endl;
                    break;
                }
            }
        }
    }
    return 0;
}