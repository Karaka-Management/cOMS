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
#include "../../memory/RingMemory.h"
#include "../../log/Log.h"
#include "Opengl.h"
#include "../ShaderType.h"

int32 shader_type_index(ShaderType type)
{
    switch (type) {
        case SHADER_TYPE_VERTEX:
            return GL_VERTEX_SHADER;
        case SHADER_TYPE_FRAGMENT:
            return GL_FRAGMENT_SHADER;
        default:
            return 0;
    }
}

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

GLuint shader_make(GLenum type, const char* source, RingMemory* ring)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (GLchar **) &source, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    #if DEBUG || INTERNAL
        if (status == GL_FALSE) {
            GLint length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            GLchar *info = (GLchar *) ring_get_memory(ring, length * sizeof(GLchar));

            glGetShaderInfoLog(shader, length, NULL, info);
            LOG(true, info);

            ASSERT_SIMPLE(false);
        }
    #endif

    return shader;
}

inline
int32 program_get_size(uint32 program)
{
    int32 size;
    glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &size);

    return size;
}

GLuint program_make(
    GLuint vertex_shader,
    GLuint fragment_shader,
    GLint geometry_shader,
    RingMemory* ring
) {
    GLuint program = glCreateProgram();

    if (geometry_shader > -1) {
        glAttachShader(program, geometry_shader);
    }

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glProgramParameteri(program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    #if DEBUG || INTERNAL
        if (status == GL_FALSE) {
            GLint length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

            GLchar *info = (GLchar *) ring_get_memory(ring, length * sizeof(GLchar));

            glGetProgramInfoLog(program, length, NULL, info);
            LOG(true, info);

            ASSERT_SIMPLE(false);
        }
    #endif

    // @question really?
    if (geometry_shader > -1) {
        glDetachShader(program, geometry_shader);
    }

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    // @question really?
    if (geometry_shader > -1) {
        glDeleteShader(geometry_shader);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

// @question Depending on how the different gpu apis work we may want to pass Shader* to have a uniform structure
inline
void pipeline_use(uint32 id)
{
    glUseProgram(id);
}

#endif