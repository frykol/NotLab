#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "text.h"
#include "../core/vector.h"


namespace notlab{

    class FigureBase{
        protected:
            GLFWwindow* m_Window = nullptr;
            Shader* m_Shader = nullptr;
            Text* m_Text = nullptr;

            std::string m_WindowName;
            unsigned int m_Width;
            unsigned int m_Height;

            int m_Fbw;
            int m_Fbh;

            bool m_IsDragging;
            glm::vec2 m_LastMousePos{0.0f, 0.0f};
            glm::vec2 m_LastMouseScrollPos{0.0f, 0.0f};

            virtual void renderScene() = 0;
            virtual void renderUI() = 0;

            void beginFrame();

            void init();
            void initCallbacks();

            virtual void onDrag(const glm::vec2& delta);
            virtual void onCursorMove(double xpos, double ypos);
            virtual void onCursorScroll(double xoffset, double yoffset);
            virtual void onMouseButton(int button, int action, int mods);

        public:
            void render();

            virtual ~FigureBase() = default;



    };

}