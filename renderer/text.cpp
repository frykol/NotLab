#include "text.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

namespace notlab{

    Text::~Text(){
        if(m_Shader){
            delete m_Shader;
        }
    }

    int Text::init(const std::string& fontPath, int pixelHeight){
        m_Shader = new Shader("../renderer/resources/textVertex.glsl", "../renderer/resources/textFragment.glsl");
        m_Shader->compile();

        FT_Library ft;
        if(FT_Init_FreeType(&ft)){
            std::cout << "Error: Freetype Init failed" << std::endl;
            return -1;
        }

        FT_Face face;

        if(FT_New_Face(ft, fontPath.c_str(), 0, &face)){
            std::cout << "Error: Freetype new Face failed" << std::endl;
            FT_Done_FreeType(ft);
            return -2;
        }

        FT_Set_Pixel_Sizes(face, 0, pixelHeight);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for(unsigned char c = 32; c < 128; c++){
            if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
                continue;
            }

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Glyph glyph;
            glyph.texture = texture;
            glyph.size = { (int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows};
            glyph.bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top};
            glyph.advance = (unsigned int) face->glyph->advance.x;

            m_Glyphs[c] = glyph;
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return 0;
    }
    

    void Text::drawScreen(const std::string& text, float x, float y, float scale, const glm::vec3& color, const glm::mat4& projection, const glm::mat4& model){
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // m_Shader->bind();
        // m_Shader->setMat4("uProjection", projection);
        // m_Shader->setVec3("uColor", color);
        // glActiveTexture(GL_TEXTURE0);

        // glBindVertexArray(m_VAO);

        // for(unsigned char c : text){
        //     auto glyphIterator = m_Glyphs.find(c);
        //     if(glyphIterator == m_Glyphs.end()){
        //         std::cout << "Cant find char: " << c << std::endl;
        //         continue;
        //     }
        //     const Glyph& glyph = glyphIterator->second;

        //     float xpos = x + glyph.bearing.x * scale;
        //     float ypos = y - (glyph.size.y - glyph.bearing.y) * scale;
        //     float w = glyph.size.x * scale;
        //     float h = glyph.size.y * scale;

        //     float vertices[6][4] = {
        //         { xpos,     ypos + h, 0.0f, 0.0f },
        //         { xpos,     ypos,     0.0f, 1.0f },
        //         { xpos + w, ypos,     1.0f, 1.0f },

        //         { xpos,     ypos + h, 0.0f, 0.0f },
        //         { xpos + w, ypos,     1.0f, 1.0f },
        //         { xpos + w, ypos + h, 1.0f, 0.0f }
        //     };
        //     glBindTexture(GL_TEXTURE_2D, glyph.texture);
        //     glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        //     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        //     glDrawArrays(GL_TRIANGLES, 0, 6);

        //     x += (glyph.advance >> 6) * scale;
        // }

        // glBindVertexArray(0);
        // glBindTexture(GL_TEXTURE_2D, 0);
        drawWorld(text, x, y, scale, color, projection, glm::mat4(1.0f), model);
    }

    void Text::drawWorld(const std::string& text, float x, float y, float scale, const glm::vec3& color, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        m_Shader->bind();
        m_Shader->setMat4("uModel", model);
        m_Shader->setMat4("uProjection", projection);
        m_Shader->setMat4("uView", view);
        m_Shader->setVec3("uColor", color);
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(m_VAO);

        for(unsigned char c : text){
            auto glyphIterator = m_Glyphs.find(c);
            if(glyphIterator == m_Glyphs.end()){
                std::cout << "Cant find char: " << c << std::endl;
                continue;
            }
            const Glyph& glyph = glyphIterator->second;

            float xpos = x + glyph.bearing.x * scale;
            float ypos = y - (glyph.size.y - glyph.bearing.y) * scale;
            float w = glyph.size.x * scale;
            float h = glyph.size.y * scale;

            float vertices[6][4] = {
                { xpos,     ypos + h, 0.0f, 0.0f },
                { xpos,     ypos,     0.0f, 1.0f },
                { xpos + w, ypos,     1.0f, 1.0f },

                { xpos,     ypos + h, 0.0f, 0.0f },
                { xpos + w, ypos,     1.0f, 1.0f },
                { xpos + w, ypos + h, 1.0f, 0.0f }
            };
            glBindTexture(GL_TEXTURE_2D, glyph.texture);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (glyph.advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}