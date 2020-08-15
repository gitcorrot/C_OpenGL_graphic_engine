#version 330

    layout (location = 0) in vec3 Position;
    layout (location = 1) in vec2 TexturePosition;
    layout (location = 2) in vec3 Normal;

    out vec2 vs_texturePosition;
    out vec3 vs_normal;
    out vec3 vs_toLight;
    out vec3 vs_toCamera;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform vec3 lightPosition;

    void main() 
    {        
        vec4 worldPosition = model * vec4(Position, 1.0); // 4x4 * 1x4 => 1x4

        gl_Position = projection * view * worldPosition;

        vs_texturePosition = TexturePosition;
        vs_normal = Normal; //mat3(transpose(inverse(model))) * Normal; // TODO: calculate normal matrix on CPU
        vs_toLight = lightPosition - worldPosition.xyz;
        vs_toCamera = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
    }
