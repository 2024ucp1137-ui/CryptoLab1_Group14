
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;
using namespace chrono;

using BigInt = cpp_int;

BigInt mod_pow(BigInt base, BigInt exp, const BigInt& mod) {
    BigInt result = 1;
    base %= mod;

    while (exp > 0) {
        if ((exp & 1) != 0)
            result = (result * base) % mod;

        base = (base * base) % mod;
        exp >>= 1;
    }

    return result;
}

bool miller_rabin(const BigInt& n, const vector<int>& bases) {
    if (n < 2)
        return false;

    if (n == 2 || n == 3)
        return true;

    if ((n & 1) == 0)
        return false;

    BigInt d = n - 1;
    unsigned int s = 0;

    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    for (int base : bases) {
        BigInt a = base;

        if (a >= n - 1)
            continue;

        BigInt x = mod_pow(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        bool passed = false;

        for (unsigned int r = 1; r < s; ++r) {
            x = (x * x) % n;

            if (x == n - 1) {
                passed = true;
                break;
            }
        }

        if (!passed)
            return false;
    }

    return true;
}

BigInt generate_candidate(mt19937_64& rng, unsigned int bits) {
    BigInt n = 0;

    for (unsigned int i = 0; i < bits; i += 64) {
        n <<= 64;
        n |= static_cast<uint64_t>(rng());
    }

    unsigned int excess = ((bits + 63) / 64) * 64 - bits;
    n >>= excess;

    n |= (BigInt(1) << (bits - 1));
    n |= 1;

    return n;
}

unsigned int bit_length(const BigInt& n) {
    if (n == 0)
        return 0;

    return static_cast<unsigned int>(
        msb(n) + 1
    );
}

int main() {
    const unsigned int assigned_bits = 512;

    const vector<int> bases = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71
    };

    random_device rd;
    mt19937_64 rng(
        static_cast<uint64_t>(rd()) ^
        static_cast<uint64_t>(
            high_resolution_clock::now()
                .time_since_epoch().count()
        )
    );

    unsigned long long candidates_tested = 0;
    BigInt prime;

    auto start = high_resolution_clock::now();

    while (true) {
        BigInt candidate = generate_candidate(rng, assigned_bits);
        ++candidates_tested;

        if (miller_rabin(candidate, bases)) {
            prime = candidate;
            break;
        }
    }

    auto finish = high_resolution_clock::now();

    double elapsed_ms =
        duration<double, milli>(finish - start).count();

    ofstream prime_file("outputs/prime_candidate.txt");
    prime_file << prime << '\n';
    prime_file.close();

    ofstream result_file("outputs/experiment_results.txt");

    result_file << "Cryptographic Prime Generation using Miller-Rabin\n";
    result_file << "Assigned bit length: " << assigned_bits << '\n';
    result_file << "Generated prime candidate: " << prime << '\n';
    result_file << "Actual bit length: " << bit_length(prime) << '\n';
    result_file << "Number of candidates tested: "
                << candidates_tested << '\n';
    result_file << "Miller-Rabin rounds per candidate: "
                << bases.size() << '\n';

    result_file << "Bases used: ";
    for (size_t i = 0; i < bases.size(); ++i) {
        if (i > 0)
            result_file << ", ";
        result_file << bases[i];
    }

    result_file << '\n';
    result_file << fixed << setprecision(3);
    result_file << "Total execution time (milliseconds): "
                << elapsed_ms << '\n';
    result_file << "Miller-Rabin result: PASS\n";

    result_file.close();

    cout << "Prime generation completed successfully.\n";
    cout << "Assigned bit length: " << assigned_bits << '\n';
    cout << "Actual bit length: " << bit_length(prime) << '\n';
    cout << "Candidates tested: " << candidates_tested << '\n';
    cout << "Miller-Rabin rounds: " << bases.size() << '\n';
    cout << "Execution time: " << fixed << setprecision(3)
         << elapsed_ms << " ms\n";
    cout << "Prime saved to outputs/prime_candidate.txt\n";
    cout << "Results saved to outputs/experiment_results.txt\n";

    return 0;
}
