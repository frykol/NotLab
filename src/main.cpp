#include <iostream>
#include "vector.h"
#include "matrix.h"
#include "lu_decomposition.h"
#include "inverse.h"
#include "determinant.h"
#include "linear_solve.h"

#include "equation.h"

int main(void){

    std::string testString = "x1^2 + 3*x1 + 6";
    
    notlab::Equation eq(testString);

    notlab::MatrixF variableValues = notlab::MatrixF::fromList({{3}, {4}, {1}});

    notlab::VectorF y = eq.eval(variableValues);

    std::cout << y.toString() << std::endl;
    
    return 0;
}