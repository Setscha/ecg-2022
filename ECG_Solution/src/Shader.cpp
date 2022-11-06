#include "Shader.h"

void Shader::activate() const {
    glUseProgram(shader);
}

Shader::~Shader() {
    glDeleteProgram(shader);
}

Shader::Shader(const string &vertexShaderPath, const string &fragmentShaderPath) {
    shader = glCreateProgram();

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderPath);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);
    glValidateProgram(shader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint Shader::compileShader(GLuint type, const std::string& path) const {
    std::ifstream stream(path);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string source = buffer.str();

    GLuint shaderId = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    GLint succeeded;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &succeeded);

    if (succeeded == GL_FALSE) {
        //Log auslesen und ausgeben
        GLint logSize;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
        char* message = new char[logSize];
        glGetShaderInfoLog(shaderId, logSize, nullptr, message);
        std::cerr << "Error in compileShader for " << path  << ": " << message << std::endl;
        delete[] message;
    }

    return shaderId;
}

void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) {
    activate();
    glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
}

void Shader::setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, glm::mat4 matrix) {
    setUniformMatrix4fv(name, size, transposed, &matrix[0][0]);
}

void Shader::setUniformMatrix4fv(const std::string &name, int size, GLboolean transposed, GLfloat* matrix) {
    activate();
    glUniformMatrix4fv(getUniformLocation(name), size, transposed, matrix);
}

GLint Shader::getUniformLocation(const std::string& name) {
    if (uniformLocationMap.find(name) != uniformLocationMap.end()) {
        return uniformLocationMap[name];
    }
    GLint id = glGetUniformLocation(shader, name.c_str());
    uniformLocationMap[name] = id;
    return id;
}
