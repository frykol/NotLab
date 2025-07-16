#include <iostream>
#include "vector.h"
#include "matrix.h"
#include "lu_decomposition.h"
#include "inverse.h"
#include "determinant.h"
#include "linear_solve.h"

#include "parser.h"

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

    notlab::MatrixI testMinorMatrix = notlab::MatrixI::fromList({{3,2,1}, {1,12,7}, {6,2,1}}, "testez");

    float determinant = notlab::determinantByLaplace(testMinorMatrix);

    std::cout << "Determinant: " << determinant << std::endl;

    std::cout << "Matrix equal: " << notlab::isMatrixEqual(testMinorMatrix, transpoze) << std::endl;


    notlab::MatrixF inverse = notlab::inverseByLu(testMinorMatrix);
    inverse.printAll();

    notlab::MatrixI matrixA = notlab::MatrixI::fromList({{1,1,1}, {2,3,7}, {2,5,9}});
    notlab::MatrixI vectorb = notlab::MatrixI::fromList({{6},{28},{36}});

    notlab::VectorF vectorx = notlab::linearSolveByLu(matrixA, vectorb);

    matrixA.printAll();
    vectorb.printAll();

    std::cout << vectorx.toString() << std::endl;

    notlab::MatrixI testmacierz = notlab::MatrixI::fromList({{1,2},{3,2},{5,6}});

    testmacierz.printAll();

    testmacierz.transpose();
    testmacierz.printAll();

    std::string testString = "x1 + 3*-3 + 5";
    notlab::tokenize(testString);

    return 0;
}