#include "Shaders.h"

bool Render::ProgramShader::createShader(const std::string &src, const GLenum shaderType, GLuint &shader_id)
{
    shader_id = glCreateShader(shaderType);
    const char *temp = src.c_str();
    glShaderSource(shader_id, 1, &temp, nullptr);
    glCompileShader(shader_id);

    GLint success{0};
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024] = {0};
        glGetShaderInfoLog(shader_id, 1024, nullptr, infoLog);
        std::cerr << "ERROR: SHADER COMPILE-TIME err:\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

Render::ProgramShader::ProgramShader(const std::string &ver_shader, const std::string &fr_shader)
{
    GLuint vsID = 0, frID = 0;
    if (!createShader(ver_shader, GL_VERTEX_SHADER, vsID))
    {
        std::cerr << "ERROR: VERTEX SHADER COMPILE-TIME\n";
        return;
    }

    if (!createShader(fr_shader, GL_FRAGMENT_SHADER, frID))
    {
        std::cerr << "ERROR: FRAGMENT SHADER COMPILE-TIME\n";
        glDeleteShader(vsID);
        return;
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vsID);
    glAttachShader(m_id, frID);
    glLinkProgram(m_id);

    GLint success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024] = {0};
        glGetShaderInfoLog(m_id, 1024, nullptr, infoLog);
        std::cerr << "ERROR: PROGRAM LINK-TIME err:\n" << infoLog << std::endl;
        return;
    }
    else
    {
        is_Compiled = true;
    }

    glDeleteShader(vsID);
    glDeleteShader(frID);
}

Render::ProgramShader::~ProgramShader()
{
    glDeleteProgram(m_id);
}

bool Render::ProgramShader::usage() const
{
    bool res = false;
    if (m_id)
    {
        res = true;
        glUseProgram(m_id);
    }
    return res;
}

void Render::ProgramShader::setInt(const std::string &name, const GLint value)
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Render::ProgramShader::setMat4(const std::string &name, glm::mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

GLuint Render::ProgramShader::get_id() const
{
    return m_id;
}

Render::ProgramShader::ProgramShader(ProgramShader &&right) noexcept
{
    glDeleteProgram(m_id);
    m_id = right.m_id;
    is_Compiled = right.is_Compiled;

    right.m_id = 0;
    right.is_Compiled = false;
}

Render::ProgramShader &Render::ProgramShader::operator=(ProgramShader &&right) noexcept
{
    m_id = right.m_id;
    is_Compiled = right.is_Compiled;

    right.m_id = 0;
    right.is_Compiled = false;
    return *this;
}