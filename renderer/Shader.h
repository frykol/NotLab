#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Shader {
private:
  unsigned int m_programId;

  unsigned int m_vertexId;
  unsigned int m_fragmentId;

  std::string m_vertexSource;
  std::string m_fragmentSource;

  std::string readShader(const std::string &path);
  int compileShader(GLenum shaderType, const std::string &shaderSource);

public:
  Shader(const std::string &vertexS, const std::string &fragmentS)
      : m_vertexSource(vertexS), m_fragmentSource(fragmentS) {}

  void compile();

  void bind();
  void unbind();
};
