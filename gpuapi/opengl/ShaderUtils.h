/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_OPENGL_SHADER_UTILS_H
#define TOS_GPUAPI_OPENGL_SHADER_UTILS_H

#include "../../../EngineDependencies/opengl/glew/include/GL/glew.h"
#include "../../../EngineDependencies/opengl/glfw/include/glfw3.h"

#include "../../stdlib/Types.h"

inline
void shader_set_value(uint32 id, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(id, name), (int) value);
}

inline
void shader_set_value(uint32 id, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(id, name), value);
}

inline
void shader_set_value(uint32 id, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(id, name), value);
}

inline
void shader_set_v2(uint32 id, const char* name, float* value)
{
    glUniform2fv(glGetUniformLocation(id, name), 1, value);
}

inline
void shader_set_v3(uint32 id, const char* name, float* value)
{
    glUniform3fv(glGetUniformLocation(id, name), 1, value);
}

inline
void shader_set_v4(uint32 id, const char* name, float* value)
{
    glUniform4fv(glGetUniformLocation(id, name), 1, value);
}

inline
void shader_set_m2(uint32 id, const char* name, float* value)
{
    glUniformMatrix2fv(glGetUniformLocation(id, name), 1, GL_FALSE, value);
}

inline
void shader_set_m3(uint32 id, const char* name, float* value)
{
    glUniformMatrix3fv(glGetUniformLocation(id, name), 1, GL_FALSE, value);
}

inline
void shader_set_m4(uint32 id, const char* name, float* value)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, value);
}

inline
void shader_check_link_errors(uint32 id, char* log)
{
    GLint success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 1024, NULL, log);
    }
}

inline
void shader_check_compile_errors(uint32 id, char* log)
{
    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 1024, NULL, log);
    }
}

#endif