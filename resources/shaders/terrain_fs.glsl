#version 330

    in vec2 vs_texturePosition;
    in vec3 vs_color;

    out vec4 fs_color;

    uniform sampler2D Texture;

    void main() 
    {
        fs_color = vec4(vs_color, 1.0);
        // fs_color = texture(Texture, vs_texturePosition) * vec4(vs_color, 1.0);
        // fs_color = vec4(1.0, 0, 0, 1.0);
    }
