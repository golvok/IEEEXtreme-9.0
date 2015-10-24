#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

template<typename T, typename STREAM>
T get(STREAM& is) {
    T t;
    is >> t;
    return t;
}

int main() {
    std::ios_base::sync_with_stdio (false);

    int cases = get<int>(std::cin);
    while (cases > 0 ) {
        cases--;
        int n_switch = get<int>(std::cin);
        int flips = get<int>(std::cin);
        
        vector<int> primes;
        while (flips > 0) {
            flips--;
            primes.push_back(get<int>(std::cin));
        }
        
        int ons = 0;
        for (int i = 1; i <= n_switch;  i++ ) {
            int flop = 0;
            // cout << "--" << i << endl;
            for (std::vector<int>::iterator it = primes.begin() ; it != primes.end(); ++it) {
                if (i % *it == 0 || *it == 1) {flop++; }
            }
            if (flop % 2 != 0) {ons++;}
        }
        cout << ons << endl;
    }
    return 0;
}
