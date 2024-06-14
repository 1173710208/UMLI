#include <iostream>
#include <random>
#include <vector>
#include <chrono>

#include "./models.hpp"
#include "./rmi.hpp"

using namespace std;
using namespace std::chrono;

int mainRMI()
{
    // Initialize random number generator.
    using key_type = uint64_t;
    random_device rand_dev; // will be used to obtain a seed for the random number engine
    std::mt19937 gen(rand_dev());
    std::uniform_int_distribution<key_type> key_distrib(0, 10);
    auto rand = [&gen, &key_distrib] { return key_distrib(gen); };

    // Create 1M random keys.
    std::size_t n_keys = 1000000;
    std::vector<key_type> keys(n_keys);
    std::generate(keys.begin(), keys.end(), rand);
    std::sort(keys.begin(), keys.end());

    // Build a two-layer RMI.
    using layer1_type = rmi::LinearSpline;
    using layer2_type = rmi::LinearRegression;
    std::size_t layer2_size = 2UL << 16;
    auto start_rmi = high_resolution_clock::now();
    rmi::RmiLAbs<key_type, layer1_type, layer2_type> rmi(keys, layer2_size);
    auto stop_rmi = high_resolution_clock::now();
    auto duration_rmi = duration_cast<microseconds>(stop_rmi - start_rmi);
    cout << duration_rmi.count() << " (us)" << endl;

    // Pick a key.
    std::uniform_int_distribution<std::size_t> uniform_distrib(0, n_keys - 1);
    key_type key = keys[uniform_distrib(gen)];

    // Perform a lookup.
    auto range = rmi.search(key);
    auto pos = std::lower_bound(keys.begin() + range.lo, keys.begin() + range.hi, key);
    std::cout << "Key " << key << " is located at position "
        << std::distance(keys.begin(), pos) << '.' << std::endl;

    return 0;
}