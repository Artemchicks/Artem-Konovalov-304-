#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 Texture_Coords;
layout (location = 3) in vec3 Tangent; //Передаем касательные 
layout (location = 4) in vec3 Bitangent;

out VS_OUT { // Передаем 
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} shader_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    shader_out.FragPos = vec3(model * vec4(Position, 1.0));   
    shader_out.TexCoords = Texture_Coords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model))); //Мировые координаты
    vec3 T = normalize(normalMatrix * Tangent); 
    vec3 N = normalize(normalMatrix * Normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T); //Находим бикасательную
    
    mat3 TBN = transpose(mat3(T, B, N));    //Получаем матрицу TBN
    shader_out.TangentLightPos = TBN * lightPos; //В касательное пространство
    shader_out.TangentViewPos  = TBN * viewPos;
    shader_out.TangentFragPos  = TBN * shader_out.FragPos;
        
    gl_Position = projection * view * model * vec4(Position, 1.0);
}