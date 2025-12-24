#include "figure_base.h"

namespace notlab{
    
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

                

                // // UWAGA: GLFW cursor y rośnie w dół, a Ty masz ortho z y w górę
                // self->m_Camera.pan.x += delta.x;
                // self->m_Camera.pan.y -= delta.y; // odwróć oś Y
        });

        glfwSetScrollCallback(m_Window,
            [](GLFWwindow* w, double xoffset, double yoffset){
                auto* self = static_cast<FigureBase*>(glfwGetWindowUserPointer(w));
                if(!self) return;
                self->onCursorScroll(xoffset, yoffset);

                // if(yoffset > 0 && self->m_Camera.zoom >= 5.0f){
                // self->m_Camera.zoom = 5.0f;
                // return;
                // }
                // if(yoffset < 0 && self->m_Camera.zoom <= 0.5f){
                // self->m_Camera.zoom = 0.5;

                // return;
                // }

                // int fbw, fbh;
                // glfwGetFramebufferSize(w, &fbw, &fbh);

                // double mx, my;
                // glfwGetCursorPos(w, &mx, &my);
                // float sx = (float)mx;
                // float sy = (float)(fbh - my);
                // self->m_LastMouseScrollPos.x = sx;
                // self->m_LastMouseScrollPos.y = sy;


                // if(yoffset > 0){
                // self->m_Camera.zoom *= 1.1f;
                // }
                // else{
                // self->m_Camera.zoom *= 0.9f;
                // }
        });
    }

    void FigureBase::beginFrame(){
        glfwGetFramebufferSize(m_Window, &m_Fbw, &m_Fbh);
        //glViewport(0, 0, m_Fbw, m_Fbh);
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

        onDrag(delta);
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