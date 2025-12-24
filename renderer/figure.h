#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "text.h"
#include "../core/vector.h"

namespace notlab{

    struct Camera2D{
        glm::vec2 pan{0.0f, 0.0f};
        float zoom = 1.0f;
    };

    struct Rect2D{
      float x,y,w,h;
    };

    struct Line{
        glm::vec2 a, b;
    };

    class Figure{
        private:
            int m_Id;

            static int s_Counter;
        
            GLFWwindow* m_Window = nullptr;
            Shader* m_Shader = nullptr;
            Text* m_Text = nullptr;
            Camera2D m_Camera;

            Rect2D m_PlotBounds;
            VectorF m_X;
            VectorF m_Y;

            Line m_AxisX;
            Line m_AxisY;

            int m_NumberOfPoints;

            std::string m_WindowName;
            unsigned int m_Width;
            unsigned int m_Height;

            
            unsigned int m_VAO;
            unsigned int m_VBO;

            unsigned int m_LineVAO;
            unsigned int m_LineVBO;

            bool m_IsDragging;
            glm::vec2 m_LastMousePos{0.0f, 0.0f};
            glm::vec2 m_LastMouseScrollPos{0.0f, 0.0f};

            Figure(int id);
            void prepareAxis(const Rect2D& rect, float xmin, float xmax, float ymin, float ymax);

            float worldXtoDataX(float worldX) const;
            float worldYtoDataY(float worldY) const;

            float dataXtoWorldX(float dataX) const;
            float dataYtoWorldY(float dataY) const;

            std::string m_Title;
            std::string m_LabelX;
            std::string m_LabelY;

            glm::vec2 worldToDataClamped(const glm::vec2& world) const;
            glm::vec2 worldClamp(const glm::vec2& world) const;
            size_t findClosestIndexX(float dataX) const;

            void renderData();
            void renderAxis();
            void renderUI(const glm::mat4& projection, const glm::mat4& view);

        public:
            void render();  

            
            void plot();
            void prepareData(VectorF &x, VectorF &y);
            void setupInput();

            void setTitle(const std::string& title) { m_Title = title; }
            void setLabelX(const std::string& labelX) { m_LabelX = labelX; }
            void setLabelY(const std::string& labelY) { m_LabelY = labelY; }

            
            Figure(const std::string& windowName, int windowWidth, int windowHeight);
            ~Figure();
            
            GLFWwindow* getWindow(){
                return m_Window;
            }


            friend class Renderer;
    };
}