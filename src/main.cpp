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

  notlab::VectorF x = notlab::VectorF::fromRange(-20.0f, 40.0f,0.5f);
  //std::cout << x.toString() << std::endl;
  notlab::VectorF y = notlab::VectorF::zeros(x.getSize());
  notlab::VectorF y2 = notlab::VectorF::zeros(x.getSizre());

  for(size_t i = 1; i <= x.getSize(); i++){
    y(i) = std::pow(x(i), 2) + 3*x(i) - 5;
    y2(i) = std::sin(x(i));
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

  int fig3 = notlab::Renderer::instance().addFigure("Fig 3");
  notlab::Renderer::instance().testPlot(fig3, x, y2);
  notlab::Renderer::instance().setTitle(fig3, "Wykres");
  notlab::Renderer::instance().setLabelX(fig3, "x");
  notlab::Renderer::instance().setLabelY(fig3, "y");
  

  notlab::Renderer::instance().render();

  //render();
  return 0;
}
