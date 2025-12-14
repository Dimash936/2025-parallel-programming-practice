#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <math.h>

using namespace std;
std::vector<size_t> read_arr() {
    size_t length, a, b, p;
    std::cin >> length >> a >> b >> p;
    std::vector<size_t> result(length);
    result[0] = a % p;
    for (size_t i = 1; i < result.size(); ++i) {
        result[i] = (result[i - 1] * a + b) % p;
    }
    return result;
}

unsigned int tc = std::thread::hardware_concurrency();

std::vector<size_t> arr;
int n;

vector<pair<int, int>> f;
void sort_part(int l, int r, vector<size_t> &arr) {
    sort(arr.begin() + l, arr.begin() + r + 1);
    f.push_back({l, r});
}
void sort_(vector<size_t> &arr) {
    int sb = n / tc + 1;
    std::vector<std::thread> F;
    for(int i = 0; i < n; i += sb) {
        int nx = std::min(n - 1, i + sb - 1);
        F.emplace_back([i, nx, &arr] () {
            sort(arr.begin() + i, arr.begin() + nx + 1);
        });
        f.push_back({i, nx});
    }
    for(auto &i : F) {
        i.join();
    }
}
int main() {
    auto arr = read_arr();
    n = (int)arr.size();
    sort_(arr);

    while((int)f.size() > 1) {
        vector<thread> t;
        vector<pair<int, int>> bf;
        for(int i = 0; i < (int)f.size() - 1; i += 2) {
            auto X = f[i];
            auto Y = f[i + 1];
            t.emplace_back([X, Y, &arr]() {
                inplace_merge(arr.begin() + X.first,
                                   arr.begin() + X.second + 1,
                                   arr.begin() + Y.second + 1);
            });
            bf.push_back({f[i].first, f[i + 1].second});
        }
        if((int)f.size() % 2 == 1) {
            bf.push_back(f.back());
        }
        f = bf;
        for(auto &i : t) i.join();
    }
    sort(arr.begin(), arr.end());
    size_t k;
    std::cin >> k;
    for (size_t i = k - 1; i < arr.size(); i += k) {
        std::cout << arr[i] << ' ';
    }
    std::cout << "\n";

    return 0;
}