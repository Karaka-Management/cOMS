/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_OPENGL_UTILS_H
#define TOS_GPUAPI_OPENGL_UTILS_H

#include "../../stdlib/Types.h"
#include "../../memory/RingMemory.h"
#include "../../utils/TestUtils.h"
#include "../../models/Attrib.h"
#include "../../object/Texture.h"

#include "../RenderUtils.h"
#include "Opengl.h"

#ifdef _WIN32
    #include <windows.h>
    #include "../../platform/win32/UtilsWin32.h"
    #include "../../platform/win32/Window.h"
#endif

/*
struct Window {
    bool is_fullscreen;
    int32 width;
    int32 height;
    char name[32];

    int32 x;
    int32 y;

    GLFWwindow* hwnd_lib;

    #ifdef _WIN32
        HWND hwnd;
    #endif
};
*/

/*
inline
void window_create(Window* window, void*)
{
    //GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    window->hwnd_lib = glfwCreateWindow(
        window->width,
        window->height,
        window->name,
        NULL,
        NULL
    );

    ASSERT_SIMPLE(window->hwnd_lib);

    //glfwSetInputMode(window->hwnd_lib, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window->hwnd_lib);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    #if GLFW_EXPOSE_NATIVE_WIN32
        window->hwnd = glfwGetWin32Window(window->hwnd_lib);
    #endif
}

inline
void window_open(Window* window)
{
    glfwMakeContextCurrent(window->hwnd_lib);
    glViewport(window->x, window->y, window->width, window->height);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
}

inline
void window_close(Window* window)
{
    glfwWindowShouldClose(window->hwnd_lib);
}
*/

inline
uint32 get_texture_data_type(uint32 texture_data_type)
{
    switch (texture_data_type) {
        case TEXTURE_DATA_TYPE_2D: {
            return GL_TEXTURE_2D;
        }
        case TEXTURE_DATA_TYPE_1D: {
            return GL_TEXTURE_1D;
        }
        case TEXTURE_DATA_TYPE_3D: {
            return GL_TEXTURE_3D;
        }
        case TEXTURE_DATA_TYPE_1D_ARRAY: {
            return GL_TEXTURE_1D_ARRAY;
        }
        case TEXTURE_DATA_TYPE_2D_ARRAY: {
            return GL_TEXTURE_2D_ARRAY;
        }
        case TEXTURE_DATA_TYPE_2D_MULTISAMPLED: {
            return GL_TEXTURE_2D_MULTISAMPLE;
        }
        case TEXTURE_DATA_TYPE_2D_MULTISAMPLED_ARRAY: {
            return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
        }
        default: {
            return GL_TEXTURE_2D;
        }
    }
}

// 1. prepare_texture
// 2. define wrap
// 3. define filter
// 4. load_texture_to_gpu

inline
void prepare_texture(OpenGL* gl, Texture* texture, uint32 texture_unit)
{
    uint32 texture_data_type = get_texture_data_type(texture->texture_data_type);

    glGenTextures(1, (GLuint *) &texture->id);
    gl->glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(texture_data_type, (GLuint) texture->id);
}

inline
void load_texture_to_gpu(OpenGL* gl, const Texture* texture, int mipmap_level = 0)
{
    uint32 texture_data_type = get_texture_data_type(texture->texture_data_type);
    glTexImage2D(
        texture_data_type, mipmap_level, GL_RGBA,
        texture->image.width, texture->image.height,
        0, GL_RGBA, GL_UNSIGNED_BYTE,
        texture->image.pixels
    );

    if (mipmap_level > -1) {
        gl->glGenerateMipmap(GL_TEXTURE_2D);
    }
}

inline
void texture_use(OpenGL* gl, const Texture* texture, uint32 texture_unit)
{
    gl->glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, (GLuint) texture->id);
}

GLuint shader_make(OpenGL* gl, GLenum type, const char *source, RingMemory* ring)
{
    GLuint shader = gl->glCreateShader(type);
    gl->glShaderSource(shader, 1, (GLchar **) &source, NULL);
    gl->glCompileShader(shader);

    GLint status;
    gl->glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint length;
        gl->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        GLchar *info = (GLchar *) ring_get_memory(ring, length * sizeof(GLchar));

        gl->glGetShaderInfoLog(shader, length, NULL, info);

        ASSERT_SIMPLE(false);

        // @todo log
    }

    return shader;
}

GLuint shader_load(OpenGL* gl, GLenum type, const char* path, RingMemory* ring) {
    uint64 temp = ring->pos;

    FileBody file;

    // @todo consider to accept file as parameter and load file before
    file_read(path, &file, ring);
    GLuint result = shader_make(gl, type, (const char *) file.content, ring);

    // We can immediately dispose of it we can also reset our ring memory position
    ring->pos = temp;

    return result;
}

GLuint program_make(
    OpenGL* gl,
    GLuint vertex_shader,
    GLuint fragment_shader,
    GLint geometry_shader,
    RingMemory* ring
) {
    GLuint program = gl->glCreateProgram();

    if (geometry_shader > -1) {
        gl->glAttachShader(program, geometry_shader);
    }

    gl->glAttachShader(program, vertex_shader);
    gl->glAttachShader(program, fragment_shader);
    gl->glLinkProgram(program);

    GLint status;
    gl->glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        ASSERT_SIMPLE(false);

        GLint length;
        gl->glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        GLchar *info = (GLchar *) ring_get_memory(ring, length * sizeof(GLchar));

        gl->glGetProgramInfoLog(program, length, NULL, info);

        // @todo use global logger
        fprintf(stderr, "glLinkProgram failed: %s\n", info);
    }

    // @question really?
    if (geometry_shader > -1) {
        gl->glDetachShader(program, geometry_shader);
    }

    gl->glDetachShader(program, vertex_shader);
    gl->glDetachShader(program, fragment_shader);

    // @question really?
    if (geometry_shader > -1) {
        gl->glDeleteShader(geometry_shader);
    }

    gl->glDeleteShader(vertex_shader);
    gl->glDeleteShader(fragment_shader);

    return program;
}

GLuint program_load(
    OpenGL* gl,
    const char* path1,
    const char* path2,
    const char* path3,
    RingMemory* ring
) {
    GLuint vertex_shader = shader_load(gl, GL_VERTEX_SHADER, path1, ring);
    GLuint fragment_shader = shader_load(gl, GL_FRAGMENT_SHADER, path2, ring);

    GLint geometry_shader = -1;
    if (path3) {
        geometry_shader = shader_load(gl, GL_GEOMETRY_SHADER, path3, ring);
    }

    GLuint program = program_make(gl, vertex_shader, fragment_shader, geometry_shader, ring);

    return program;
}

inline
void shader_use(OpenGL* gl, uint32 id)
{
    gl->glUseProgram(id);
}

inline
void draw_triangles_3d(OpenGL* gl, VertexRef* vertices, GLuint buffer, int count) {
    gl->glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    gl->glVertexAttribPointer(vertices->position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) 0);
    gl->glEnableVertexAttribArray(vertices->position);

    // normal attribute
    gl->glVertexAttribPointer(vertices->normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(float) * 3));
    gl->glEnableVertexAttribArray(vertices->normal);

    // texture coord attribute
    gl->glVertexAttribIPointer(vertices->tex_coord, 2, GL_UNSIGNED_INT, sizeof(Vertex3D), (void *) (sizeof(float) * 6));
    gl->glEnableVertexAttribArray(vertices->tex_coord);

    // color attribute
    gl->glVertexAttribPointer(vertices->color, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(float) * 8));
    gl->glEnableVertexAttribArray(vertices->color);

    glDrawArrays(GL_TRIANGLES, 0, count);

    gl->glDisableVertexAttribArray(vertices->position);
    gl->glDisableVertexAttribArray(vertices->normal);
    gl->glDisableVertexAttribArray(vertices->tex_coord);
    gl->glDisableVertexAttribArray(vertices->color);

    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_3d_textureless(OpenGL* gl, VertexRef* vertices, GLuint buffer, int count) {
    gl->glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    gl->glVertexAttribPointer(vertices->position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) 0);
    gl->glEnableVertexAttribArray(vertices->position);

    // normal attribute
    gl->glVertexAttribPointer(vertices->normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(float) * 3));
    gl->glEnableVertexAttribArray(vertices->normal);

    // color attribute
    gl->glVertexAttribPointer(vertices->color, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(float) * 8));
    gl->glEnableVertexAttribArray(vertices->color);

    glDrawArrays(GL_TRIANGLES, 0, count);

    gl->glDisableVertexAttribArray(vertices->position);
    gl->glDisableVertexAttribArray(vertices->normal);
    gl->glDisableVertexAttribArray(vertices->color);

    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_3d_colorless(OpenGL* gl, VertexRef* vertices, GLuint buffer, int count) {
    gl->glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    gl->glVertexAttribPointer(vertices->position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) 0);
    gl->glEnableVertexAttribArray(vertices->position);

    // normal attribute
    gl->glVertexAttribPointer(vertices->normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(float) * 3));
    gl->glEnableVertexAttribArray(vertices->normal);

    // texture coord attribute
    gl->glVertexAttribIPointer(vertices->tex_coord, 2, GL_UNSIGNED_INT, sizeof(Vertex3D), (void *) (sizeof(float) * 6));
    gl->glEnableVertexAttribArray(vertices->tex_coord);

    glDrawArrays(GL_TRIANGLES, 0, count);

    gl->glDisableVertexAttribArray(vertices->position);
    gl->glDisableVertexAttribArray(vertices->normal);
    gl->glDisableVertexAttribArray(vertices->tex_coord);

    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_2d(OpenGL* gl, VertexRef* vertices, GLuint buffer, int count) {
    gl->glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    gl->glVertexAttribPointer(vertices->position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) 0);
    gl->glEnableVertexAttribArray(vertices->position);

    // texture coord attribute
    gl->glVertexAttribIPointer(vertices->tex_coord, 2, GL_UNSIGNED_INT, sizeof(Vertex2D), (void *) (sizeof(float) * 2));
    gl->glEnableVertexAttribArray(vertices->tex_coord);

    // color attribute
    gl->glVertexAttribPointer(vertices->color, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) (sizeof(float) * 4));
    gl->glEnableVertexAttribArray(vertices->color);

    glDrawArrays(GL_TRIANGLES, 0, count);

    gl->glDisableVertexAttribArray(vertices->position);
    gl->glDisableVertexAttribArray(vertices->tex_coord);
    gl->glDisableVertexAttribArray(vertices->color);

    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_2d_textureless(OpenGL* gl, VertexRef* vertices, GLuint buffer, int count) {
    gl->glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    gl->glVertexAttribPointer(vertices->position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) 0);
    gl->glEnableVertexAttribArray(vertices->position);

    // color attribute
    gl->glVertexAttribPointer(vertices->color, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) (sizeof(float) * 4));
    gl->glEnableVertexAttribArray(vertices->color);

    glDrawArrays(GL_TRIANGLES, 0, count);

    gl->glDisableVertexAttribArray(vertices->position);
    gl->glDisableVertexAttribArray(vertices->color);

    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_2d_colorless(OpenGL* gl, VertexRef* vertices, GLuint buffer, int count) {
    gl->glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    gl->glVertexAttribPointer(vertices->position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) 0);
    gl->glEnableVertexAttribArray(vertices->position);

    // texture coord attribute
    gl->glVertexAttribIPointer(vertices->tex_coord, 2, GL_UNSIGNED_INT, sizeof(Vertex2D), (void *) (sizeof(float) * 2));
    gl->glEnableVertexAttribArray(vertices->tex_coord);

    glDrawArrays(GL_TRIANGLES, 0, count);

    gl->glDisableVertexAttribArray(vertices->position);
    gl->glDisableVertexAttribArray(vertices->tex_coord);

    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_text(OpenGL* gl, VertexRef* vertices, GLuint buffer, int length)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    draw_triangles_2d(gl, vertices, buffer, length * 6);
    glDisable(GL_BLEND);
}

GLuint gen_text_buffer(float x, float y, float n, const char *text) {
    size_t length = strlen(text);
    GLfloat *data = NULL; //malloc_faces(4, length); // sizeof(GLfloat) * 6 * 4 * length

    for (int i = 0; i < length; i++) {
        make_character(data + i * 24, x, y, n / 2, n, text[i]);
        x += n;
    }

    return 0; //gen_faces(4, length, data);
}

inline
void render_text(OpenGL* gl, Attrib* attrib, int justify, float x, float y, float n, const char *text)
{
    float matrix[16];
    //set_matrix_2d(matrix, g->width, g->height);

    gl->glUseProgram(attrib->program);
    gl->glUniformMatrix4fv(attrib->matrix, 1, GL_FALSE, matrix);
    gl->glUniform1i(attrib->sampler, 1);
    gl->glUniform1i(attrib->extra1, 0);

    size_t length = strlen(text);
    x -= n * justify * (length - 1) / 2;

    GLuint buffer = gen_text_buffer(x, y, n, text);
    draw_text(gl, &attrib->vertices, buffer, (int) length);

    gl->glDeleteBuffers(1, &buffer);
}

inline
int calculate_face_size(int components, int faces)
{
    return sizeof(GLfloat) * 6 * components * faces;
}

// generates faces
// data is no longer needed after this
inline
uint32 gpuapi_buffer_generate(OpenGL* gl, int size, void* data)
{
    uint32 vbo;

    gl->glGenBuffers(1, &vbo);
    gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    gl->glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return vbo;
}

inline
uint32 gpuapi_shaderbuffer_generate(OpenGL* gl, int size, void* data)
{
    uint32 sbo;

    gl->glGenBuffers(1, &sbo);
    gl->glBindBuffer(GL_SHADER_STORAGE_BUFFER, sbo);
    gl->glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);

    return sbo;
}

inline
uint32 gpuapi_uniformbuffer_generate(OpenGL* gl, int size, void* data)
{
    uint32 ubo;

    gl->glGenBuffers(1, &ubo);
    gl->glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    gl->glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);

    return ubo;
}

inline
uint32 gpuapi_buffer_element_generate(OpenGL* gl, int size, uint32 *data)
{
    uint32 ebo;

    gl->glGenBuffers(1, &ebo);
    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return ebo;
}

inline
uint32 gpuapi_vertex_array_generate(OpenGL* gl)
{
    uint32 vao;
    gl->glGenVertexArrays(1, &vao);
    gl->glBindVertexArray(vao);

    return vao;
}

inline
void gpuapi_unbind_all(OpenGL* gl)
{
    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl->glBindVertexArray(0);
}

inline
void gpuapi_buffer_delete(OpenGL* gl, GLuint buffer)
{
    gl->glDeleteBuffers(1, &buffer);
}

int get_gpu_free_memory()
{
    GLint available = 0;
    glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available);

    if (available != 0) {
        return available;
    }

    glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, &available);

    return available;
}

/*
void render_9_patch(GLuint texture,
    int imgWidth, int imgHeight,
    int img_x1, int img_x2,
    int img_y1, int img_y2,
    int renderWidth, int renderHeight,
    int repeat
)
{

}
*/

#endif