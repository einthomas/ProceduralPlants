#include "Shader.h"

#include <iostream>
#include <streambuf>
#include <sstream>

Shader::Shader() {}

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) :
    vertexShaderPath(vertexShaderPath), fragmentShaderPath(fragmentShaderPath) {
    compileShader(vertexShaderPath, fragmentShaderPath);
}

void Shader::compileShader(std::string vertexShaderPath, std::string fragmentShaderPath) {
    std::ifstream vertexFile(vertexShaderPath);
    std::string vertexSource((std::istreambuf_iterator<char>(vertexFile)), std::istreambuf_iterator<char>());
    vertexFile.close();

    std::ifstream fragmentFile(fragmentShaderPath);
    std::string fragmentSource((std::istreambuf_iterator<char>(fragmentFile)), std::istreambuf_iterator<char>());
    fragmentFile.close();

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint success;
    GLchar *infoLog;
    GLint infoLogLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    infoLog = new GLchar[infoLogLength];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
        std::cout << "ERROR::SHADERPROGRAM::LINKING\n" << infoLog << std::endl;
    }
    delete[] infoLog;
}

void Shader::use() const {
    glUseProgram(program);
}

void Shader::reload() {
    glDeleteProgram(program);
    compileShader(vertexShaderPath, fragmentShaderPath);
}

GLuint Shader::loadShader(GLuint shaderType, std::string shaderCode) {
    GLuint shader = glCreateShader(shaderType);
    const char *shaderCode_c_str = shaderCode.c_str();
    glShaderSource(shader, 1, &shaderCode_c_str, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::Shader::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

void Shader::setFloat(std::string name, GLfloat value) {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setInteger(std::string name, GLint value) {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVector2f(std::string name, GLfloat x, GLfloat y) {
    glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::setVector2f(std::string name, glm::vec2 value) {
    glUniform2f(glGetUniformLocation(program, name.c_str()), value.x, value.y);
}

void Shader::setVector3f(std::string name, GLfloat x, GLfloat y, GLfloat z) {
    glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setVector3f(std::string name, glm::vec3 value) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z);
}

void Shader::setMatrix4(std::string name, const glm::mat4 &value) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setTexture2D(std::string name, GLenum activeTexture, GLuint texture, GLuint loc) {
    glActiveTexture(activeTexture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program, name.c_str()), loc);
}

void Shader::setTextureCubeMap(std::string name, GLenum activeTexture, GLuint texture, GLuint loc) {
    glActiveTexture(activeTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glUniform1i(glGetUniformLocation(program, name.c_str()), loc);
}
