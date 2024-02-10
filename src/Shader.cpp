#include "Shader.hpp"

#include "glad/gl.h"

#include <exception>
#include <sstream>

#include <iostream>

uint32_t Shader::getID()
{
    if (m_ID)
    {
        return m_ID.value();
    }
    else
    {
        throw shader_existence_error();
    }
}

void Shader::compileFromPath(const std::string& vertexPath, const std::string& fragmentPath)
{
    const std::string vertexSource = readContentsOfFile(vertexPath);
    const std::string fragmentSource = readContentsOfFile(fragmentPath);

    compileFromSource(vertexSource, fragmentSource);
}

void Shader::compileFromSource(const std::string& vertexSource, const std::string& fragmentSource)
{
    uint32_t sVertex;
    uint32_t sFragment;

    sVertex = glCreateShader(GL_VERTEX_SHADER);
    const char* vSource = vertexSource.c_str();
    glShaderSource(sVertex, 1, &vSource, NULL);
    glCompileShader(sVertex);
    checkShaderUnitError(sVertex, "Vertex");

    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSource = fragmentSource.c_str();
    glShaderSource(sFragment, 1, &fSource, NULL);
    glCompileShader(sFragment);
    checkShaderUnitError(sFragment, "Fragment");

    m_ID = glCreateProgram();
    glAttachShader(getID(), sVertex);
    glAttachShader(getID(), sFragment);
    glLinkProgram(getID());
    checkProgramError(getID());

    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
}

void Shader::bind() { glUseProgram(getID()); }

void Shader::unbind() { glUseProgram(0); }

void Shader::setFloat(const std::string& name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setInt(const std::string& name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value)
{
    glUniform2f(getUniformLocation(name), value.x, value.y);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, false, glm::value_ptr(value));
}

int32_t Shader::getUniformLocation(const std::string& name)
{
    return glGetUniformLocation(getID(), name.c_str());
}

void Shader::checkShaderUnitError(uint32_t object, const std::string& unitType)
{
    int32_t success;
    char infoLog[1024];
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(object, 1024, NULL, infoLog);
        std::cerr << "SHADER:COMPILER_ERROR:" << unitType << "\n" << infoLog << "\n";
        throw std::runtime_error("Failed to compile unit of shader");
    }
}

void Shader::checkProgramError(uint32_t object)
{
    int32_t success;
    char infoLog[1024];

    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(object, 1024, NULL, infoLog);
        std::cerr << "SHADER:LINK_ERROR:\n" << infoLog << "\n";
        throw std::runtime_error("Failed to compile Shader");
    }
}

const std::string Shader::readContentsOfFile(const std::string& filename)
{
    std::string source;

    std::ifstream file;
    file.open(filename, std::ios::in);
    if (file.is_open())
    {
        std::stringstream stream;

        stream << file.rdbuf();
        file.close();

        source = stream.str();

        return source;
    }
    else
    {
        throw std::runtime_error("Failed to open " + std::string(filename));
    }
}
