#include <iostream>
#include "vector.h"
#include "matrix.h"
#include "lu_decomposition.h"
#include "inverse.h"
#include "determinant.h"

int main(void){
    notlab::MatrixF first = notlab::MatrixF::fromList({{1,2,3},{3,2,2},{1,6,12}}, "MacierzA");
    notlab::MatrixF second = notlab::MatrixF::fromList({{1,1,1},{6,12,1},{5,6,1}}, "MacierzB");

    notlab::MatrixF add = first + second;
    notlab::MatrixF sub = first - second;
    notlab::MatrixF mult = first * second;
    notlab::MatrixF div = first/first;

    add.printAll();
    sub.printAll();
    mult.printAll();
    div.printAll();

    div.setName("division matrix");

    div = 3 * div;

    div.printAll();

    notlab::MatrixI transpoze = notlab::MatrixI::fromList({{1,2,3},{3,2,1}}, "toTranspose");

    notlab::MatrixI transposedMatrix = notlab::MatrixI::transpose(transpoze);

    transpoze.printAll();
    transposedMatrix.printAll();

    notlab::VectorI matrixToVector = transposedMatrix.toVector();

    std::cout << matrixToVector.toString() << std::endl;
    std::cout << matrixToVector.getSize() << std::endl;
    

    notlab::MatrixI testFromVector = notlab::MatrixI::fromVector(matrixToVector, 3, 2);

    testFromVector.printAll();

    notlab::MatrixI testMinorMatrix = notlab::MatrixI::fromList({{3,2,1}, {1,12,7}, {6,2,1}});

    float determinant = notlab::determinantByLaplace(testMinorMatrix);

    std::cout << "Determinant: " << determinant << std::endl;

    return 0;
}