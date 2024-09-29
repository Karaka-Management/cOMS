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
#include "../../utils/StringUtils.h"

#include "../RenderUtils.h"
#include "Opengl.h"

#ifdef _WIN32
    #include <windows.h>
    #include "../../platform/win32/UtilsWin32.h"
    #include "../../platform/win32/Window.h"
#endif

inline
void change_viewport(Window* w, int offset_x = 0, int offset_y = 0)
{
    glViewport(offset_x, offset_y, w->width, w->height);
}

inline
void vsync_set(bool on)
{
    wglSwapIntervalEXT((int) on);
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
    int major;
    int minor;
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

inline
void prepare_texture(Texture* texture)
{
    uint32 texture_data_type = get_texture_data_type(texture->texture_data_type);

    glGenTextures(1, (GLuint *) &texture->id);
    glActiveTexture(GL_TEXTURE0 + texture->sample_id);
    glBindTexture(texture_data_type, (GLuint) texture->id);
}

inline
void load_texture_to_gpu(const Texture* texture, int mipmap_level = 0)
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
void texture_use(const Texture* texture, uint32 texture_unit)
{
    glActiveTexture(GL_TEXTURE0 + texture->sample_id);
    glBindTexture(GL_TEXTURE_2D, (GLuint) texture->id);
}

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

        ASSERT_SIMPLE(false);

        // @todo log
    }

    return shader;
}

inline
GLuint shader_load(GLenum type, const char* path, RingMemory* ring) {
    uint64 temp = ring->pos;

    FileBody file;

    // @todo consider to accept file as parameter and load file before
    file_read(path, &file, ring);
    GLuint result = shader_make(type, (const char *) file.content, ring);

    // We can immediately dispose of it we can also reset our ring memory position
    ring->pos = temp;

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
        ASSERT_SIMPLE(false);

        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        GLchar *info = (GLchar *) ring_get_memory(ring, length * sizeof(GLchar));

        glGetProgramInfoLog(program, length, NULL, info);

        // @todo use global logger
        fprintf(stderr, "glLinkProgram failed: %s\n", info);
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
void draw_triangles_3d(VertexRef* vertices, GLuint buffer, int count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->data_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) 0);
    glEnableVertexAttribArray(vertices->data_id);

    // normal attribute
    glVertexAttribPointer(vertices->normal_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(float) * 3));
    glEnableVertexAttribArray(vertices->normal_id);

    // texture coord attribute
    // vs glVertexAttribPointer
    glVertexAttribIPointer(vertices->tex_coord_id, 2, GL_UNSIGNED_INT, sizeof(Vertex3D), (void *) (sizeof(float) * 6));
    glEnableVertexAttribArray(vertices->tex_coord_id);

    // color attribute
    glVertexAttribPointer(vertices->color_id, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(float) * 8));
    glEnableVertexAttribArray(vertices->color_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->data_id);
    glDisableVertexAttribArray(vertices->normal_id);
    glDisableVertexAttribArray(vertices->tex_coord_id);
    glDisableVertexAttribArray(vertices->color_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_3d_textureless(VertexRef* vertices, GLuint buffer, int count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->data_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) 0);
    glEnableVertexAttribArray(vertices->data_id);

    // normal attribute
    glVertexAttribPointer(vertices->normal_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(float) * 3));
    glEnableVertexAttribArray(vertices->normal_id);

    // color attribute
    glVertexAttribPointer(vertices->color_id, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(float) * 8));
    glEnableVertexAttribArray(vertices->color_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->data_id);
    glDisableVertexAttribArray(vertices->normal_id);
    glDisableVertexAttribArray(vertices->color_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_3d_colorless(VertexRef* vertices, GLuint buffer, int count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->data_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) 0);
    glEnableVertexAttribArray(vertices->data_id);

    // normal attribute
    glVertexAttribPointer(vertices->normal_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *) (sizeof(float) * 3));
    glEnableVertexAttribArray(vertices->normal_id);

    // texture coord attribute
    glVertexAttribIPointer(vertices->tex_coord_id, 2, GL_UNSIGNED_INT, sizeof(Vertex3D), (void *) (sizeof(float) * 6));
    glEnableVertexAttribArray(vertices->tex_coord_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->data_id);
    glDisableVertexAttribArray(vertices->normal_id);
    glDisableVertexAttribArray(vertices->tex_coord_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_2d(VertexRef* vertices, GLuint buffer, int count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->position_id, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) 0);
    glEnableVertexAttribArray(vertices->position_id);

    // texture coord attribute
    // vs glVertexAttribPointer
    glVertexAttribIPointer(vertices->tex_coord_id, 2, GL_UNSIGNED_INT, sizeof(Vertex2D), (void *) (sizeof(float) * 2));
    glEnableVertexAttribArray(vertices->tex_coord_id);

    // color attribute
    glVertexAttribPointer(vertices->color_id, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) (sizeof(float) * 4));
    glEnableVertexAttribArray(vertices->color_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->position_id);
    glDisableVertexAttribArray(vertices->tex_coord_id);
    glDisableVertexAttribArray(vertices->color_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_2d_textureless(VertexRef* vertices, GLuint buffer, int count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->data_id, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) 0);
    glEnableVertexAttribArray(vertices->data_id);

    // color attribute
    glVertexAttribPointer(vertices->color_id, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) (sizeof(float) * 4));
    glEnableVertexAttribArray(vertices->color_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->data_id);
    glDisableVertexAttribArray(vertices->color_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
void draw_triangles_2d_colorless(VertexRef* vertices, GLuint buffer, int count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // position attribute
    glVertexAttribPointer(vertices->data_id, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) 0);
    glEnableVertexAttribArray(vertices->data_id);

    // texture coord attribute
    glVertexAttribIPointer(vertices->tex_coord_id, 2, GL_UNSIGNED_INT, sizeof(Vertex2D), (void *) (sizeof(float) * 2));
    glEnableVertexAttribArray(vertices->tex_coord_id);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(vertices->data_id);
    glDisableVertexAttribArray(vertices->tex_coord_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline
int calculate_face_size(int components, int faces)
{
    return sizeof(GLfloat) * 6 * components * faces;
}

// generates faces
// data is no longer needed after this
inline
uint32 gpuapi_buffer_generate(int size, const void* data)
{
    uint32 vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return vbo;
}

inline
uint32 gpuapi_shaderbuffer_generate(int size, const void* data)
{
    uint32 sbo;

    glGenBuffers(1, &sbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, sbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);

    return sbo;
}

uint32 gpuapi_upload_color_palette(const byte* palette, int count, int sampler_id)
{
    uint32 texture_id;

    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0 + sampler_id);
    glBindTexture(GL_TEXTURE_1D, texture_id);

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, count, 0, GL_RGBA,  GL_UNSIGNED_BYTE, palette);

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture_id;
}

inline
uint32 gpuapi_uniformbuffer_generate(int size, const void* data)
{
    uint32 ubo;

    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);

    return ubo;
}

inline
uint32 gpuapi_buffer_element_generate(int size, uint32 *data)
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

struct TextRender {
    uint32 align;
    uint32 x;
    uint32 y;
    float scale;
    VertexRef vertices;
    char* text;

    uint32 sampler_id;

    TextShader shader_data;
};

#define TEXT_ALIGN_LEFT 0
#define TEXT_ALIGN_CENTER 1
#define TEXT_ALIGN_RIGHT 2

// @performance This needs to handle batched randering, isolated rendering is WAY to inefficient
inline
void render_text_batched(
    RingMemory* ring,
    int width, int height,
    TextRender* text_data
) {
    glUseProgram(text_data->shader_data.program_id);
    glUniform1i(text_data->shader_data.sampler_addr, text_data->vertices.sampler_id);

    // @performance Instead of re-creating the matrix every render call just use the id to activate it
    // 2d projection
    if (text_data->shader_data.matrix_id == 0) {
        float matrix[16] = {};
        mat4_ortho_sparse_rh(matrix, 0.0f, (float) width, 0.0f, (float) height, -1.0f, 1.0f);
        glUniformMatrix4fv(text_data->shader_data.matrix_addr, 1, GL_FALSE, matrix);
        text_data->shader_data.matrix_id = 1;
        // @bug this is wrong, we need to make buffer instead. We don't want to upload the matrix every time
        //      Isn't this buffer the same for every text?
        //      If yes, consider to save the orth projection matrix globally and change it whenever we change the resolution/window dimensions
    } else {
        // @question Do we even need to bind it if we never change it?
        // We also never bind our projection matrix apart from the first time and it works. This should be the same no?
        //glBindVertexArray(text_data->shader_data.matrix_id);
    }

    int length = (int) strlen(text_data->text);
    float x = text_data->x - text_data->scale * text_data->align * (length - 1) / 2;

    // @performance Only create when the text got removed from memory
    if (text_data->vertices.data_id == 0) {
        GLfloat *data = (GLfloat *) ring_get_memory(ring, sizeof(GLfloat) * 6 * 4 * length);

        for (int i = 0; i < length; i++) {
            make_character(data + i * 24, x, (float) text_data->y, text_data->scale / 2, text_data->scale, text_data->text[i]);
            x += text_data->scale;
        }

        text_data->vertices.data_id = gpuapi_buffer_generate(sizeof(GLfloat) * 6 * 4 * length, data);
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, text_data->vertices.data_id);
        //glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); Is this required?
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    draw_triangles_2d(&text_data->vertices, text_data->vertices.data_id, length * 6);
    glDisable(GL_BLEND);

    // @performance We should only delete the buffer, when the text becomes invisible
    // @todo remember to implement a remove logic for all the buffers
    //glDeleteBuffers(1, &text_data->vertices.data_id);
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