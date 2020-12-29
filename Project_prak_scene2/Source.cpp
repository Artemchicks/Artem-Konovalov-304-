#include <iostream>
#include <stb_image.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
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
void to_tanget_and_render();
unsigned int get_skybox(std::vector<std::string> faces);

const GLuint SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;	
GLfloat pitch = 0.0f;
GLfloat last_x = SCREEN_WIDTH / 2.0;
GLfloat last_y = SCREEN_HEIGHT / 2.0;
bool keys[1024];
float heightScale = 0.1;
GLfloat deltaTime = 0.0f;	
GLfloat lastFrame = 0.0f;

int main()
{
    glfwInit(); //Инициализация GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//Задаем обрабатываемые версии
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Профайл и контекст
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//Выключаем возможность изменения размера окна
    glfwWindowHint(GLFW_SAMPLES, 4);
    //Создаем объект окна
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scene2", nullptr, nullptr);
    glfwMakeContextCurrent(window);//Создаем контекст окна
    glfwSetKeyCallback(window, keybord_reaction);//Установка реакции на клавиатуру
    glewExperimental = GL_TRUE;//Подключение каких-то важных техник для GLEW
    glewInit(); //Инициализация GLEW   
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);//Устанавливаем размер окна
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_TEST);//Включить трафаретное тестирование
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//При провале одного из тестов текущее значение остается в случае успеха заменяем
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    Shader Box("box1.vs", "box2.vs");
    Shader Obvodka("DrawColor1.vs", "DrawColor2.vs");
    Shader Wall("shader1.vs", "shader2.vs");
    Shader Lamp("lamp1.vs", "lamp2.vs");
    Shader SkyboxShader("sky1.vs", "sky2.vs");
    Shader RM("PM1.vs",  "PM2.vs");
    
float box_vertics[] = {
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
    glm::vec3 lamp_positions[] = {
       glm::vec3(0.7f,  0.2f,  2.0f),
       glm::vec3(0.0f,  0.2f,  -2.0f),
       glm::vec3(1.3f,  -0.1f,  0.0f),
       glm::vec3(-1.7f,  0.0f,  0.0f)
    };
    
    GLuint VBO, VAO, EBO;//VBO - объект вершинного буфера, 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);//Создаем VBO
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//Привязка к буферу
    glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertics), box_vertics, GL_STATIC_DRAW);//Копироввание вершин в буфер
    glEnableVertexAttribArray(0);//Интерпретация вершинных данных и включение атрибута
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertics), &skybox_vertics, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //Текстура коробки
    GLuint texture_of_box = get_texture("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/container.jpg");
    //Загружаем текстуры стены
    unsigned int wall_map_diffuse = get_texture("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/wall1.jpg");
    unsigned int wall_map_normal = get_texture("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/wall2.jpg");
    //Текстуры для RM
    unsigned int RM_map_diffuse = get_texture("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/wall_real.jpg");
    unsigned int RM_map_normal = get_texture("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/wall_normal.jpg");
    unsigned int RM_map_depth = get_texture("C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/wall_disp.jpg");
    //Скайбокс
    std::vector<std::string> skybox_parts
    {
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/right.jpg",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/left.jpg",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/top.jpg",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/bottom.jpg",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/front.jpg",
        "C:/Users/Artem/Documents/ВМК/3 курс/КГ/Project_prak/Resources/skybox/back.jpg"
    };
    unsigned int skybox_texture = get_skybox(skybox_parts);

    Wall.Use();
    Wall.setInt("wall_map_diffuse", 0);
    Wall.setInt("wall_map_normal", 1);
    SkyboxShader.Use();
    SkyboxShader.setInt("skybox", 0);
    RM.Use();
    RM.setInt("diffuseMap", 0);
    RM.setInt("normalMap", 1);
    RM.setInt("depthMap", 2);
    Box.Use();
    Box.setInt("skybox", 0);

    glm::vec3 light_position(-2.0f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(window))//Начинаем игровой цикл
    {
        glfwPollEvents();//Проверка на действия со стороны клавы или мышки
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//Цвет заливки
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//Очистка буфера       
        do_movement();

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection;

        glStencilMask(0x00); //Далее объекты без обводки

        Box.Use();
        model = glm::translate(model, glm::vec3(-5.0, 0.0, 0.0));
        Box.setMat4("model", model);
        Box.setMat4("view", view);
        Box.setMat4("projection", projection);
        Box.setVec3("cameraPos", camera_position);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);  
        SkyboxShader.Use();
        view = glm::mat4(glm::mat3(glm::lookAt(camera_position, camera_position + cameraFront, camera_up)));
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
        view = glm::lookAt(camera_position, camera_position + cameraFront, camera_up);
        RM.Use();
        RM.setMat4("projection", projection);
        RM.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); 
        RM.setMat4("model", model);
        RM.setVec3("viewPos", camera_position);
        RM.setVec3("lightPos", light_position);
        RM.setFloat("heightScale", heightScale); 
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, RM_map_diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, RM_map_normal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, RM_map_depth);
        to_tanget_and_render();

        //Рисуем стену с normal mapping
        projection = glm::mat4(1.0f);
        view = glm::mat4(1.0f);
        projection = glm::perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        view = glm::lookAt(camera_position, camera_position + cameraFront, camera_up);
        Wall.Use();
        Wall.setMat4("projection", projection);
        Wall.setMat4("view", view);
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 1.0f));
        //model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); 
        Wall.setMat4("model", model);
        Wall.setVec3("viewPos", camera_position);
        Wall.setVec3("lightPos", light_position);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wall_map_diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, wall_map_normal);
        to_tanget_and_render();

        glStencilFunc(GL_ALWAYS, 1, 0xFF);//Включаем запись, каждый фрагмент контейнера  обновит буфер
        glStencilMask(0xFF);

        //Рисуем коробку
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_of_box);
        glUniform1i(glGetUniformLocation(Box.Program, "ourTexture1"), 0);
        Box.Use();
        view = glm::mat4(1.0f);
        projection = glm::mat4(1.0f);
        view = glm::lookAt(camera_position, camera_position + cameraFront, camera_up);
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

        Obvodka.Use();
        float scale = 1.01;
        glBindVertexArray(VAO);
       // glBindTexture(GL_TEXTURE_2D, texture_of_box);
        model = glm::mat4(1.0f);
        model = glm::translate(model, box_coords[0]);
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        view = glm::lookAt(camera_position, camera_position + cameraFront, camera_up);
        projection = glm::perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);  
        Obvodka.setMat4("view", view);
        Obvodka.setMat4("projection", projection);
        Obvodka.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
 
        glStencilMask(0xFF);
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
        camera_position += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        camera_position -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        camera_position -= glm::normalize(glm::cross(cameraFront, camera_up)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        camera_position += glm::normalize(glm::cross(cameraFront, camera_up)) * cameraSpeed;
}
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double mouse_current_x, double mouse_current_y)
{
    if (firstMouse)
    {
        last_x = mouse_current_x;
        last_y = mouse_current_y;
        firstMouse = false;
    }
    //Считаем смещение
    GLfloat mouse_offset_x = mouse_current_x - last_x;
    GLfloat mouse_offset_y = last_y - mouse_current_y; 
    last_x = mouse_current_x;
    last_y = mouse_current_y;
    //Иначе движения будут слишком резкими
    GLfloat sensitivity = 0.05;	
    mouse_offset_x *= sensitivity;
    mouse_offset_y *= sensitivity;

    yaw += mouse_offset_x;
    pitch += mouse_offset_y;
   
    //Ограничиваем пользователя в вертикальной плоскости 
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    //Результирующий вектор напрвления камеры
    glm::vec3 final_vector;
    final_vector.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    final_vector.y = sin(glm::radians(pitch));
    final_vector.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(final_vector);
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

//Расчет касательного пространства
unsigned int mappingVAO = 0;
unsigned int mappingVBO;
void to_tanget_and_render()
{
    if (mappingVAO == 0)
    {
        glm::vec3 position1(-1.0f, 1.0f, 0.0f); //Координаты вершин
        glm::vec3 position2(-1.0f, -1.0f, 0.0f);
        glm::vec3 position3(1.0f, -1.0f, 0.0f);
        glm::vec3 position4(1.0f, 1.0f, 0.0f);
     
        glm::vec2 uv1(0.0f, 1.0f); // Текстурные координаты
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);
     
        glm::vec3 normal(0.0f, 0.0f, 1.0f); //Вектор нормали

        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;

        glm::vec3 edge1 = position2 - position1; //Грани треугольника
        glm::vec3 edge2 = position3 - position1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        
        edge1 = position3 - position1;
        edge2 = position4 - position1;
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
            position1.x, position1.y, position1.z, normal.x, normal.y, normal.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            position2.x, position2.y, position2.z, normal.x, normal.y, normal.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            position3.x, position3.y, position3.z, normal.x, normal.y, normal.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            position1.x, position1.y, position1.z, normal.x, normal.y, normal.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            position3.x, position3.y, position3.z, normal.x, normal.y, normal.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            position4.x, position4.y, position4.z, normal.x, normal.y, normal.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
     
        glGenVertexArrays(1, &mappingVAO);
        glGenBuffers(1, &mappingVBO);
        glBindVertexArray(mappingVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mappingVBO);
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
    glBindVertexArray(mappingVAO);
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

