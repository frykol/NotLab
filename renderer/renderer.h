#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <figure.h>
#include <vector>

#include <memory>

#include <../core/vector.h>


namespace notlab{
    class Renderer{
        private:
            enum class Status{
                None = 0, Initialized, Ready, Error
            };
            
            
            GLFWwindow* m_mainWindow;
            Status m_Status = Status::None;
            
            bool m_isRunning = false;
            std::vector<std::unique_ptr<Figure>> m_Figures;
            Figure* m_ErrorFigure;

            Figure& findFigure(int figureId);

            Renderer() = default;
        public:
            Renderer(const Renderer&) = delete;
            Renderer& operator=(const Renderer&) = delete;

            static Renderer& instance(){
                static Renderer inst;
                return inst;
            }

            void init();
            void render();
            int addFigure(const std::string& windowName="figure", int windowWidth = 640, int windowHeight = 480);
            void testPlot(int figureId, VectorF& x, VectorF& y);
            void setLabelX(int fiugreId, const std::string& labelX);
            void setLabelY(int fiugreId, const std::string& labelY);
            void setTitle(int fiugreId, const std::string& title);



    };
}