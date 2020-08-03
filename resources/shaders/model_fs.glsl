#version 330

    in vec2 vs_texturePosition;

    out vec4 fs_color;

    uniform sampler2D Texture;

    void main() 
    {
        fs_color = texture(Texture, vs_texturePosition);
    }
