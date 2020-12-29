#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} shader_in;

uniform sampler2D wall_map_diffuse;
uniform sampler2D wall_map_normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{           
    
    vec3 normal = texture(wall_map_normal, shader_in.TexCoords).rgb; // выборка вектора из карты нормалей с областью значений [0,1]
    
    normal = normalize(normal * 2.0 - 1.0);  // перевод вектора нормали в интервал [-1,1]
  
    vec3 color = texture(wall_map_diffuse, shader_in.TexCoords).rgb;
   
    vec3 ambient = 0.1 * color;
    
    vec3 lightDir = normalize(shader_in.TangentLightPos - shader_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    vec3 viewDir = normalize(shader_in.TangentViewPos - shader_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}