#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    GLuint Program;
    Shader(const GLchar* name_shader1, const GLchar* name_shader2)
    {
        std::string code_vertex;
        std::string code_fragment;
        std::ifstream vertex_file;
        std::ifstream fragment_file;

        vertex_file.exceptions(std::ifstream::badbit);
        fragment_file.exceptions(std::ifstream::badbit);
        try//Создаем строчки из потока файлов
        {
            vertex_file.open(name_shader1);
            fragment_file.open(name_shader2);
            std::stringstream vertex_stream, fragment_stream;
            vertex_stream << vertex_file.rdbuf();
            fragment_stream << fragment_file.rdbuf();
            vertex_file.close();
            fragment_file.close();
            code_vertex = vertex_stream.str();
            code_fragment = fragment_stream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "Error during reading files" << std::endl;
        }
        const GLchar* vertex_context = code_vertex.c_str();
        const GLchar* fragment_context = code_fragment.c_str();

        GLuint vertex, fragment;
        GLint success;

        vertex = glCreateShader(GL_VERTEX_SHADER); //Создание вершинного шейдера
        glShaderSource(vertex, 1, &vertex_context, NULL);//Привязываем  код к шейдеру
        glCompileShader(vertex);//Компилируем шейдер
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); //Проверка на ошибки компиляции
        if (!success)
            std::cout << "Error in compilation of vertex shader !!!" << std::endl;

        fragment = glCreateShader(GL_FRAGMENT_SHADER); //Создание объекта фрагментного шейдера
        glShaderSource(fragment, 1, &fragment_context, NULL);//Привязываем  код к шейдеру
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
            std::cout << "Error in compilation of fragment shader !!!" << std::endl;

        this->Program = glCreateProgram(); //Создание шейдерной программы
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program); //Связываем шейдеры
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success)
            std::cout << "Error in connection of two shaders" << std::endl;
        glDeleteShader(vertex); //Удаление после связывания
        glDeleteShader(fragment);

    }

    void Use()//Активация шейдерной программы
    {
        glUseProgram(this->Program);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(this->Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(this->Program, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(this->Program, name.c_str()), x, y, z);
    }
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(this->Program, name.c_str()), value);
    }
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(this->Program, name.c_str()), value);
    }
};

#endif
