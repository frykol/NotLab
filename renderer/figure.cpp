#include "figure.h"
#include <iostream>

namespace notlab{

  int Figure::s_Counter;

  static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  Figure::Figure(int id){
    m_Id = id;
  }

  Figure::Figure(const std::string& windowName, int windowWidth, int windowHeight): is_created(false){
    m_Window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);
    if (!m_Window) {
      std::cout << "Window error" << std::endl;
      glfwTerminate();
      return;
    }
    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

    int fbw, fbh;
    glfwGetFramebufferSize(m_Window, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);
    m_Id = s_Counter;
    s_Counter++;
    is_created = true;
  }

  Figure::~Figure(){
    if(m_Window){
      glfwDestroyWindow(m_Window);
    }
  }

  void Figure::render(){
      m_Shader->bind();
      glBindVertexArray(m_VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  void Figure::plot(){
    glfwMakeContextCurrent(m_Window);
    m_Shader = new Shader("../renderer/vertex.shader", "../renderer/fragment.shader");
    m_Shader->compile();

    float data[] = {-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f,
                    1.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.0f, 1.0f};
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
  }
}