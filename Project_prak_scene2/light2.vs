#version 330 core
out vec4 FragColor;

struct Material { //Свойства объекта
    sampler2D diffuse;
    sampler2D specular;
    float shininess; //Сила блеска
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 4

in vec3 Fragment_position;
in vec3 Normal_light;
in vec2 TexCoords;

uniform vec3 view_position;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_direction);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 view_direction);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 view_direction);

void main()
{    
    vec3 norm = normalize(Normal_light);    //Нормализуем нормали
    vec3 view_direction = normalize(view_position - Fragment_position);  //Вычисляем направление от источника к коробке
    
    vec3 result = CalcDirLight(dirLight, norm, view_direction); //Выходной цвет
    
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, Fragment_position, view_direction); //Значения из источноков освещения   
   
   // result += CalcSpotLight(spotLight, norm, Fragment_position, view_direction);     //Добавляем остальные значения
    
    FragColor = vec4(result, 1.0);
}

//Направленный источник освещения
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_direction)
{
    vec3 lightDir = normalize(-light.direction);// диффузное освещение
   
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);  // освещение зеркальных бликов
    float spec = pow(max(dot(view_direction, reflectDir), 0.0), material.shininess);
   
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 view_direction)
{
    vec3 lightDir = normalize(light.position - fragPos);
  
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(view_direction, reflectDir), 0.0), material.shininess);
   
    float distance = length(light.position - fragPos);  // затухание
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Комбинация результатов
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

//Прожектор
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 view_direction)
{
    vec3 lightDir = normalize(light.position - fragPos);    //Напрвление взгляда
    
    float diff = max(dot(normal, lightDir), 0.0);
   
    vec3 reflectDir = reflect(-lightDir, normal);   //Вектор отражения относительно нормали
    float spec = pow(max(dot(view_direction, reflectDir), 0.0), material.shininess);
   
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
   
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}