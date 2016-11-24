#include <iostream>
#include <type_traits>

#include "units.h"

int main(int argc, char* argv[])
{
    using namespace units;
    auto m1 = 10.0_m;
    auto m2 = 20.0_cm;
    std::cout << (m1 + m2) << std::endl;
    std::cout << (m1 + 20) << std::endl;
    std::cout << (-m1 + 20) << std::endl;
    std::cout << (+m1 + 20) << std::endl;
    auto kg1 = 20.0_kg;
    std::cout << (m1 * kg1) << std::endl;
    std::cout << (m1 / kg1) << std::endl;
    auto n = 9.81_N;
    std::cout << n << std::endl;
    std::cout << (long double)(n / n) << std::endl;
    return 0;
}
