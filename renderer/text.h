#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>
#include "Shader.h"

namespace notlab{

    struct Glyph
    {
        unsigned int texture = 0;
        glm::ivec2 size{};
        glm::ivec2 bearing{};
        unsigned int advance = 0;
    };



    class Text{
        private:
            std::unordered_map<char, Glyph> m_Glyphs;
            unsigned int m_VAO;
            unsigned int m_VBO;
            Shader* m_Shader;

        public:
            ~Text();

            int init(const std::string& fontPath, int pixelHeight);
            void drawScreen(const std::string& text, float x, float y, float scale, const glm::vec3& color, const glm::mat4& projection, const glm::mat4& model = glm::mat4(1.0f));
            void drawWorld(const std::string& text, float x, float y, float scale, const glm::vec3& color, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model = glm::mat4(1.0f));
    };

}