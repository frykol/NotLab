#include <iostream>
#include "vector.h"
#include "matrix.h"

int main(void){
    // notlab::VectorI test = notlab::VectorI::fromList({1,3,4,24});
    // std::cout << test.toString() << std::endl;
    // std::cout << test.getSize() << std::endl;

    // notlab::VectorI zeros = notlab::VectorI::zeros(10);
    // std::cout << zeros.toString() << std::endl;
    // std::cout << zeros.getSize() << std::endl;

    // zeros[3] = 5;

    // std::cout << zeros.toString() << std::endl;
    // std::cout << zeros.getSize() << std::endl;

    // int* ptest = &zeros[3];

    // zeros.addBack(3);
    // zeros.addBack(test);
    // zeros.addFront(159);
    // zeros.addFront(test);

    // test.addBack(100);

    // std::cout << zeros.toString() << std::endl;
    // std::cout << zeros.getSize() << std::endl;
    // std::cout << *ptest << std::endl;

    notlab::MatrixI zeros = notlab::MatrixI::zeros(3, 2);

    std::cout << zeros.toString() << std::endl;
    std::cout << zeros(3,1) << std::endl;

    return 0;
}