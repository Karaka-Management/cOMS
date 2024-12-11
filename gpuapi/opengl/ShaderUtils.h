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
#include "Opengl.h"

// Set value based on shader uniform name
inline
void shader_set_value(uint32 id, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(id, name), (int32) value);
}

inline
void shader_set_value(uint32 id, const char* name, int32 value)
{
    glUniform1i(glGetUniformLocation(id, name), value);
}

inline
void shader_set_value(uint32 id, const char* name, f32 value)
{
    glUniform1f(glGetUniformLocation(id, name), value);
}

inline
void shader_set_v2(uint32 id, const char* name, const f32* value)
{
    glUniform2fv(glGetUniformLocation(id, name), 1, value);
}

inline
void shader_set_v3(uint32 id, const char* name, const f32* value)
{
    glUniform3fv(glGetUniformLocation(id, name), 1, value);
}

inline
void shader_set_v4(uint32 id, const char* name, const f32* value)
{
    glUniform4fv(glGetUniformLocation(id, name), 1, value);
}

inline
void shader_set_m2(uint32 id, const char* name, const f32* value)
{
    glUniformMatrix2fv(glGetUniformLocation(id, name), 1, GL_FALSE, value);
}

inline
void shader_set_m3(uint32 id, const char* name, const f32* value)
{
    glUniformMatrix3fv(glGetUniformLocation(id, name), 1, GL_FALSE, value);
}

inline
void shader_set_m4(uint32 id, const char* name, const f32* value)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, value);
}

// Set value based on uniform location
inline
void shader_set_value(uint32 location, bool value)
{
    glUniform1i(location, (int32) value);
}

inline
void shader_set_value(uint32 location, int32 value)
{
    glUniform1i(location, value);
}

inline
void shader_set_value(uint32 location, f32 value)
{
    glUniform1f(location, value);
}

inline
void shader_set_v2(uint32 location, const f32* value)
{
    glUniform2fv(location, 1, value);
}

inline
void shader_set_v3(uint32 location, const f32* value)
{
    glUniform3fv(location, 1, value);
}

inline
void shader_set_v4(uint32 location, const f32* value)
{
    glUniform4fv(location, 1, value);
}

inline
void shader_set_m2(uint32 location, const f32* value)
{
    glUniformMatrix2fv(location, 1, GL_FALSE, value);
}

inline
void shader_set_m3(uint32 location, const f32* value)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, value);
}

inline
void shader_set_m4(uint32 location, const f32* value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

inline
uint32 shader_get_attrib_location(uint32 id, const char* name)
{
    // By using this you can retreive the shader variable name at a point where and when you know it
    // BUT set values later on in generalized functions without knowing the shader variable name
    // Basically like pointers
    return glGetAttribLocation(id, name);
}

inline
uint32 shader_get_uniform_location(uint32 id, const char* name)
{
    // By using this you can retreive the shader variable name at a point where and when you know it
    // BUT set values later on in generalized functions without knowing the shader variable name
    // Basically like pointers
    return glGetUniformLocation(id, name);
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

int32 shader_program_optimize(const char* input, char* output)
{
    const char* read_ptr = input;
    char* write_ptr = output;
    bool in_string = false;

    while (*read_ptr) {
        // Remove leading whitespace
        while (*read_ptr == ' ' || *read_ptr == '\t' || is_eol(read_ptr)) {
            ++read_ptr;
        }

        if (write_ptr != output
            && *(write_ptr - 1) != '\n' && *(write_ptr - 1) != ';' && *(write_ptr - 1) != '{'
            && *(write_ptr - 1) != '('
            && *(write_ptr - 1) != ','
        ) {
            *write_ptr++ = '\n';
        }

        // Handle single-line comments (//)
        if (*read_ptr == '/' && *(read_ptr + 1) == '/' && !in_string) {
            // Go to end of line
            while (*read_ptr && *read_ptr != '\n') {
                ++read_ptr;
            }

            continue;
        }

        // Handle multi-line comments (/* */)
        if (*read_ptr == '/' && *(read_ptr + 1) == '*' && !in_string) {
            // Go to end of comment
            while (*read_ptr && (*read_ptr != '*' || *(read_ptr + 1) != '/')) {
                ++read_ptr;
            }

            if (*read_ptr == '*' && *(read_ptr + 1) == '/') {
                read_ptr += 2;
            }

            continue;
        }

        // Handle strings to avoid removing content within them
        if (*read_ptr == '"') {
            in_string = !in_string;
        }

        // Copy valid characters to write_ptr
        while (*read_ptr && !is_eol(read_ptr) && *read_ptr != '"'
            && !(*read_ptr == '/' && (*(read_ptr + 1) == '/' || *(read_ptr + 1) == '*'))
        ) {
            if (!in_string
                && (*read_ptr == '*' || *read_ptr == '/' || *read_ptr == '=' || *read_ptr == '+' || *read_ptr == '-' || *read_ptr == '%'
                    || *read_ptr == '(' || *read_ptr == ')'
                    || *read_ptr == '{' || *read_ptr == '}'
                    || *read_ptr == ',' || *read_ptr == '?' || *read_ptr == ':' || *read_ptr == ';'
                    || *read_ptr == '&' || *read_ptr == '|'
                    || *read_ptr == '>' || *read_ptr == '<'
                )
            ) {
                if (is_whitespace(*(write_ptr - 1)) || *(write_ptr - 1) == '\n') {
                    --write_ptr;
                }

                *write_ptr++ = *read_ptr++;

                if (*read_ptr && is_whitespace(*read_ptr)) {
                    ++read_ptr;
                }
            } else {
                *write_ptr++ = *read_ptr++;
            }
        }
    }

    *write_ptr = '\0';

    // -1 to remove \0 from length, same as strlen
    return (int32) (write_ptr - output);
}

#endif