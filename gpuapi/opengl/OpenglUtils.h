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
#include "../../object/Texture.h"
#include "../../utils/StringUtils.h"
#include "../../log/Log.h"

#include "../RenderUtils.h"
#include "Opengl.h"

#ifdef _WIN32
    #include <windows.h>
    #include "../../platform/win32/UtilsWin32.h"
    #include "../../platform/win32/Window.h"
#endif

inline
void change_viewport(Window* w, int32 offset_x = 0, int32 offset_y = 0)
{
    glViewport(offset_x, offset_y, w->width, w->height);
}

inline
void vsync_set(int32 on)
{
    wglSwapIntervalEXT((int32) on);
}

inline
void wireframe_mode(bool on)
{
    if (on) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

struct OpenglInfo {
    char* renderer;
    int32 major;
    int32 minor;
};

void opengl_info(OpenglInfo* info)
{
    info->renderer = (char *) glGetString(GL_RENDERER);
    info->major = 1;
    info->minor = 0;

    char* version = (char *) glGetString(GL_VERSION);

    for (char *at = version; *at; ++at) {
        if (*at == '.') {
            info->major = str_to_int(version);

            ++at;
            info->minor = str_to_int(at);
            break;
        }
    }
}

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
// 5. texture_use

inline
void prepare_texture(Texture* texture)
{
    uint32 texture_data_type = get_texture_data_type(texture->texture_data_type);

    glGenTextures(1, (GLuint *) &texture->id);
    glActiveTexture(GL_TEXTURE0 + texture->sample_id);
    glBindTexture(texture_data_type, (GLuint) texture->id);
}

inline
void load_texture_to_gpu(const Texture* texture, int32 mipmap_level = 0)
{
    uint32 texture_data_type = get_texture_data_type(texture->texture_data_type);
    glTexImage2D(
        texture_data_type, mipmap_level, GL_RGBA,
        texture->image.width, texture->image.height,
        0, GL_RGBA, GL_UNSIGNED_BYTE,
        texture->image.pixels
    );

    if (mipmap_level > -1) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

inline
void texture_use(const Texture* texture)
{
    glActiveTexture(GL_TEXTURE0 + texture->sample_id);
    glBindTexture(GL_TEXTURE_2D, (GLuint) texture->id);
}

// @todo should be texture_use, the Texture holds information that should make it possible to determine 1D or 2D
inline
void texture_use_1D(const Texture* texture, uint32 texture_unit)
{
    glActiveTexture(GL_TEXTURE0 + texture->sample_id);
    glBindTexture(GL_TEXTURE_1D, (GLuint) texture->id);
}

GLuint shader_make(GLenum type, const char *source, RingMemory* ring)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (GLchar **) &source, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        GLchar *info = (GLchar *) ring_get_memory(ring, length * sizeof(GLchar));

        glGetShaderInfoLog(shader, length, NULL, info);
        LOG(info, true, true);

        ASSERT_SIMPLE(false);
    }

    return shader;
}

inline
GLuint shader_load(GLenum type, const char* path, RingMemory* ring) {
    byte* temp = ring->head;

    FileBody file;

    // @todo consider to accept file as parameter and load file before
    file_read(path, &file, ring);
    GLuint result = shader_make(type, (const char *) file.content, ring);

    // We can immediately dispose of it we can also reset our ring memory position
    ring->head = temp;

    return result;
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

    if (status == GL_FALSE) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        GLchar *info = (GLchar *) ring_get_memory(ring, length * sizeof(GLchar));

        glGetProgramInfoLog(program, length, NULL, info);
        LOG(info, true, true);

        ASSERT_SIMPLE(false);
    }

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

GLuint program_load(
    const char* path1,
    const char* path2,
    const char* path3,
    RingMemory* ring
) {
    GLuint vertex_shader = shader_load(GL_VERTEX_SHADER, path1, ring);
    GLuint fragment_shader = shader_load(GL_FRAGMENT_SHADER, path2, ring);

    GLint geometry_shader = -1;
    if (path3) {
        geometry_shader = shader_load(GL_GEOMETRY_SHADER, path3, ring);
    }

    GLuint program = program_make(vertex_shader, fragment_shader, geometry_shader, ring);

    return program;
}

inline
void shader_use(uint32 id)
{
    glUseProgram(id);
}

inline
void draw_triangles_3d(VertexRef* vertices, GLuint buffer, int32 count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->data_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) 0);
    glEnableVertexAttribArray(vertices->data_id);

    // normal attribute
    glVertexAttribPointer(vertices->normal_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(f32) * 3));
    glEnableVertexAttribArray(vertices->normal_id);

    // texture coord attribute
    // vs glVertexAttribPointer
    glVertexAttribIPointer(vertices->tex_coord_id, 2, GL_UNSIGNED_INT, sizeof(Vertex3D), (void *) (sizeof(f32) * 6));
    glEnableVertexAttribArray(vertices->tex_coord_id);

    // color attribute
    glVertexAttribPointer(vertices->color_id, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(f32) * 8));
    glEnableVertexAttribArray(vertices->color_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->data_id);
    glDisableVertexAttribArray(vertices->normal_id);
    glDisableVertexAttribArray(vertices->tex_coord_id);
    glDisableVertexAttribArray(vertices->color_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_3d_textureless(VertexRef* vertices, GLuint buffer, int32 count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->data_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) 0);
    glEnableVertexAttribArray(vertices->data_id);

    // normal attribute
    glVertexAttribPointer(vertices->normal_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(f32) * 3));
    glEnableVertexAttribArray(vertices->normal_id);

    // color attribute
    glVertexAttribPointer(vertices->color_id, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(f32) * 8));
    glEnableVertexAttribArray(vertices->color_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->data_id);
    glDisableVertexAttribArray(vertices->normal_id);
    glDisableVertexAttribArray(vertices->color_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_3d_colorless(VertexRef* vertices, GLuint buffer, int32 count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->data_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) 0);
    glEnableVertexAttribArray(vertices->data_id);

    // normal attribute
    glVertexAttribPointer(vertices->normal_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(f32) * 3));
    glEnableVertexAttribArray(vertices->normal_id);

    // texture coord attribute
    glVertexAttribIPointer(vertices->tex_coord_id, 2, GL_UNSIGNED_INT, sizeof(Vertex3D), (void *) (sizeof(f32) * 6));
    glEnableVertexAttribArray(vertices->tex_coord_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->data_id);
    glDisableVertexAttribArray(vertices->normal_id);
    glDisableVertexAttribArray(vertices->tex_coord_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_2d(VertexRef* vertices, GLuint buffer, int32 count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->position_id, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) 0);
    glEnableVertexAttribArray(vertices->position_id);

    // texture coord attribute
    // vs glVertexAttribPointer
    glVertexAttribIPointer(vertices->tex_coord_id, 2, GL_UNSIGNED_INT, sizeof(Vertex2D), (void *) (sizeof(f32) * 2));
    glEnableVertexAttribArray(vertices->tex_coord_id);

    // color attribute
    glVertexAttribPointer(vertices->color_id, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) (sizeof(f32) * 4));
    glEnableVertexAttribArray(vertices->color_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->position_id);
    glDisableVertexAttribArray(vertices->tex_coord_id);
    glDisableVertexAttribArray(vertices->color_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_2d_textureless(VertexRef* vertices, GLuint buffer, int32 count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->data_id, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) 0);
    glEnableVertexAttribArray(vertices->data_id);

    // color attribute
    glVertexAttribPointer(vertices->color_id, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) (sizeof(f32) * 4));
    glEnableVertexAttribArray(vertices->color_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->data_id);
    glDisableVertexAttribArray(vertices->color_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_2d_colorless(VertexRef* vertices, GLuint buffer, int32 count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->data_id, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) 0);
    glEnableVertexAttribArray(vertices->data_id);

    // texture coord attribute
    glVertexAttribIPointer(vertices->tex_coord_id, 2, GL_UNSIGNED_INT, sizeof(Vertex2D), (void *) (sizeof(f32) * 2));
    glEnableVertexAttribArray(vertices->tex_coord_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->data_id);
    glDisableVertexAttribArray(vertices->tex_coord_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
int calculate_face_size(int components, int32 faces)
{
    return sizeof(GLfloat) * 6 * components * faces;
}

// generates faces
// data is no longer needed after this
inline
uint32 gpuapi_buffer_generate(int32 size, const void* data)
{
    uint32 vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return vbo;
}

inline
uint32 gpuapi_buffer_generate_dynamic(int32 size, const void* data)
{
    uint32 vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

    return vbo;
}

inline
uint32 gpuapi_framebuffer_generate()
{
    uint32 fbo;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    return fbo;
}

inline
void gpuapi_buffer_update_dynamic(uint32 vbo, int32 size, const void* data)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

inline
uint32 gpuapi_shaderbuffer_generate(int32 size, const void* data)
{
    uint32 sbo;

    glGenBuffers(1, &sbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, sbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);

    return sbo;
}

// @todo this is not necessary?! We have a flag to determine the BindTexture Type
//      Only problem are the parameters
uint32 gpuapi_upload_color_palette(const byte* palette, int32 count, int32 sampler_id)
{
    uint32 texture_id;

    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0 + sampler_id);
    glBindTexture(GL_TEXTURE_1D, texture_id);

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, count, 0, GL_RGBA,  GL_UNSIGNED_BYTE, palette);

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture_id;
}

inline
uint32 gpuapi_uniformbuffer_generate(int32 size, const void* data)
{
    uint32 ubo;

    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);

    return ubo;
}

inline
uint32 gpuapi_buffer_element_generate(int32 size, uint32 *data)
{
    uint32 ebo;

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return ebo;
}

inline
uint32 gpuapi_vertex_array_generate()
{
    uint32 vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    return vao;
}

inline
void gpuapi_unbind_all()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

inline
void gpuapi_buffer_delete(GLuint buffer)
{
    glDeleteBuffers(1, &buffer);
}

inline
void gpuapi_vertex_array_delete(GLuint buffer)
{
    glDeleteVertexArrays(1, &buffer);
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
    int32 imgWidth, int32 imgHeight,
    int32 img_x1, int32 img_x2,
    int32 img_y1, int32 img_y2,
    int32 renderWidth, int32 renderHeight,
    int32 repeat
)
{

}
*/

#endif