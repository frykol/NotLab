#include "test.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"

#include "figure.h"

void render() {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit()) {
    return;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "GLFW CMake starter", NULL, NULL);
  if (!window) {
    std::cout << "Window error" << std::endl;
    glfwTerminate();
    return;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    std::cout << "GLFW error" << std::endl;
    return;
  }
  glClearColor(1.0f, 0.0f, 0.0f, 0.0f);


  notlab::Figure f;
  notlab::Figure f2;
  notlab::Figure f3;


  glfwMakeContextCurrent(window);

  Shader s("../renderer/vertex.shader", "../renderer/fragment.shader");
  s.compile();

  float data[] = {-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f,
                  1.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.0f, 1.0f};
  unsigned int VAO;
  unsigned int VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glfwMakeContextCurrent(window);
    glClear(GL_COLOR_BUFFER_BIT);
    s.bind();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glfwSwapBuffers(window);

    glfwMakeContextCurrent(f.getWindow());   // ← dodamy getter
    glClear(GL_COLOR_BUFFER_BIT);
    s.bind();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glfwSwapBuffers(f.getWindow());

    // figure 2
    glfwMakeContextCurrent(f2.getWindow());
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(f2.getWindow());

    // figure 3
    glfwMakeContextCurrent(f3.getWindow());
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(f3.getWindow());

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
}
