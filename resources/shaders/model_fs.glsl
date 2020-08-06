#version 330

    in vec2 vs_texturePosition;
    in vec3 vs_normal;
    in vec4 worldPosition;

    out vec4 fs_color;

    uniform sampler2D Texture;
    // uniform vec3 lightPosition;

    void main() 
    {
        vec3 lightPosition = vec3(5.0, 5.0, 5.0);

        vec3 nNormal = normalize(vs_normal);
        vec3 nLightDirection = normalize(lightPosition - worldPosition.xyz);
        float diffuse = max(0.1, dot(nNormal, nLightDirection)); // max to avoid values below 0
        vec4 light = (texture(Texture, vs_texturePosition) + vec4(0.3, 0.2, 0.0, 0.0) + 0.1) * diffuse; 
        fs_color =  (light); //* vec4(0.7, 0.5, 1.0, 1.0);
        //fs_color = vec4(0.5, 0.0, 0.0, 1.0);
    }
