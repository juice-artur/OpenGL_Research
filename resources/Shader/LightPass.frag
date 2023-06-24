#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform samplerCube shadowCubemap;
uniform float far_plane;
uniform vec3 lightPos;


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


float ShadowCalculation(vec3 fragPos)
{

    vec3 fragToLight = fragPos - lightPos;

    float closestDepth = texture(shadowCubemap, fragToLight).r;

    closestDepth *= far_plane;

    float currentDepth = length(fragToLight);

    float bias = 0.5; // мы используем гораздо большее теневое смещение, так как значение глубины теперь находитс€ в  диапазоне [near_plane, far_plane]
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;        


    return shadow;
}

void main()
{             
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Color = texture(gColor, TexCoords).rgb;
    vec3 viewDir  = normalize(viewPos - FragPos);

    vec3 lighting = vec3(0);


       
        float distance = length(lights[0].Position - FragPos);
        
           //  vec3 lightDir = normalize(lights[0].Position - FragPos);

             //float diff = max(dot(lightDir, Normal), 0.0);
             //vec3 diffuse = diff * Color;

             //vec3 reflectDir = reflect(-lightDir, Normal);
            // float spec = 0.0;
           // vec3 halfwayDir = normalize(lightDir + viewDir);  

             //   spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);
           //      vec3 specular = vec3(0.3) * spec;
            
          //  float attenuation = 1.0 / (1.0 + lights[0].Linear * distance + lights[0].Quadratic * distance * distance);
         //   diffuse *= attenuation;
      //      lighting += diffuse + spec;




    vec3 lightColor = vec3(0.3);

    // ‘онова€ составл€юща€
    vec3 ambient = 0.3 * Color;

    // ƒиффузна€ составл€юща€
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, Normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // ќтраженна€ составл€юща€
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(Normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
        
    float shadow = ShadowCalculation(FragPos);
    FragColor =  vec4(ambient + (1.0 - shadow) * (diffuse + specular) * Color, 1.0);
}