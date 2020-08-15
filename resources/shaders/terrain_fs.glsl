#version 330

    in vec2 vs_texturePosition;
    in vec3 vs_normal;
    in vec3 vs_toLight;
    in vec3 vs_toCamera;

    out vec4 fs_color;

    uniform sampler2D Texture;

    void main() 
    {
        vec3 nNormal = normalize(vs_normal);
        vec3 nToLight = normalize(vs_toLight);
        vec3 nToCamera = normalize(vs_toCamera);

        vec3 nFromLight = -nToLight;
        vec3 reflectedLight = reflect(nFromLight, nNormal);

        vec3 lightColor = vec3(1.0, 1.0, 1.0);

        float ambient = 0.1;
        float diffuse = max(0.0, dot(nNormal, nToLight)); 

        float specularFactor = max(0.0, dot(reflectedLight, nToCamera));
        float damperFactor = pow(specularFactor, 16);
        // vec3 specular = damperFactor * lightColor * 0.75; 

        vec3 light = ( ambient + diffuse ) * lightColor; 
        
        // fs_color =  texture(Texture, vs_texturePosition);
        fs_color =  texture(Texture, vs_texturePosition) * vec4(light, 1.0);
        // fs_color = vec4(1.0, 0, 0, 1.0);
    }
