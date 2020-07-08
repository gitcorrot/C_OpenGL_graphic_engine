#version 330

    layout (location = 0) in vec3 Position;
    layout (location = 1) in vec3 Color;

    out vec3 vs_color;

    void main() {
        gl_Position.xyz = Position;
        gl_Position.w = 1.0;

        vs_color = Color;
    }
