#include <iostream>
#include "vector.h"
#include "matrix.h"
#include "lu_decomposition.h"
#include "inverse.h"

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


    notlab::VectorI first = notlab::VectorI::fromList({1,2,3,7});
    notlab::VectorF second = notlab::VectorF::fromList({1.0f, 6.0f, 3.0f});

    std::cout << "Equal: " << notlab::isVectorEqual(first, second) << std::endl;

    notlab::MatrixI zeros = notlab::MatrixI::zeros(3, 2, "first");

    std::cout << zeros.toString() << std::endl;
    std::cout << zeros(3,1) << std::endl;

    notlab::MatrixI identity = notlab::MatrixI::identity(3, "second");

    identity(2,2) = 3;

    identity.printAll();

    identity.addRow({1,3,4});

    identity.printAll();

    identity.addColumn({5,7,9,0});

    identity.printAll();

    identity.addColumn(first);

    identity.printAll();

    notlab::MatrixI secondMatrix = notlab::MatrixI::zeros(4, 2, "to Add");
    secondMatrix.printAll();

    identity.addColumn(secondMatrix);

    identity.printAll();

    notlab::MatrixI testMatrix = notlab::MatrixI::fromList({{2,3,1},{4,7,5},{6,18,19}});

    notlab::VectorI tVector = testMatrix.getColumn(2);
    std::cout << tVector.toString() << std::endl;

    testMatrix.printAll();

    auto LU = notlab::gaussDollitle(testMatrix);

    LU.first.printAll();
    LU.second.printAll();

    notlab::inverseByLu(testMatrix);

    return 0;
}