#include "test.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void render() {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "GLFW CMake starter", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    return;
  }
  glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

  float data[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f};
  unsigned int VAO;
  unsigned int VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
}
