#include "matrix.h"
#include "vector.h"
#include <iostream>
#include "equation.h"

#include "renderer.h"

int main(void) {
  notlab::Renderer::instance().init();
  // std::string testString = "3^2+ 1+ max(sin(x1),2) * 10";

  // notlab::Equation eq(testString);

  // notlab::MatrixF variableValues = notlab::MatrixF::fromList({{3}});
  // notlab::VectorF y = eq.eval(variableValues);

  // std::cout << y.toString() << std::endl;

  notlab::VectorF x = notlab::VectorF::fromRange(-20.0f, 20.0f,0.5f);
  //std::cout << x.toString() << std::endl;
  notlab::VectorF y = notlab::VectorF::zeros(x.getSize());

  for(size_t i = 1; i <= x.getSize(); i++){
    y(i) = std::pow(x(i), 2) + 3*x(i) - 5;
  }

  std::cout << y.toString() << std::endl;

  int fid = notlab::Renderer::instance().addFigure("Fig 1");
  notlab::Renderer::instance().testPlot(fid, x, y);
  // notlab::Renderer::instance().addFigure();
  for(size_t i = 1; i <= x.getSize(); i++){
    y(i) = x(i) * 2;
  }

  int fig2id = notlab::Renderer::instance().addFigure("Fig 2");
  notlab::Renderer::instance().testPlot(fig2id, x, y);
  notlab::Renderer::instance().setTitle(fig2id, "Wykres zupy od trupa");
  notlab::Renderer::instance().setLabelX(fig2id, "zupaadsdadasd");
  notlab::Renderer::instance().setLabelY(fig2id, "trupy");
  

  notlab::Renderer::instance().render();

  //render();
  return 0;
}
