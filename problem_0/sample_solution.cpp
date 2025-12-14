#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <thread>
#include <mutex>
#include <atomic>
#include <cmath>


std::istream& operator>>(std::istream& in, __int128& value) {
    std::string s;
    in >> s;
    value = 0;
    bool negative = false;
    size_t i = 0;
    if (s[0] == '-') {
        negative = true;
        i = 1;
    }
    for (; i < s.size(); ++i) {
        value = value * 10 + (s[i] - '0');
    }
    if (negative) value = -value;
    return in;
}

std::ostream& operator<<(std::ostream& out, __int128 value) {
    if (value == 0) {
        out << '0';
        return out;
    }
    std::string s;
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = -value;
    }
    while (value > 0) {
        s += '0' + static_cast<int>(value % 10);
        value /= 10;
    }
    if (negative) s += '-';
    std::reverse(s.begin(), s.end());
    out << s;
    return out;
}

std::mutex mtx;
__int128 n;
std::vector<__int128> factors;
void make(__int128_t start, __int128_t step) {
    for(__int128 p = start; p * p <= n; p += step) {

        while(n % p == 0) {
            mtx.lock();
            factors.push_back(p);
            n /= p;
            mtx.unlock();
        }
    }
}
int main() {
    std::cin >> n;
    if (n <= 1) {
        return 0;
    }
    unsigned int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> t;
    for (unsigned int i = 0; i < num_threads; ++i) {
        t.emplace_back(make, i + 2, num_threads);
    }
    for(auto& i : t) {
        i.join();
    }
    if (n > 1) {
        factors.push_back(n);
    }
    for (const auto& factor : factors) {
        std::cout << factor << ' ';
    }
    std::cout << '\n';

    return 0;
}