#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <thread>

std::vector<std::vector<double>> read_matrix() {
    size_t rows, cols;
    std::cin >> rows >> cols;

    size_t a, b, x, y, z, p;
    std::cin >> a >> b >> x >> y >> z >> p;
    std::vector<std::vector<size_t>> intermediate(rows, std::vector<size_t>(cols, b % p));
    intermediate[0][0] = a % p;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (i > 0 && j > 0) {
                intermediate[i][j] = (intermediate[i][j] + intermediate[i - 1][j - 1] * x) % p;
            }
            if (i > 0) {
                intermediate[i][j] = (intermediate[i][j] + intermediate[i - 1][j] * y) % p;
            }
            if (j > 0) {
                intermediate[i][j] = (intermediate[i][j] + intermediate[i][j - 1] * z) % p;
            }
        }
    }
    size_t max_value = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            max_value = std::max(max_value, intermediate[i][j]);
        }
    }

    std::vector<std::vector<double>> result(rows, std::vector<double>(cols));
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = static_cast<double>(intermediate[i][j]) / static_cast<double>(max_value);
        }
    }

    return result;
}

std::vector<std::thread> t;
std::mutex mtx;
std::vector<std::vector<double>> result;
std::vector<std::vector<double>> left, right;
void solve(int start_i, int step, int left_rows, int right_cols, int left_cols) {
    for(int i = start_i; i < left_rows; i += step) {
        for (int j = 0; j < right_cols; ++j) {
            double sum = 0;
            for (int k = 0; k < left_cols; ++k) {
//                mtx.lock();
                sum += left[i][k] * right[k][j];
//                mtx.unlock();
            }
            result[i][j] = sum;
        }
    }
}
int main() {
    unsigned int num_threads = std::thread::hardware_concurrency();
    left = read_matrix();
    right = read_matrix();
    auto left_rows = left.size();
    auto left_cols = left[0].size();
    auto right_cols = right[0].size();

    if (left.empty() or right.empty() or left[0].size() != right.size()) {
        std::cerr << "Wrong matrices";
        return 1;
    }
    result.assign(left_rows, std::vector<double>(right_cols));
    for(int i = 0; i < num_threads; i++) {
        t.emplace_back(solve, i, num_threads, left_rows, right_cols, left_cols);
    }
    for(auto& i : t) {
        i.join();
    }
    std::cout << left_rows << ' ' << right_cols << "\n";
    for (int i = 0; i < left_rows; ++i) {
        for (int j = 0; j < right_cols; ++j) {
            std::cout << std::setprecision(15) << result[i][j] << ' ';
        }
        std::cout << "\n";
    }

    return 0;
}