#version 330

    layout (location = 0) in vec3 Position;
    layout (location = 1) in vec2 TexturePosition;
    layout (location = 2) in vec2 Normal;

    out vec2 vs_texturePosition;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() 
    {
        gl_Position = projection * view * model * vec4(Position, 1.0);
        vs_texturePosition = vec2(TexturePosition.x, TexturePosition.y);
    }
