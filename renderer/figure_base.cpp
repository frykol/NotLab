#include "figure_base.h"

namespace notlab{
    
    //FigureBase::~FigureBase();

    void FigureBase::initCallbacks(){
        glfwSetWindowUserPointer(m_Window, this);

        glfwSetMouseButtonCallback(m_Window,
        [](GLFWwindow* w, int button, int action, int mods) {
            auto* self = static_cast<FigureBase*>(glfwGetWindowUserPointer(w));
            if (!self) return;
            self->onMouseButton(button, action, mods);
        });

        glfwSetCursorPosCallback(m_Window,
            [](GLFWwindow* w, double xpos, double ypos) {
                auto* self = static_cast<FigureBase*>(glfwGetWindowUserPointer(w));
                if (!self || !self->m_IsDragging) return;

                self->onCursorMove(xpos, ypos);
        });

        glfwSetScrollCallback(m_Window,
            [](GLFWwindow* w, double xoffset, double yoffset){
                auto* self = static_cast<FigureBase*>(glfwGetWindowUserPointer(w));
                if(!self) return;
                self->onCursorScroll(xoffset, yoffset);
        });
    }

    void FigureBase::beginFrame(){
        glfwGetFramebufferSize(m_Window, &m_Fbw, &m_Fbh);
        glViewport(0, 0, m_Fbw, m_Fbh);
    }

    void FigureBase::render(){
        beginFrame();
        renderScene();
        renderUI();
    }

    void FigureBase::onCursorMove(double xpos, double ypos){
        glm::vec2 cur{(float)xpos, (float)ypos};
        glm::vec2 delta = cur - m_LastMousePos;
        m_LastMousePos = cur;

        onDrag(xpos, ypos, delta);
    }

    void FigureBase::onMouseButton(int button, int action, int mods){
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                m_IsDragging = true;
                double x, y;
                glfwGetCursorPos(m_Window, &x, &y);
                m_LastMousePos = {(float)x, (float)y};
            } else if (action == GLFW_RELEASE) {
                  
                m_IsDragging = false;
            }
        }
    }
}