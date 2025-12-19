#include "determinant.h"
#include "inverse.h"
#include "linear_solve.h"
#include "lu_decomposition.h"
#include "matrix.h"
#include "vector.h"
#include <iostream>

#include "../equation_parser/tokenizer.h"
#include "equation.h"
// #include "test.h"
#include "renderer.h"

#include "Shader.h"

int main(void) {
  notlab::Renderer::instance().init();
  std::string testString = "3^2+ 1+ max(sin(x1),2) * 10";

  notlab::Equation eq(testString);

  notlab::MatrixF variableValues = notlab::MatrixF::fromList({{3}});
  // float y = eq.eval();
  notlab::VectorF y = eq.eval(variableValues);

  std::cout << y.toString() << std::endl;

  // notlab::tokenize(testString);

  // std::cout << y << std::endl;
  int fid = notlab::Renderer::instance().addFigure();
  notlab::Renderer::instance().testPlot(fid);
  notlab::Renderer::instance().addFigure();


  notlab::Renderer::instance().render();

  //render();
  return 0;
}
