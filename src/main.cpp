#include <iostream>
#include "vector.h"
#include "matrix.h"
#include "lu_decomposition.h"
#include "inverse.h"
#include "determinant.h"
#include "linear_solve.h"

#include "../equation_parser/tokenizer.h"
#include "equation.h"

int main(void){

    std::string testString = "3^2+ 1+ max(sin(x1),2) * 10";
    
    notlab::Equation eq(testString);

    notlab::MatrixF variableValues = notlab::MatrixF::fromList({{3}});
    //float y = eq.eval();
     notlab::VectorF y = eq.eval(variableValues);

     std::cout << y.toString() << std::endl;

    //notlab::tokenize(testString);

    //std::cout << y << std::endl;
    
    return 0;
}