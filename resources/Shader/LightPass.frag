#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
    float Radius;
};
const int NR_LIGHTS = 1;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{             
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 _C = texture(gColor, TexCoords).rgb;
    vec3 viewDir  = normalize(viewPos - FragPos);

    vec3 lighting  = _C;

    for(int i = 0; i < NR_LIGHTS; ++i)
    {
       
        float distance = length(lights[i].Position - FragPos);
        if(distance < lights[i].Radius)
        {
            vec3 lightDir = normalize(lights[i].Position - FragPos);
             vec3 diffuse = max(dot(Normal, lightDir), 0.0) * _C * lights[i].Color;

            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            
            float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
            diffuse *= attenuation;
            lighting += diffuse;
        }
    }    

    FragColor =vec4(_C, 1.0);
}