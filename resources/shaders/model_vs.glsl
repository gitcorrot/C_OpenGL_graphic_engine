#version 330

    layout (location = 0) in vec3 Position;
    layout (location = 1) in vec2 TexturePosition;
    layout (location = 2) in vec3 Normal;

    out vec2 vs_texturePosition;
    out vec3 vs_normal;
    out vec4 worldPosition;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() 
    {
        worldPosition = model * vec4(Position, 1.0); // 4x4 * 1x4 => 1x4
        gl_Position = projection * view * worldPosition;
        vs_texturePosition = vec2(TexturePosition.x, 1-TexturePosition.y);
        vs_normal = mat3(transpose(inverse(model))) * Normal; // TODO: calculate normal matrix on CPU
    }
