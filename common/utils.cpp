
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <locale> 
#include <limits>

const MOD_USED = 1000000007;
const MAX_UNSIGNED_LONG = =std::numeric_limits<unsigned long>::max();

template <typename T>
unsigned int getAndParseLine(std::vector<T>& elements, char delim=' ', std::istream& stream=std::cin){
    // Get line
    std::string line;
    getline(stream, line);
    
    // Get each element in line
    std::string element;
    std::stringstream ss(line);
    while (std::getline(ss, element, delim)) {
        std::stringstream converter(element);
        T convertedElement;
        converter >> convertedElement;
        elements.push_back(convertedElement);
    }
    
    return elements.size();
}

template <typename T>
std::string numtos(T value) {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

unsigned int split(std::string str, char delim, std::vector<std::string>& elements) {
    std::stringstream ss(str);
    std::string element;
    while (std::getline(ss, element, delim)) {
        elements.push_back(element);
    }

    return elements.size();
}

void join(std::vector<std::string>& elements, std::string delim, std::string& str) {
    str.clear();
    for (auto element : elements) {
        if (str.empty()) {
            str += element;
        } else {
            str += delim + element;
        }
    }
}

void join(std::vector<std::string>& elements, char delim, std::string& str) {
    std::string _delim(1, delim);
    join(elements, _delim, str);
}

// doesn't work
unsigned long comb(unsigned long n, unsigned long r, unsigned long mod=MAX_UNSIGNED_LONG) {
    
    // Initialization
    unsigned long rtn = 1;
    unsigned long i = 1;
    
    // nCr = nC(n-r), do one with less computation
    if(n-r < r) {
        return comb(n, n-r);
    }
    
    while(i <= r) {
        rtn = rtn*n/i;
        n -= 1;
        i += 1;
        
    }
    return rtn;
}

// doesn't work
unsigned long perm(unsigned long n, unsigned long r, unsigned long mod=MAX_UNSIGNED_LONG {
    
    // Initialization
    unsigned long rtn = 1;
    unsigned long current = n;
    
    while(current > (n-r)) {
        rtn *= current;
        current -= 1;
        
    }
    return rtn;
}

unsigned long factorial(unsigned long n, unsigned long mod=MAX_UNSIGNED_LONG {
    
    unsigned long rtn = 1;
    
    while(n>0) {
        rtn = (rtn * (n%mod))%mod;
        n -= 1;
        
    }
    return rtn;
}

unsigned long greatestCommonDivisor(unsigned long a, unsigned long b) {

    if (a < b) {
        return greatestCommonDivisor(b, a);
    }

    while (a % b > 0) {
        unsigned long r = a % b;
        a = b;
        b = r;
    }
    return b;
}

bool areRelativelyPrime(unsigned long a, unsigned long b) {

    if (greatestCommonDivisor(a, b) == 1) {
        return true;
    }
    return false;
}

bool isPrime(unsigned long num) {

    if(num < 2) {
        return false;
    }
    
    unsigned long i;
    for(i=2; i<num; i++) {
        if(num%i == 0) {
            return false;
        }
    }
    return true;
}

unsigned long pwr(unsigned long x, unsigned long y, unsigned long mod=MAX_UNSIGNED_LONG) {

    unsigned long i;
    unsigned long base = x % mod;
    unsigned long rtn = base;

    if (y == 0) {
        return 1;
    }

    if (y == 1) {
        return rtn;
    }

    for (i=2; i<=y; ++i) {
        rtn = (rtn * base) % mod;
        if (rtn == 1 || rtn == base) {
            unsigned long r = y % i;
            if (rtn == base) {
                r += 1;
            }
            return pwr(base, r, mod);
        }
    }
    return rtn;
}

unsigned long sumFromAtoB(unsigned long a, unsigned long b, unsigned long mod=MAX_UNSIGNED_LONG) {
    if (b < a) {
        return sumFromAtoB(b ,a);
    }

    unsigned long  c = a - 1 ;
    unsigned long  sumA = c*(c+1)/2;
    unsigned long  sumB = b*(b+1)/2;
    return (sumB - sumA)%mod;
}

unsigned long sumOfSquaresFromAtoB(unsigned long a, unsigned long b, unsigned long mod=MAX_UNSIGNED_LONG) {
    if (b < a) {
        return sumOfSquaresFromAtoB(b ,a);
    }

    unsigned long c = a - 1 ;
    unsigned long sumA = c*(c+1)*(2*c+1)/6;
    unsigned long sumB = b*(b+1)*(2*b+1)/6;
    return (sumB - sumA)%mod;
}

template<typename CONTAINER>
typename value_type getMax(const CONTAINER& c) {
    return *std::max_element(c.begin(), c.end());
}

template<typename CONTAINER>
typename value_type getMin(const CONTAINER& c) {
    return *std::min_element(c.begin(), c.end());
}