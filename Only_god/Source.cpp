#include <iostream>
#include <stb_image.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
// GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Header.h"
#include <SOIL/SOIL2.h>

//Функция обратного вызова при использовании клавиатуры
void keybord_reaction(GLFWwindow* window, int key, int scancode, int action, int mode);

void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
unsigned int get_texture(char const* path);
void render();
unsigned int get_skybox(std::vector<std::string> faces);

const GLuint SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = SCREEN_WIDTH / 2.0;
GLfloat lastY = SCREEN_HEIGHT / 2.0;
bool keys[1024];
float heightScale = 0.1;
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;

int main()
{
    glfwInit(); //Инициализация GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//Задаем обрабатываемые версии
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Профайл и контекст
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//Выключаем возможность изменения размера окна
    //Создаем объект окна
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);//Создаем контекст окна
    glfwSetKeyCallback(window, keybord_reaction);//Установка реакции на клавиатуру
    glewExperimental = GL_TRUE;//Подключение каких-то важных техник для GLEW
    glewInit(); //Инициализация GLEW   
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);//Устанавливаем размер окна
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);//Включить трафаретное тестирование
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    Shader Box("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/shader1.vs", 
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/shader2.vs");
    Shader Obvodka("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/DrawColor1.vs",
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/DrawColor2.vs");
    Shader Wall("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/shader1.vs",
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/shader2.vs");
    
    Shader SkyboxShader("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/sky1.vs",
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/sky2.vs");
    Shader shader1("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/skycube1.vs",
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/skycube2.vs");
    Shader RM("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/PM1.vs", 
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Only_god/PM2.vs");
    
    float box_vertics[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    glm::vec3 box_coords[] = {
      glm::vec3(-2.0f,  1.0f,  3.0f),
      glm::vec3(2.0f,  3.0f, -1.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
    };
    
    
    GLuint VBO, VAO, EBO;//VBO - объект вершинного буфера, 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);//Создаем VBO
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//Привязка к буферу
    glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertics), box_vertics, GL_STATIC_DRAW);//Копироввание вершин в буфер
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);//Интерпретация вершинных данных и включение атрибута
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    //Текстура коробки
    GLuint texture_of_box = get_texture("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/container.jpg");
    
    //Загружаем текстуры стены
    unsigned int wall_map_diffuse = get_texture("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/wall1.jpg");
    unsigned int wall_map_normal = get_texture("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/wall2.jpg");
    
    //Текстуры для RM
    unsigned int RM_map_diffuse = get_texture("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/wall_real.jpg");
    unsigned int RM_map_normal = get_texture("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/wall_normal.jpg");
    unsigned int RM_map_depth = get_texture("C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/wall_disp.jpg");
    
    //Скайбокс
    std::vector<std::string> skybox_parts
    {
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/skybox/right.jpg",
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/skybox/left.jpg",
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/skybox/top.jpg",
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/skybox/bottom.jpg",
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/skybox/front.jpg",
        "C:/Users/TOSHIBA/Documents/ВМК/КГ/Project2/Resorces/skybox/back.jpg"
    };
    unsigned int skybox_texture = get_skybox(skybox_parts);


    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertics), &skybox_vertics, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    Wall .Use();
    Wall.setInt("wall_map_diffuse", 0);
    Wall.setInt("wall_map_normal", 1);
    SkyboxShader.Use();
    SkyboxShader.setInt("skybox", 0);

    RM.Use();
    RM.setInt("diffuseMap", 0);
    RM.setInt("normalMap", 1);
    RM.setInt("depthMap", 2);

    glm::vec3 light_position(0.5f, 1.0f, 0.3f);
    while (!glfwWindowShouldClose(window))//Начинаем игровой цикл
    {
        glfwPollEvents();//Проверка на действия со стороны клавы или мышки
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//Цвет заливки
        //Очистка буфера
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        do_movement();
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection;
        glStencilMask(0x00); //Далее объекты без обводки

        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);  
        SkyboxShader.Use();
        view = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp))); // remove translation from the view matrix
        SkyboxShader.setMat4("view", view);
        SkyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);

        //Рисуем стену с Relief mapping
        projection = glm::mat4(1.0f);
        projection = glm::perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        RM.Use();
        RM.setMat4("projection", projection);
        RM.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 1.0f, 1.0f));
        //model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show parallax mapping from multiple directions
        RM.setMat4("model", model);
        RM.setVec3("viewPos", cameraPos);
        RM.setVec3("lightPos", light_position);
        RM.setFloat("heightScale", heightScale); 
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, RM_map_diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, RM_map_normal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, RM_map_depth);
        render();

        //Рисуем стену с normal mapping
        projection = glm::mat4(1.0f);
        view = glm::mat4(1.0f);
        projection = glm::perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        Wall.Use();
        Wall.setMat4("projection", projection);
        Wall.setMat4("view", view);
        model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
        model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show normal mapping from multiple directions
        Wall.setMat4("model", model);
        Wall.setVec3("viewPos", cameraPos);
        Wall.setVec3("lightPos", light_position);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wall_map_diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, wall_map_normal);
        render();

        glStencilFunc(GL_ALWAYS, 1, 0xFF);//Включаем запись
        glStencilMask(0xFF);

        //Рисуем коробку
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_of_box);
        glUniform1i(glGetUniformLocation(Box.Program, "ourTexture1"), 0);
        Box.Use();
        view = glm::mat4(1.0f);
        projection = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(45.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
        GLint modelLoc = glGetUniformLocation(Box.Program, "model");
        GLint viewLoc = glGetUniformLocation(Box.Program, "view");
        GLint projLoc = glGetUniformLocation(Box.Program, "projection");   
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(VAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, box_coords[0]);           
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00); //Снова отключается запись в трафарет
        glDisable(GL_DEPTH_TEST);
        
        Obvodka.Use();
        float scale = 1.02;
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture_of_box);
        model = glm::mat4(1.0f);
        model = glm::translate(model, box_coords[0]);
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);  
        Obvodka.setMat4("view", view);
        Obvodka.setMat4("projection", projection);
        Obvodka.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
 
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
                   
        glBindVertexArray(0);
        glfwSwapBuffers(window);//Заменяет цветовой буфер
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();//Очистка ресурсов
    return 0;
}

void keybord_reaction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) //Закрытие при esc
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
    GLfloat cameraSpeed = 0.01f;
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(image);
    }
    else
    {
        std::cout << "Error, texture was not loaded: " << std::endl;
        stbi_image_free(image);
    }
    return texture;
}

unsigned int quadVAO1 = 0;
unsigned int quadVBO1;
void render()
{
    if (quadVAO1 == 0)
    {
        glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3(1.0f, -1.0f, 0.0f);
        glm::vec3 pos4(1.0f, 1.0f, 0.0f);
     
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);
     
        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;

        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


        float quadVertices[] = {
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
     
        glGenVertexArrays(1, &quadVAO1);
        glGenBuffers(1, &quadVBO1);
        glBindVertexArray(quadVAO1);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    }
    glBindVertexArray(quadVAO1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
unsigned int get_skybox(std::vector<std::string> skybox_parts)
{
    unsigned int texture; //Создаем объект 
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture); //Точка привязки GL_TEXTURE_CUBE_MAP

    int width, height, nrComponents;
    for (unsigned int i = 0; i < skybox_parts.size(); i++)
    {
        unsigned char* image = stbi_load(skybox_parts[i].c_str(), &width, &height, &nrComponents, 0);
        if (image)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            stbi_image_free(image);
        }
        else
        {
            std::cout << "Error, skybox was not loaded. Part: " << skybox_parts[i] << std::endl;
            stbi_image_free(image);
        }
    }
    //Настройки фильтрации
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return texture;
}

