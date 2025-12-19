#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

namespace notlab{

    class Figure{
        private:
            int m_Id;

            static int s_Counter;

            GLFWwindow* m_Window;
            Shader* m_Shader;

            std::string m_Name;
            unsigned int m_Width;
            unsigned int m_Height;

            bool is_created;
            unsigned int m_VAO;
            unsigned int m_VBO;

            Figure(int id);
        public:
            void render();  
            void plot();
            
            Figure(const std::string& windowName, int windowWidth, int windowHeight);
            ~Figure();
            
            GLFWwindow* getWindow(){
                return m_Window;
            }


            friend class Renderer;
    };
}