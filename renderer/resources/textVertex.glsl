#version 330 core
layout(location = 0) in vec4 aVertex; // x, y, u, v
out vec2 vUV;

uniform mat4 uModel;
uniform mat4 uProjection;
uniform mat4 uView;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aVertex.xy, 0.0, 1.0);
    vUV = aVertex.zw;
}