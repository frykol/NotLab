#include "figure.h"
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>

namespace notlab{

  int Figure::s_Counter;


  static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  Figure::Figure(int id): m_X(0), m_Y(0){
    m_Id = id;
  }

  Figure::Figure(const std::string& windowName, int windowWidth, int windowHeight): m_WindowName(windowName), m_Width(windowWidth), m_Height(windowHeight), m_X(0), m_Y(0){
    m_Window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);
    if (!m_Window) {
      std::cout << "Window error" << std::endl;
      glfwTerminate();
      return;
    }
    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
    setupInput();

    int fbw, fbh;
    glfwGetFramebufferSize(m_Window, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);
    m_Id = s_Counter;
    s_Counter++;

    m_Text = new Text();
    m_Text->init("../renderer/resources/Roboto-Regular.ttf", 48);
  }

  Figure::~Figure(){
    if(m_Window){
      glfwDestroyWindow(m_Window);
    }
    if(m_Shader){
      delete m_Shader;
    }
    if(m_Text){
      delete m_Text;
    }
  }

  void Figure::setupInput(){
      glfwSetWindowUserPointer(m_Window, this);
      glfwSetMouseButtonCallback(m_Window,
        [](GLFWwindow* w, int button, int action, int mods) {
            auto* self = static_cast<Figure*>(glfwGetWindowUserPointer(w));
            if (!self) return;

            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                if (action == GLFW_PRESS) {
                    self->m_IsDragging = true;
                    double x, y;
                    glfwGetCursorPos(w, &x, &y);
                    self->m_LastMousePos = {(float)x, (float)y};
                } else if (action == GLFW_RELEASE) {
                  
                    self->m_IsDragging = false;
                }
            }
        });

    glfwSetCursorPosCallback(m_Window,
        [](GLFWwindow* w, double xpos, double ypos) {
            auto* self = static_cast<Figure*>(glfwGetWindowUserPointer(w));
            if (!self || !self->m_IsDragging) return;

            glm::vec2 cur{(float)xpos, (float)ypos};
            glm::vec2 delta = cur - self->m_LastMousePos;
            self->m_LastMousePos = cur;

            // UWAGA: GLFW cursor y rośnie w dół, a Ty masz ortho z y w górę
            self->m_Camera.pan.x += delta.x;
            self->m_Camera.pan.y -= delta.y; // odwróć oś Y
        });

    glfwSetScrollCallback(m_Window,
       [](GLFWwindow* w, double xoffset, double yoffset){
        auto* self = static_cast<Figure*>(glfwGetWindowUserPointer(w));
        if(!self) return;

        if(yoffset > 0 && self->m_Camera.zoom >= 5.0f){
          self->m_Camera.zoom = 5.0f;
          return;
        }
        if(yoffset < 0 && self->m_Camera.zoom <= 0.5f){
          self->m_Camera.zoom = 0.5;

          return;
        }

        int fbw, fbh;
        glfwGetFramebufferSize(w, &fbw, &fbh);

        double mx, my;
        glfwGetCursorPos(w, &mx, &my);
        float sx = (float)mx;
        float sy = (float)(fbh - my);
        self->m_LastMouseScrollPos.x = sx;
        self->m_LastMouseScrollPos.y = sy;


        if(yoffset > 0){
          self->m_Camera.zoom *= 1.1f;
        }
        else{
          self->m_Camera.zoom *= 0.9f;
        }
        
    });
  }

  void Figure::prepareData(VectorF &x, VectorF &y){
      if(x.getSize() != y.getSize()){
        return;
      }
      glfwMakeContextCurrent(m_Window);
      m_Shader = new Shader("../renderer/resources/vertex.glsl", "../renderer/resources/fragment.glsl");
      m_Shader->compile();

      m_X = x;
      m_Y = y;

      float xmin = x.min();
      float xmax = x.max();
      float ymin = y.min();
      float ymax = y.max();
      m_NumberOfPoints = x.getSize();

      int fbw, fbh;
      glfwGetFramebufferSize(m_Window, &fbw, &fbh);

      float left=80, right=30, bottom=60, top=30;
      float plotX0 = left;
      float plotY0 = bottom;
      float plotW  = fbw - left - right;
      float plotH  = fbh - bottom - top;

      m_PlotBounds.x = plotX0;
      m_PlotBounds.y = plotY0;
      m_PlotBounds.w = plotW;
      m_PlotBounds.h = plotH;

      std::vector<float> vbo; 
      vbo.reserve(m_NumberOfPoints * 2);

      for (size_t i = 1; i <= m_NumberOfPoints; ++i) {
          float sx = plotX0 + (x(i) - xmin) / (xmax - xmin) * plotW;
          float sy = plotY0 + (y(i) - ymin) / (ymax - ymin) * plotH;
          vbo.push_back(sx);
          vbo.push_back(sy);
      }
      glGenVertexArrays(1, &m_VAO);
      glGenBuffers(1, &m_VBO);

      glBindVertexArray(m_VAO);

      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(float), vbo.data(), GL_DYNAMIC_DRAW);

      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glBindVertexArray(0);

      Rect2D rect{plotX0, plotY0, plotW,plotH};
      prepareAxis(rect, xmin, xmax, ymin, ymax);
  }

  // inline float mapX(float x, float xmin, float xmax, const Rect2D& plot) {
  //   return plot.x + (x - xmin) / (xmax - xmin) * plot.w;
  // }

  // inline float mapY(float y, float ymin, float ymax, const Rect2D& plot) {
  //     return plot.y + (y - ymin) / (ymax - ymin) * plot.h;
  // }

  Line computeYAxis(const Rect2D& rect, float xmin, float xmax){
      float axisX;
      axisX = rect.x - 22.0f;
      // if (xmin > 0.0f) {
      //     axisX = rect.x - 22.0f;
      // }
      // else if (xmax < 0.0f) {
      //     axisX = rect.x + rect.w + 22.0f;
      // }
      // else {
      //     axisX = mapX(0.0f, xmin, xmax, rect);
      // }

      return { { axisX, rect.y - 22.0f}, { axisX, rect.y + rect.h + 50.0f } };
  }

  Line computeXAxis(const Rect2D& rect, float ymin, float ymax){
      float axisY;
      axisY = rect.y - 22.0f;
      // if (ymin > 0.0f) {
      //     axisY = rect.y - 22.0f;
      // }
      // else if (ymax < 0.0f) {
      //     axisY = rect.y + rect.h + 22.0f;
      // }
      // else {
      //     axisY = mapY(0.0f, ymin, ymax, rect);
      // }

      return { { rect.x - 22.0f, axisY }, { rect.x + rect.w + 50.0f, axisY } };
  }

  void Figure::prepareAxis(const Rect2D& rect, float xmin, float xmax, float ymin, float ymax){
      Line yAxis = computeYAxis(rect, xmin, xmax);
      Line xAxis = computeXAxis(rect, ymin, ymax);

      m_AxisX = xAxis;
      m_AxisY = yAxis;

      float axisData[8] = {
        yAxis.a.x, yAxis.a.y,
        yAxis.b.x, yAxis.b.y,

        xAxis.a.x, xAxis.a.y,
        xAxis.b.x, xAxis.b.y
      };


      glGenVertexArrays(1, &m_LineVAO);
      glGenBuffers(1, &m_LineVBO);

      glBindVertexArray(m_LineVAO);

      glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(axisData), axisData, GL_DYNAMIC_DRAW);

      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glBindVertexArray(0);
      

      //Line yAxix = computeYAxis()
  }

  glm::vec2 mouseScreenToWolrd(double mouseX, double mouseY, int fbw, int fbh, const glm::mat4& view){
      float xw = (float)mouseX;
      float yw = (float)(fbh - mouseY); // flip Y (GLFW: 0 top, ortho: 0 bottom)

    // 2) cofnij kamerę (pan/zoom) = inverse(view)
      glm::vec4 p = glm::inverse(view) * glm::vec4(xw, yw, 0.0f, 1.0f);
      return glm::vec2(p.x, p.y);
  }

  float clamp(float x, float min, float max){
      if(x < min){
        return min;
      }
      if(x > max){
        return max;
      }
      return x;
  }

  glm::vec2 Figure::worldClamp(const glm::vec2& world) const{
      float wx = clamp(world.x, m_PlotBounds.x, m_PlotBounds.x + m_PlotBounds.w);
      float wy = clamp(world.y, m_PlotBounds.y, m_PlotBounds.y + m_PlotBounds.h);
      return { wx, wy };
  }

  glm::vec2 Figure::worldToDataClamped(const glm::vec2& world) const{
      // float wx = clamp(world.x, m_PlotBounds.x, m_PlotBounds.x + m_PlotBounds.w);
      // float wy = clamp(world.y, m_PlotBounds.y, m_PlotBounds.y + m_PlotBounds.h);
      // return { worldXtoDataX(wx), worldYtoDataY(wy) };
      return { worldXtoDataX(world.x), worldYtoDataY(world.y) };
  }

  float Figure::worldXtoDataX(float worldX) const{
      float t = (worldX - m_PlotBounds.x) / m_PlotBounds.w;
      float xmin = m_X.min(); 
      float xmax = m_X.max();
      float x = xmin + t * (xmax - xmin);
      return x;
  }

  float Figure::worldYtoDataY(float worldY) const{
      float t = (worldY - m_PlotBounds.y) / m_PlotBounds.h;
      float ymin = m_Y.min(); 
      float ymax = m_Y.max();
      float y = ymin + t * (ymax - ymin);
      return y;
  }

  float Figure::dataXtoWorldX(float dataX) const{
      float xmin = m_X.min(); 
      float xmax = m_X.max();
      return m_PlotBounds.x + (dataX - xmin) * m_PlotBounds.w / (xmax - xmin);
  }

  float Figure::dataYtoWorldY(float dataY) const
  {
      float ymin = m_Y.min(); 
      float ymax = m_Y.max();
      return m_PlotBounds.y + (dataY - ymin) * m_PlotBounds.h / (ymax - ymin);
  }


  size_t Figure::findClosestIndexX(float dataX) const{
      auto it = std::lower_bound(m_X.getData().begin(), m_X.getData().end(), dataX);
      if (it == m_X.getData().begin()){
        return 0;
      }
      if (it == m_X.getData().end()){
        return m_X.getSize() - 1;
      }

      size_t i1 = it - m_X.getData().begin();
      size_t i0 = i1 - 1;

      return (fabs(m_X.getData()[i0] - dataX) < fabs(m_X.getData()[i1] - dataX)) ? i0 : i1;
  }


  void Figure::renderData(){
      m_Shader->setVec3("uColor", glm::vec3(0.4f, 0.5f, 0.6f));
      glBindVertexArray(m_VAO);
      glDrawArrays(GL_LINE_STRIP, 0, m_NumberOfPoints);
      glPointSize(6.0f);
      glDrawArrays(GL_POINTS, 0, m_NumberOfPoints);
  }

  void Figure::renderAxis(){
      m_Shader->setVec3("uColor", glm::vec3(0.9f, 0.9f, 0.9f));
      glBindVertexArray(m_LineVAO);
      glDrawArrays(GL_LINES, 0, 4);
  }

  void Figure::renderUI(const glm::mat4& projection, const glm::mat4& view){
      int fbw, fbh;
      glfwGetFramebufferSize(m_Window, &fbw, &fbh);
      glViewport(0, 0, fbw, fbh);

      double xpos, ypos;
      glfwGetCursorPos(m_Window, &xpos, &ypos);

      glm::vec2 worldPos = worldClamp(mouseScreenToWolrd(xpos, ypos, fbw, fbh, view));
      

      glm::vec2 worldToData = worldToDataClamped(worldPos);

      std::ostringstream pos;

      std::ostringstream posScreen;
      std::ostringstream posData;

      size_t closestData = findClosestIndexX(worldToData.x);
      glm::vec2 functionData{m_X.getData()[closestData], m_Y.getData()[closestData]};

      float functionWorldX = dataXtoWorldX(functionData.x);
      float functionWorldY = dataYtoWorldY(functionData.y);


      float halfY = (m_AxisY.a.y + m_AxisY.b.y) / 2.0f;
      float halfX = (m_AxisX.a.x + m_AxisX.b.x) / 2.0f;

      glm::vec2 pivot = { halfY, halfX};
      glm::mat4 M(1.0f);
      M = glm::translate(M, glm::vec3(pivot, 0.0f));
      M = glm::rotate(M, glm::radians(90.0f), glm::vec3(0,0,1));
      M = glm::translate(M, glm::vec3(-pivot, 0.0f));



      pos << functionData.x << ", " << functionData.y;
      m_Text->drawWorld(pos.str().c_str(), functionWorldX - 40 * 1/m_Camera.zoom, functionWorldY + 20 * 1/m_Camera.zoom, 1/m_Camera.zoom * 0.4f, {1,1,1}, projection, view);

      posScreen << "Pozycja na wykresie: " << pos.str();
      m_Text->drawScreen(posScreen.str().c_str(),20, 48, 0.4f, {1,1,1}, projection);

      posData << "Pozycja myszki: " << worldToData.x << ", " << worldToData.y; 
      m_Text->drawScreen(posData.str().c_str(),20, 20, 0.4f, {1,1,1}, projection);

      
      if(!m_Title.empty()){
        m_Text->drawWorld(m_Title, halfX, halfY * 2, 1, {1,1,1}, projection, view);
      }

      if(!m_LabelY.empty()){
        m_Text->drawWorld(m_LabelY, halfY, halfX*2, 1, {1,1,1}, projection, view, M);
      }

      if(!m_LabelX.empty()){
        m_Text->drawWorld(m_LabelX, halfX, -10, 1, {1,1,1}, projection, view);
      }


      m_Shader->bind();
      m_Shader->setVec3("uColor", glm::vec3(1.0f, 0.0f, 0.0f));
      glBindVertexArray(m_VAO);

      glPointSize(10.0f);
      glDrawArrays(GL_POINTS, (GLint)closestData, 1);

  }

  

  void Figure::render(){

      int fbw, fbh;
      glfwGetFramebufferSize(m_Window, &fbw, &fbh);
      glViewport(0, 0, fbw, fbh);
      m_Shader->bind();

      glm::mat4 model(1.0f);
      //glm::mat4 view(1.0f);
      glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(m_Camera.pan, 0.0f));
      //view = glm::scale(view, glm::vec3(m_Camera.zoom, m_Camera.zoom, 1.0f));

      view = glm::translate(view, glm::vec3(m_LastMouseScrollPos.x, m_LastMouseScrollPos.y, 0.0f));
      view = glm::scale(view, glm::vec3(m_Camera.zoom, m_Camera.zoom, 1.0f));
      view = glm::translate(view, glm::vec3(-m_LastMouseScrollPos.x, -m_LastMouseScrollPos.y, 0.0f));


      glm::mat4 proj = glm::ortho(0.0f, (float)fbw, 0.0f, (float)fbh);
      //glm::mat4 proj(1.0f);
      m_Shader->setMat4("uModel", model);
      m_Shader->setMat4("uView", view);
      m_Shader->setMat4("uProjection", proj);
      

      renderData();
      renderAxis();
      renderUI(proj, view);      
  }


  // void Figure::plot(){
  //   glfwMakeContextCurrent(m_Window);
  //   m_Shader = new Shader("../renderer/resources/vertex.glsl", "../renderer/resources/fragment.glsl");
  //   m_Shader->compile();

  //   float data[] = {
  //     -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 
  //     0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  
  //     0.0f, 0.5f, 0.0f, 0.0f, 1.0f};
  //   glGenVertexArrays(1, &m_VAO);
  //   glGenBuffers(1, &m_VBO);

  //   glBindVertexArray(m_VAO);

  //   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  //   glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

  //   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  //   glEnableVertexAttribArray(0);
  //   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),(void *)(2 * sizeof(float)));
  //   glEnableVertexAttribArray(1);

  //   glBindBuffer(GL_ARRAY_BUFFER, 0);

  //   glBindVertexArray(0);
  // }
}