#version 330

    layout (location = 0) in vec3 Position;
    layout (location = 1) in vec2 TexturePosition;
    layout (location = 2) in vec3 Normal;
    layout (location = 3) in vec3 Color;

    out vec2 vs_texturePosition;
    out vec3 vs_color;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform vec3 lightPosition;

    void main() 
    {        
        vec4 worldPosition = model * vec4(Position, 1.0); // 4x4 * 1x4 => 1x4

        gl_Position = projection * view * worldPosition;

        vec3 nNormal = normalize(Normal);
        vec3 nToLight = normalize(lightPosition - worldPosition.xyz);

        vec3 lightColor = vec3(255.0/255.0, 194.0/255.0, 102.0/255.0);
        float ambient = 0.2;
        float diffuse = max(0.0, dot(nNormal, nToLight)); 
        vec3 light = ( ambient + diffuse ) * lightColor; 

        vs_texturePosition = TexturePosition;
        // vs_color = Color;
        vs_color = Color * light;
    }
