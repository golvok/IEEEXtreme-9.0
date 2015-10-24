
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <locale> 
#include <limits>


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

using namespace std;

int main() {
    vector<int> numTestCases;
    getAndParseLine(numTestCases);
    int i,j;
    for (i=0; i<numTestCases[0]; ++i) {
        vector<unsigned long> ingredients;
        getAndParseLine(ingredients);
        unsigned long shells = ingredients[0];
        unsigned long i1 = ingredients[1];;
        unsigned long i2 = ingredients[2];;
        unsigned long i3 = ingredients[3];
        unsigned long l,h,m;
        if (i1 > i2) {
            if (i1 > i3) {
                h = i1;
                if(i2 > i3) {
                    m = i2;
                    l = i3;
                } else {
                    m = i3;
                    l = i2;
                }
            } else {
                h = i3;
                m = i1;
                l = i2;
            }
        } else {
            if(i2 > i3) {
                h = i2;
                if(i1 > i3) {
                    m = i1;
                    l = i3;
                } else {
                    m = i3;
                    l = i1;
                }
            } else {
                h = i3;
                m = i2;
                l = i1;
            }
        }
        
        unsigned long temp, ans;
        if(l+m > h) {
             temp = (l+m+h)/2;
        } else {
            temp = l+m;
        }
        ans = min(temp, shells);
        cout << ans << endl;
        
    }  
    return 0;
}
