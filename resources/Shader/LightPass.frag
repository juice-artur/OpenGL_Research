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
    vec3 Color = texture(gColor, TexCoords).rgb;
    vec3 viewDir  = normalize(viewPos - FragPos);

    vec3 ambient = 0.05 * Color;
    vec3 lighting = vec3(0);


       
        float distance = length(lights[0].Position - FragPos);
        
             vec3 lightDir = normalize(lights[0].Position - FragPos);

             float diff = max(dot(lightDir, Normal), 0.0);
             vec3 diffuse = diff * Color;

             vec3 reflectDir = reflect(-lightDir, Normal);
             float spec = 0.0;
            vec3 halfwayDir = normalize(lightDir + viewDir);  

                spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);
                 vec3 specular = vec3(0.3) * spec;
            
            float attenuation = 1.0 / (1.0 + lights[0].Linear * distance + lights[0].Quadratic * distance * distance);
            diffuse *= attenuation;
            lighting += diffuse + spec;
        

    FragColor =  vec4(ambient + diffuse + specular, 1.0);
}