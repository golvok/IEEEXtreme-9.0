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
        int smallest_p = *min_element(primes.begin(),primes.end());
        while (flips > 0) {
            flips--;
            int i = get<int>(std::cin);
            if (i <= n_switch && i != 0 )
                primes.push_back(i);
        }
        
        int ons = 0;
        for (int i = smallest_p; i <= n_switch;  i++ ) {
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
