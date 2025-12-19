#include "renderer.h"
#include <iostream>

namespace notlab{
    //Renderer Renderer::renderer;

    void Renderer::init(){
        if(m_Status != Status::None){
            std::cout << "Error: Renderer is already initialized" << std::endl;
            return;
        }

        if (!glfwInit()) {
            return;
        }
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        m_mainWindow = glfwCreateWindow(640, 480, "Main hidden window", NULL, NULL);
        if (!m_mainWindow) {
            std::cout << "Window error" << std::endl;
            m_Status = Status::Error;
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_mainWindow);

        if (glewInit() != GLEW_OK) {
            std::cout << "GLFW error" << std::endl;
            m_Status = Status::Error;
            glfwTerminate();
            return;
        }

        m_Status = Status::Initialized;
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

        m_ErrorFigure = new Figure(-1);
    }

    Figure& Renderer::findFigure(int figureId){
        for(size_t i = 0; i < m_Figures.size(); i++){
            if(m_Figures[i]->m_Id == figureId){
                return *m_Figures[i];
            }
        }
        std::cout << "Cant find figure: " << figureId << std::endl;
        return *m_ErrorFigure;
    }

    int Renderer::addFigure(const std::string& windowName, int windowWidth, int windowHeight){
        if(m_Status != Status::Initialized && m_Status != Status::Ready){
            return -1;
        }
        m_Figures.emplace_back(std::make_unique<Figure>(windowName, windowWidth, windowHeight));
        Figure &fig = *m_Figures.back();
        m_Status = Status::Ready;
        return fig.m_Id;
    }

    void Renderer::testPlot(int figureId){
        if(m_Status != Status::Ready){
            return;
        }

        Figure& figure = findFigure(figureId);
        if(&figure == m_ErrorFigure){
            return;
        }
        figure.plot();
    }

    void Renderer::render(){
        if(m_Status != Status::Ready){
            std::cout << "Error: Renderer encounter error or have nothing to render" << std::endl;
        }
        m_isRunning = true;

        while(m_isRunning){
            glfwPollEvents();
                
            for(auto figure = m_Figures.begin(); figure != m_Figures.end();){
                GLFWwindow* win = figure->get()->getWindow();
                if(glfwWindowShouldClose(win)){
                    figure = m_Figures.erase(figure);
                    std::cout << "Zostało: " << m_Figures.size() << "okien" << std::endl;
                    continue;
                }
                
                glfwMakeContextCurrent(win);
                glClear(GL_COLOR_BUFFER_BIT);
                figure->get()->render();
                glfwSwapBuffers(win);


                figure++;
            }

            if(m_Figures.empty()){
                m_isRunning = false;
            }
        }
        delete m_ErrorFigure;
    }
}