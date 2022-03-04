// Example 01
// The most typical usage of SimpleMT

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "../SimpleMT.h"

std::string some_heavy_work(int n)
{
    std::stringstream ss;
    for (int i = 0; i < n; i++) {
        ss << "*";
    }
    return ss.str();
}

int main()
{
    std::vector<int> data {3, 3, 4, 5, 5, 4, 3, 2, 1, 1, 2, 3, 3, 2, 2};
    auto res = SimpleMT<int, std::string>::map(data, some_heavy_work);
    for (const auto &it : res) {
        std::cout << it << std::endl;
    }
    return 0;
}
