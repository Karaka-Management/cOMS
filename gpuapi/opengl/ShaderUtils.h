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

#include "../../stdlib/Types.h"
#include "../../math/matrix/MatrixFloat32.h"

inline
void shader_set_value(const OpenGL* gl, uint32 id, const char* name, bool value)
{
    gl->glUniform1i(gl->glGetUniformLocation(id, name), (int) value);
}

inline
void shader_set_value(const OpenGL* gl, uint32 id, const char* name, int value)
{
    gl->glUniform1i(gl->glGetUniformLocation(id, name), value);
}

inline
void shader_set_value(const OpenGL* gl, uint32 id, const char* name, float value)
{
    gl->glUniform1f(gl->glGetUniformLocation(id, name), value);
}

inline
void shader_set_v2(const OpenGL* gl, uint32 id, const char* name, const float* value)
{
    gl->glUniform2fv(gl->glGetUniformLocation(id, name), 1, value);
}

inline
void shader_set_v3(const OpenGL* gl, uint32 id, const char* name, const float* value)
{
    gl->glUniform3fv(gl->glGetUniformLocation(id, name), 1, value);
}

inline
void shader_set_v4(const OpenGL* gl, uint32 id, const char* name, const float* value)
{
    gl->glUniform4fv(gl->glGetUniformLocation(id, name), 1, value);
}

inline
void shader_set_m2(const OpenGL* gl, uint32 id, const char* name, const float* value)
{
    gl->glUniformMatrix2fv(gl->glGetUniformLocation(id, name), 1, GL_FALSE, value);
}

inline
void shader_set_m3(const OpenGL* gl, uint32 id, const char* name, const float* value)
{
    gl->glUniformMatrix3fv(gl->glGetUniformLocation(id, name), 1, GL_FALSE, value);
}

inline
void shader_set_m4(const OpenGL* gl, uint32 id, const char* name, const float* value)
{
    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, name), 1, GL_FALSE, value);
}

inline
void shader_check_link_errors(const OpenGL* gl, uint32 id, char* log)
{
    GLint success;
    gl->glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        gl->glGetProgramInfoLog(id, 1024, NULL, log);
    }
}

inline
void shader_check_compile_errors(const OpenGL* gl, uint32 id, char* log)
{
    GLint success;
    gl->glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        gl->glGetShaderInfoLog(id, 1024, NULL, log);
    }
}

#endif