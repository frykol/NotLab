#include <iostream>
#include "vector.h"

int main(void){
    
    notlab::VectorI ab = {1, 3};
    notlab::VectorI bb = {3,5};
    notlab::VectorF cc = {3.4, 2.2};
    notlab::VectorF res = ab + bb - cc;

    std::cout << res.toString() << std::endl;

    res = 2 * res * 2;
    std::cout << res.toString() << std::endl;

    res = res / 2;
    std::cout << res.toString() << std::endl;

    res = res / ab;

    std::cout << res.toString() << std::endl;

    notlab::VectorI ff = {1, 3};
    notlab::VectorI gg = {3,2};

    notlab::VectorI hh = ff / gg;
    std::cout << hh.toString() << std::endl;

    return 0;
}