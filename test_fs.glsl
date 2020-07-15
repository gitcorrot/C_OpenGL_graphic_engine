#version 330

    in vec3 vs_color;

    out vec4 fs_color;

    void main() 
    {
        fs_color = vec4(vs_color, 1.0f);
    }
