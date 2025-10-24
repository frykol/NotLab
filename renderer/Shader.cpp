#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::string Shader::readShader(const std::string &path) {
  std::ifstream file;

  file.open(path);

  std::stringstream shaderStream;

  shaderStream << file.rdbuf();

  file.close();
  std::string ret = shaderStream.str();
  return ret;
}

int Shader::compileShader(GLenum shaderType, const std::string &shaderSource) {
  char infolog[512];
  int succes;

  unsigned int shader = glCreateShader(shaderType);
  const char *shaderp = shaderSource.c_str();
  glShaderSource(shader, 1, &shaderp, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
  if (!succes) {
    glGetShaderInfoLog(shader, 512, NULL, infolog);
    std::cout << "ERROR: " << infolog << std::endl;
  }

  return shader;
}

void Shader::compile() {
  m_vertexSource = readShader(m_vertexSource);
  m_fragmentSource = readShader(m_fragmentSource);

  unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, m_vertexSource);
  unsigned int fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, m_fragmentSource);

  m_programId = glCreateProgram();
  glAttachShader(m_programId, vertexShader);
  glAttachShader(m_programId, fragmentShader);

  glLinkProgram(m_programId);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::bind() { glUseProgram(m_programId); }
