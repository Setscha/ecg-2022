#pragma once

#include "Utils.h"
#include <sstream>
#include <fstream>
#include <unordered_map>

class Shader {
private:
    GLuint shader;
    std::unordered_map<std::string, GLint> uniformLocationMap;
public:
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();
    GLuint compileShader(GLuint type, const std::string& path) const;
    void activate() const;
    void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
    void setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, glm::mat4 matrix);
    void setUniformMatrix4fv(const std::string& name, int size, GLboolean transposed, GLfloat* matrix);
    GLint getUniformLocation(const std::string& name);
};