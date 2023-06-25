#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform samplerCube shadowCubemap;
uniform float far_plane;
uniform vec3 lightPos;
uniform vec3 viewPos;



vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos)
{

    vec3 fragToLight = fragPos - lightPos;

    float currentDepth = length(fragToLight);


    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(shadowCubemap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane; 
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

void main()
{             
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Color = texture(gColor, TexCoords).rgb;
    vec3 viewDir  = normalize(viewPos - FragPos);

    vec3 lighting = vec3(0);  
   float distance = length(lightPos - FragPos);
        




    vec3 lightColor = vec3(0.3);

    vec3 ambient = 0.3 * Color;

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, Normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(Normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
        
    float shadow = ShadowCalculation(FragPos);
    FragColor =  vec4(ambient + (1.0 - shadow) * (diffuse + specular) * Color, 1.0);
}