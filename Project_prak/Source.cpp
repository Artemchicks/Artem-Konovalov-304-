#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <SOIL\SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



#include <Map>
#include <vector>
#include "stb_image.h"
#include "Header.h"


void keyboard_reaction(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_movement();
unsigned int get_texture(char const* path);
unsigned int get_texture_alpha(char const* path);
unsigned int get_skybox(std::vector<std::string> faces);

const GLuint SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;	
GLfloat pitch = 0.0f;
GLfloat lastX = SCREEN_WIDTH / 2.0;
GLfloat lastY = SCREEN_HEIGHT / 2.0;
GLfloat fov = 45.0f;
bool keys[1024];

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

GLfloat deltaTime = 0.0f;	
GLfloat lastFrame = 0.0f;  	

int main()
{

    glfwInit();
   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

  
    glfwSetKeyCallback(window, keyboard_reaction);
    glfwSetCursorPosCallback(window, mouse_callback);

   
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   
    glewExperimental = GL_TRUE;

    glewInit();

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    Shader Light("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/light1.vs",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/light2.vs");
    Shader Lamp("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/lamp1.vs",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/lamp2.vs");
    Shader shader("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/obvodka1.vs",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/obvodka2.vs");
    Shader Skybox("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox1.vs", 
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox2.vs");
    Shader shader1("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/SkyCube1.vs",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/SkyCube2.vs");

    float box_vertics[] = {

        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    glm::vec3 lamp_positions[] = {
       glm::vec3(0.7f,  0.2f,  2.0f),
    };
    glm::vec3 billboard_positions[] = {
       glm::vec3(1.5f,  0.0f,  1.3f),
       glm::vec3(-1.2f, 0.0f, -2.0f),
       glm::vec3(2.0f,  0.0f, -2.0f),
    };
    float billboard_vertics[] = {
  
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    float skybox_vertics[] = {         
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    std::vector<glm::vec3> windows
    {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(2.0f, 0.0f, 1.7f),

    };

    GLuint boxVBO, boxVAO;
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &boxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertics), box_vertics, GL_STATIC_DRAW);
    glBindVertexArray(boxVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    unsigned int billboardVAO, billboardVBO;
    glGenVertexArrays(1, &billboardVAO);
    glGenBuffers(1, &billboardVBO);
    glBindVertexArray(billboardVAO);
    glBindBuffer(GL_ARRAY_BUFFER, billboardVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(billboard_vertics), billboard_vertics, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertics), &skybox_vertics, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //Загрузка текстур
    unsigned int diffuseMap = get_texture("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/container2.png");
    unsigned int specularMap = get_texture("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/blick.png");
    unsigned int transparentTexture = get_texture_alpha("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/blending.png");   
    std::vector<std::string> skybox_parts
    {
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/right.jpg",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/left.jpg",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/top.jpg",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/bottom.jpg",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/front.jpg",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/back.jpg"
    };
    unsigned int cubemapTexture = get_skybox(skybox_parts);

    Light.Use();
    Light.setInt("material.diffuse", 0);
    Light.setInt("material.specular", 1);
    shader.Use();
    shader.setInt("texture1", 0);
    Skybox.Use();
    Skybox.setInt("skybox", 0);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        do_movement();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Рисуем скайбокс
        glDisable(GL_DEPTH_TEST);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection;
        glDepthFunc(GL_LEQUAL); 
        Skybox.Use();
        view = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp))); // remove translation from the view matrix
        Skybox.setMat4("view", view);
        Skybox.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);


        Light.Use();
        Light.setVec3("viewPos", cameraPos);
        Light.setFloat("material.shininess", 32.0f);

        Light.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        Light.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        Light.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        Light.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    
        Light.setVec3("pointLights[0].position", lamp_positions[0]);
        Light.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        Light.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        Light.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        Light.setFloat("pointLights[0].constant", 1.0f);
        Light.setFloat("pointLights[0].linear", 0.09);
        Light.setFloat("pointLights[0].quadratic", 0.032);

        Light.setVec3("pointLights[1].position", lamp_positions[1]);
        Light.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        Light.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        Light.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        Light.setFloat("pointLights[1].constant", 1.0f);
        Light.setFloat("pointLights[1].linear", 0.09);
        Light.setFloat("pointLights[1].quadratic", 0.032);

        Light.setVec3("pointLights[2].position", lamp_positions[2]);
        Light.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        Light.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        Light.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        Light.setFloat("pointLights[2].constant", 1.0f);
        Light.setFloat("pointLights[2].linear", 0.09);
        Light.setFloat("pointLights[2].quadratic", 0.032);
     
        Light.setVec3("pointLights[3].position", lamp_positions[3]);
        Light.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        Light.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        Light.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        Light.setFloat("pointLights[3].constant", 1.0f);
        Light.setFloat("pointLights[3].linear", 0.09);
        Light.setFloat("pointLights[3].quadratic", 0.032);

        Light.setVec3("spotLight.position", cameraPos);
        Light.setVec3("spotLight.direction", glm::vec3(0.0f, 0.0f, -1.0f));
        Light.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        Light.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        Light.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        Light.setFloat("spotLight.constant", 1.0f);
        Light.setFloat("spotLight.linear", 0.09);
        Light.setFloat("spotLight.quadratic", 0.032);
        Light.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        Light.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        model = glm::mat4(1.0f);
        view = glm::mat4(1.0f);
        projection = glm::mat4(1.0f);
        projection = glm::perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        Light.setMat4("projection", projection);
        Light.setMat4("view", view);
        Light.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glBindVertexArray(boxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Сортировка билбордов
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < windows.size(); i++)
        {
            float distance = glm::length(cameraPos - windows[i]);
            sorted[distance] = windows[i];
        }
        //Источник
        Lamp.Use();
        Lamp.setMat4("projection", projection);
        Lamp.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); 
        Lamp.setMat4("model", model);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, lamp_positions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); 
            Lamp.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Рисуем отсортированные билборды
        shader.Use();
        model = glm::mat4(1.0f);
        projection = glm::perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("model", model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            shader.setMat4("model", model);
            glBindVertexArray(lightVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, transparentTexture);
            glBindVertexArray(billboardVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

void keyboard_reaction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement()
{

    GLfloat cameraSpeed = 0.007f;
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;	
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

unsigned int get_texture(char const* path)
{
    unsigned int texture;
    glGenTextures(1, &texture);

    int width, height, nrComponents;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    if (image)
    {

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        SOIL_free_image_data(image);
    }
    else
    {
        std::cout << "Error, texture was not loaded "  << std::endl;
        SOIL_free_image_data(image);
    }

    return texture;
}

unsigned int get_texture_alpha(char const* path)
{
    unsigned int texture;
    glGenTextures(1, &texture);

    int width, height, component;
    unsigned char* image = stbi_load(path, &width, &height, &component, 0);
    if (image)
    {
        GLenum format;
        if (component == 1)
            format = GL_RED;
        else if (component == 3)
            format = GL_RGB;
        else if (component == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(image);
    }
    else
    {
        std::cout << "Error, skybox was not loaded. Part: " << path << std::endl;
        stbi_image_free(image);
    }

    return texture;
}

unsigned int get_skybox(std::vector<std::string> faces)
{
    unsigned int textureID; //Создаем объект 
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); //Точка привязки GL_TEXTURE_CUBE_MAP

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    //Настройки фильтрации
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}