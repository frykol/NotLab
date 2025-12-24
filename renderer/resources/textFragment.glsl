#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uText;
uniform vec3 uColor;

void main() {
    float a = texture(uText, vUV).r; // GL_RED
    FragColor = vec4(uColor, a);
}
