/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_OBJECT_H
#define TOS_OBJECT_H

#include "../stdlib/Types.h"
#include "../memory/RingMemory.h"
#include "../utils/EndianUtils.h"

#include "Vertex.h"

#if _WIN32
    #include "../platform/win32/UtilsWin32.h"
#else
    #include "../platform/linux/UtilsLinux.h"
#endif

#include "Mesh.h"
#include "../stdlib/simd/SIMD_I32.h"

#define MESH_VERSION 1

// @todo The name Object.h is stupid, copy content to Mesh.h

// @todo also handle textures etc.
// WARNING: mesh needs to have memory already reserved and asigned to data
void object_from_file_txt(
    RingMemory* ring,
    const char* path,
    Mesh* mesh
)
{
    FileBody file;
    file_read(path, &file, ring);

    char* pos = (char *) file.content;
    mesh->version = strtol(pos, &pos, 10); ++pos;

    int32 object_index = 0;
    int32 group_index = 0;

    mesh->vertices = (f32 *) mesh->data;

    mesh->vertex_count = 0;
    mesh->normal_count = 0;
    mesh->tex_coord_count = 0;
    mesh->color_count = 0;

    mesh->face_count = 0;
    mesh->face_normal_count = 0;
    mesh->face_tex_coord_count = 0;
    mesh->face_color_count = 0;

    uint32 temp_color_count = 0;

    while (*pos != '\0') {
        while (*pos == ' ' || *pos == '\t' || *pos == '\n') {
            ++pos;
        }

        if (*pos == '\0') {
            break;
        }

        // Parse type
        // WARNING: The code below could fail if [1] is outside of range
        //          However that should never happen for well formed files
        int32 state = 0;
        if (*pos == 'v' && pos[1] == ' ') {
            state = 1;
        } else if (*pos == 'v' && pos[1] == 'n') {
            state = 2;
        } else if (*pos == 'v' && pos[1] == 't') {
            state = 3;
        } else if (*pos == 'v' && pos[1] == 'p') {
            state = 4;
        } else if (*pos == 'o' && pos[1] == ' ') {
            state = 5;
        } else if (*pos == 's' && pos[1] == ' ') {
            state = 6;
        } else if (*pos == 'f' && pos[1] == ' ') {
            state = 7;
        } else if (*pos == 'g' && pos[1] == ' ') {
            state = 8;
        } else if (*pos == 'l' && pos[1] == ' ') {
            state = 9;
        } else if (*pos == 'm' && pos[1] == 't') {
            state = 10;
        } else if (*pos == 'h' && pos[1] == 'i') {
            state = 11;
        } else if (*pos == 'a' && pos[1] == 'n') {
            state = 12;
        } else if (*pos == 'u' && pos[3] == 'm') {
            state = 13;
        } else if (*pos == 'u' && pos[3] == 'h') {
            state = 14;
        } else {
            // not supported or comment
            while (*pos != '\n' && *pos != '\0') {
                ++pos;
            }
        }

        // move past keyword
        while (*pos != ' ' && *pos != '\n' && *pos != '\0') {
            ++pos;
        }

        // move past whitespaces and newline
        bool is_next_line = false;
        while (*pos == ' ' || *pos == '\n') {
            is_next_line |= *pos == '\n';
            ++pos;
        }

        if (*pos == '\0' || is_next_line) {
            continue;
        }

        // NOTE: we always load a file in the format: POSITON + NORMAL + TEXTURE + COLOR
        //      EVEN if some of the data is missing. This is necessary to keep the memory kinda in line.
        //      The actual binary file later will have the minimized layout.

        // handle data types
        switch (state) {
            case 0: break;
            case 1: {
                    // 'v'
                    if (mesh->vertex_count == 0) {
                        mesh->vertex_type |= VERTEX_TYPE_POSITION;
                    }

                    mesh->vertices[mesh->vertex_count * 3] = strtof(pos, &pos); ++pos;
                    mesh->vertices[mesh->vertex_count * 3 + 1] = strtof(pos, &pos); ++pos;
                    mesh->vertices[mesh->vertex_count * 3 + 2] = strtof(pos, &pos); ++pos;

                    // has color information
                    // @todo Move to own case statement // 'co'
                    if (*pos != '\n' && pos[1] != ' ' && pos[1] != '\n') {
                        if (mesh->vertex_count == 0) {
                            mesh->vertex_type |= VERTEX_TYPE_COLOR;
                        }

                        mesh->vertices[mesh->vertex_count * 12 + 8] = strtof(pos, &pos); ++pos;
                        mesh->vertices[mesh->vertex_count * 12 + 9] = strtof(pos, &pos); ++pos;
                        mesh->vertices[mesh->vertex_count * 12 + 10] = strtof(pos, &pos); ++pos;

                        // handle optional alpha [a]
                        if (*pos != '\n' && pos[1] != ' ' && pos[1] != '\n') {
                            mesh->vertices[mesh->vertex_count * 12 + 11] = strtof(pos, &pos); ++pos;
                        } else {
                            mesh->vertices[mesh->vertex_count * 12 + 11] = 1.0f;
                        }

                        ++temp_color_count;
                    }

                    ++mesh->vertex_count;
                } break;
            case 2: {
                    // 'vn'
                    // @bug This requires normals to be defined before textures
                    if (mesh->normal_count == 0) {
                        mesh->normals = mesh->vertices + mesh->vertex_count * 3;
                    }

                    mesh->normals[mesh->normal_count * 3] = strtof(pos, &pos); ++pos;
                    mesh->normals[mesh->normal_count * 3 + 1] = strtof(pos, &pos); ++pos;
                    mesh->normals[mesh->normal_count * 3 + 2] = strtof(pos, &pos); ++pos;

                    ++mesh->normal_count;
                } break;
            case 3: {
                    // 'vt'
                    if (mesh->tex_coord_count == 0) {
                        mesh->tex_coords = mesh->vertices + mesh->vertex_count * 3 + mesh->normal_count * 3;
                    }

                    mesh->tex_coords[mesh->tex_coord_count * 2] = strtof(pos, &pos); ++pos;
                    mesh->tex_coords[mesh->tex_coord_count * 2 + 1] = strtof(pos, &pos); ++pos;

                    ++mesh->tex_coord_count;
                } break;
            case 4: {
                    // 'vp'
                    strtof(pos, &pos); ++pos;

                    // handle optional [v]
                    if (*pos != '\n' && pos[1] != ' ' && pos[1] != '\n') {
                        strtof(pos, &pos); ++pos;

                        // handle optional [w]
                        if (*pos != '\n' && pos[1] != ' ' && pos[1] != '\n') {
                            strtof(pos, &pos); ++pos;
                        }
                    }
                } break;
            case 5: {
                    // 'o'
                    char text[100];
                    int32 i = 0;
                    while (*pos != '\0' && *pos != ' ') {
                        text[i++] = *pos++;
                    }
                    text[i] = '\0';

                    ++object_index;
                } break;
            case 6: {
                    // 's'
                    strtol(pos, &pos, 10); ++pos;
                } break;
            case 7: {
                    // 'f'
                    if (mesh->face_count == 0) {
                        mesh->faces = (uint32 *) (mesh->vertices + mesh->vertex_count * 3 + mesh->normal_count * 3 + mesh->tex_coord_count * 2 + mesh->color_count * 4);
                    }

                    int32 ftype = 0;
                    char* tmp = pos;
                    while (*tmp != ' ') {
                        if (*tmp++ == '/') {
                            ++ftype;

                            if (*tmp++ == '/') {
                                ftype = 3;
                                break;
                            }
                        }
                    }

                    const int32 max_blocks = 3; // @todo this could actually be N. Might have to change in the future
                    int32 block = 0;

                    while (*pos != '\0' && *pos != '\n') {
                        if (ftype == 0) {
                            // v1 v2 v3 ...
                            if (mesh->face_count == 0) {
                                mesh->face_type = FACE_TYPE_VERTICES;
                            }

                            mesh->faces[(mesh->face_count * max_blocks * 1) + block] = strtol(pos, &pos, 10) - 1; ++pos;
                        } else if (ftype == 1) {
                            // v1/vt1 v2/vt2 v3/vt3 ...
                            if (mesh->face_count == 0) {
                                mesh->face_type = FACE_TYPE_VERTICES | FACE_TYPE_TEXTURES;
                            }

                            mesh->faces[(mesh->face_count * max_blocks * 2) + block * 2] = strtol(pos, &pos, 10) - 1; ++pos;
                            mesh->faces[(mesh->face_count * max_blocks * 2) + block * 2 + 1] = strtol(pos, &pos, 10) - 1; ++pos;
                        } else if (ftype == 2) {
                            // v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
                            if (mesh->face_count == 0) {
                                mesh->face_type = FACE_TYPE_VERTICES | FACE_TYPE_TEXTURES | FACE_TYPE_NORMALS;
                            }

                            mesh->faces[(mesh->face_count * max_blocks * 3) + block * 3] = strtol(pos, &pos, 10) - 1; ++pos;
                            mesh->faces[(mesh->face_count * max_blocks * 3) + block * 3 + 1] = strtol(pos, &pos, 10) - 1; ++pos;
                            mesh->faces[(mesh->face_count * max_blocks * 3) + block * 3 + 2] = strtol(pos, &pos, 10) - 1; ++pos;
                        } else if (ftype == 3) {
                            // v1//vn1 v2//vn2 v3//vn3 ...
                            if (mesh->face_count == 0) {
                                mesh->face_type = FACE_TYPE_VERTICES | FACE_TYPE_NORMALS;
                            }

                            mesh->faces[(mesh->face_count * max_blocks * 2) + block * 2] = strtol(pos, &pos, 10) - 1; ++pos;
                            ++pos;
                            mesh->faces[(mesh->face_count * max_blocks * 2) + block * 2 + 1] = strtol(pos, &pos, 10) - 1; ++pos;
                        }

                        ++block;
                    }

                    ++mesh->face_count;
                } break;
            case 8: {
                    // 'g'
                    char text[100];
                    int32 i = 0;
                    while (*pos != '\0' && *pos != ' ') {
                        text[i++] = *pos++;
                    }
                    text[i] = '\0';

                    ++group_index;
                } break;
            case 9: {
                    //l
                    while (*pos != '\0' && *pos != '\n') {
                        strtol(pos, &pos, 10); ++pos;
                    }
                } break;
            case 10: {
                    // 'mtllib'
                    char text[100];
                    int32 i = 0;
                    while (*pos != '\0' && *pos != ' ') {
                        text[i++] = *pos++;
                    }
                    text[i] = '\0';
                } break;
            case 11: {
                    // 'hitlib'
                    char text[100];
                    int32 i = 0;
                    while (*pos != '\0' && *pos != ' ') {
                        text[i++] = *pos++;
                    }
                    text[i] = '\0';
                } break;
            case 12: {
                    // 'anilib'
                    char text[100];
                    int32 i = 0;
                    while (*pos != '\0' && *pos != ' ') {
                        text[i++] = *pos++;
                    }
                    text[i] = '\0';
                } break;
            case 13: {
                    // 'usemtl'
                    char text[100];
                    int32 i = 0;
                    while (*pos != '\0' && *pos != ' ') {
                        text[i++] = *pos++;
                    }
                    text[i] = '\0';
                } break;
            case 14: {
                    // 'usehit'
                    char text[100];
                    int32 i = 0;
                    while (*pos != '\0' && *pos != ' ') {
                        text[i++] = *pos++;
                    }
                    text[i] = '\0';
                } break;
        }
    }
}

enum ObjectLoadingRestriction {
    OBJECT_LOADING_RESTRICTION_POSITION = 1,
    OBJECT_LOADING_RESTRICTION_NORMAL = 2,
    OBJECT_LOADING_RESTRICTION_TEXTURE = 4,
    OBJECT_LOADING_RESTRICTION_COLOR = 8,
    OBJECT_LOADING_RESTRICTION_FACES = 16,
    OBJECT_LOADING_RESTRICTION_EVERYTHING = 31
};

// @todo sometimes we don't care about some data, we should have an option which defines which data should be loaded
//      this can improve performance for algorithms on this. e.g.:
//      on the server side we only care about the vertex positions for collision (no normals, no color, ...)
int32 object_from_file(
    RingMemory* ring,
    const char* path,
    Mesh* mesh,
    const char* group = NULL,
    int32 load_format = OBJECT_LOADING_RESTRICTION_EVERYTHING,
    int32 size = 8
)
{
    FileBody file;
    file_read(path, &file, ring);

    byte* pos = file.content;

    // Read version
    mesh->version = *((int32 *) pos);
    pos += sizeof(mesh->version);

    // Read base data
    mesh->vertex_type = *((int32 *) pos);
    pos += sizeof(mesh->vertex_type);

    mesh->vertex_count = *((int32 *) pos);
    pos += sizeof(mesh->vertex_count);

    mesh->normal_count = *((int32 *) pos);
    pos += sizeof(mesh->normal_count);

    mesh->tex_coord_count = *((int32 *) pos);
    pos += sizeof(mesh->tex_coord_count);

    mesh->color_count = *((int32 *) pos);
    pos += sizeof(mesh->color_count);

    #if !_WIN32 && !__LITTLE_ENDIAN
        mesh->version = endian_swap(mesh->version);
        mesh->vertex_type = endian_swap(mesh->vertex_type);
        mesh->verted_count = endian_swap(mesh->verted_count);
        mesh->normal_count = endian_swap(mesh->normal_count);
        mesh->tex_coord_count = endian_swap(mesh->tex_coord_count);
        mesh->color_count = endian_swap(mesh->color_count);
    #endif

    int32 vertex_size = 0;
    if (mesh->vertex_type & VERTEX_TYPE_POSITION) {
        vertex_size += 3;
    }

    if (mesh->vertex_type & VERTEX_TYPE_NORMAL) {
        vertex_size += 3;
    }

    if (mesh->vertex_type & VERTEX_TYPE_TEXTURE_COORD) {
        vertex_size += 2;
    }

    if (mesh->vertex_type & VERTEX_TYPE_COLOR) {
        vertex_size += 4;
    }

    int32 offset = 0;
    if (mesh->vertex_count > 0) {
        memcpy(mesh->data, pos, sizeof(f32) * vertex_size * mesh->vertex_count);
        mesh->vertices = (f32 *) mesh->data;

        pos += sizeof(f32) * vertex_size * mesh->vertex_count;
        offset += sizeof(f32) * vertex_size * mesh->vertex_count;
    }

    if (mesh->normal_count > 0) {
        memcpy(mesh->data + offset, pos, sizeof(f32) * 3 * mesh->normal_count);
        mesh->normals = (f32 *) (mesh->data + offset);

        pos += sizeof(f32) * 3 * mesh->normal_count;
        offset += sizeof(f32) * 3 * mesh->normal_count;
    }

    if (mesh->tex_coord_count > 0) {
        memcpy(mesh->data + offset, pos, sizeof(f32) * 3 * mesh->tex_coord_count);
        mesh->tex_coords = (f32 *) (mesh->data + offset);

        pos += sizeof(f32) * 2 * mesh->tex_coord_count;
        offset += sizeof(f32) * 2 * mesh->tex_coord_count;
    }

    if (mesh->color_count > 0) {
        memcpy(mesh->data + offset, pos, sizeof(f32) * 4 * mesh->color_count);
        mesh->colors = (f32 *) (mesh->data + offset);

        pos += sizeof(f32) * 4 * mesh->color_count;
        offset += sizeof(f32) * 4 * mesh->color_count;
    }

    // Read face data
    mesh->face_type = *((int32 *) pos);
    pos += sizeof(mesh->face_type);

    mesh->face_count = *((int32 *) pos);
    pos += sizeof(mesh->face_count);

    mesh->face_normal_count = *((int32 *) pos);
    pos += sizeof(mesh->face_normal_count);

    mesh->face_tex_coord_count = *((int32 *) pos);
    pos += sizeof(mesh->face_tex_coord_count);

    mesh->face_color_count = *((int32 *) pos);
    pos += sizeof(mesh->face_color_count);

    #if !_WIN32 && !__LITTLE_ENDIAN
        mesh->face_type = endian_swap(mesh->face_type);
        mesh->face_count = endian_swap(mesh->face_count);
        mesh->face_normal_count = endian_swap(mesh->face_normal_count);
        mesh->face_tex_coord_count = endian_swap(mesh->face_tex_coord_count);
        mesh->face_color_count = endian_swap(mesh->face_color_count);
    #endif

    int32 face_size = 0;
    if (mesh->face_type & FACE_TYPE_VERTICES) {
        face_size += 3;
    }

    if ((mesh->face_type & FACE_TYPE_NORMALS)) {
        face_size += 3;
    }

    if ((mesh->face_type & FACE_TYPE_TEXTURES)) {
        face_size += 3;
    }

    if ((mesh->face_type & FACE_TYPE_COLORS)) {
        face_size += 3;
    }

    // faces can be either in the form
    //      f: v/vt/vn ...
    // or:
    //      f: v ...
    //      f: vn ...
    //      f: vt ...
    if (mesh->face_count > 0) {
        memcpy(mesh->data + offset, pos, sizeof(uint32) * face_size * mesh->face_count);
        mesh->faces = (uint32 *) (mesh->data + offset);

        pos += sizeof(uint32) * face_size * mesh->face_count;
        offset += sizeof(uint32) * face_size * mesh->face_count;
    }

    if (mesh->face_normal_count > 0) {
        memcpy(mesh->data + offset, pos, sizeof(uint32) * 3 * mesh->face_normal_count);
        mesh->face_normals = (uint32 *) (mesh->data + offset);

        pos += sizeof(uint32) * 3 * mesh->face_normal_count;
        offset += sizeof(uint32) * 3 * mesh->face_normal_count;
    }

    if (mesh->face_tex_coord_count > 0) {
        memcpy(mesh->data + offset, pos, sizeof(uint32) * 3 * mesh->face_tex_coord_count);
        mesh->face_textures = (uint32 *) (mesh->data + offset);

        pos += sizeof(uint32) * 3 * mesh->face_tex_coord_count;
        offset += sizeof(uint32) * 3 * mesh->face_tex_coord_count;
    }

    if (mesh->face_color_count > 0) {
        memcpy(mesh->data + offset, pos, sizeof(uint32) * 3 * mesh->face_color_count);
        mesh->face_colors = (uint32 *) (mesh->data + offset);

        pos += sizeof(uint32) * 3 * mesh->face_color_count;
        offset += sizeof(uint32) * 3 * mesh->face_color_count;
    }

    SWAP_ENDIAN_LITTLE_SIMD(
        (int32 *) mesh->data,
        (int32 *) mesh->data,
        offset / 4, // everything is 4 bytes -> super easy to swap
        steps
    );

    return offset;
}

void object_to_file(
    RingMemory* ring,
    const char* path,
    const Mesh* mesh,
    int32 vertex_save_format = VERTEX_TYPE_POSITION,
    int32 face_save_format = FACE_TYPE_VERTICES,
    int32 size = 8
)
{
    FileBody file;

    // Temporary file size for buffer
    // @todo check the actual size, we are currently more or less guessing
    file.size = sizeof(mesh)
        + sizeof(Vertex3D) * mesh->vertex_count
        + sizeof(f32) * 12 * mesh->vertex_count
        + 4096;

    file.content = ring_get_memory(ring, file.size, 64);
    byte* pos = file.content;

    // version
    memcpy(pos, &mesh->version, sizeof(mesh->version));
    pos += sizeof(mesh->version);

    // vertices
    memcpy(pos, &vertex_save_format, sizeof(vertex_save_format));
    pos += sizeof(vertex_save_format);

    memcpy(pos, &mesh->vertex_count, sizeof(mesh->vertex_count));
    pos += sizeof(mesh->vertex_count);

    // We are can save the mesh in a different format from the current format -> need to adjust some values
    uint32 normal_count = mesh->normal_count == 0 && (mesh->vertex_type & VERTEX_TYPE_NORMAL)
        ? mesh->vertex_count
        : mesh->normal_count;

    memcpy(pos, &normal_count, sizeof(mesh->normal_count));
    pos += sizeof(mesh->normal_count);

    uint32 tex_coord_count = mesh->tex_coord_count == 0 && (mesh->vertex_type & VERTEX_TYPE_TEXTURE_COORD)
        ? mesh->vertex_count
        : mesh->tex_coord_count;

    memcpy(pos, &tex_coord_count, sizeof(mesh->tex_coord_count));
    pos += sizeof(mesh->tex_coord_count);

    uint32 color_count = mesh->color_count == 0 && (mesh->vertex_type & VERTEX_TYPE_COLOR)
        ? mesh->vertex_count
        : mesh->color_count;

    memcpy(pos, &color_count, sizeof(mesh->color_count));
    pos += sizeof(mesh->color_count);

    // verticies
    if (mesh->vertex_count > 0) {
        int32 vertex_size = 0;
        if (mesh->vertex_type & VERTEX_TYPE_POSITION) {
            vertex_size += 3;
        }

        if (mesh->vertex_type & VERTEX_TYPE_NORMAL) {
            vertex_size += 3;
        }

        if (mesh->vertex_type & VERTEX_TYPE_TEXTURE_COORD) {
            vertex_size += 2;
        }

        if (mesh->vertex_type & VERTEX_TYPE_COLOR) {
            vertex_size += 4;
        }

        int32 out_vertex_size = 0;
        if (vertex_save_format & VERTEX_TYPE_POSITION) {
            out_vertex_size += 3;
        }

        if (vertex_save_format & VERTEX_TYPE_NORMAL) {
            out_vertex_size += 3;
        }

        if (vertex_save_format & VERTEX_TYPE_TEXTURE_COORD) {
            out_vertex_size += 2;
        }

        if (vertex_save_format & VERTEX_TYPE_COLOR) {
            out_vertex_size += 4;
        }

        if ((mesh->vertex_type == VERTEX_TYPE_ALL && vertex_save_format == VERTEX_TYPE_ALL)
            || (mesh->vertex_type == VERTEX_TYPE_POSITION && vertex_save_format == VERTEX_TYPE_POSITION)
        ) {
            // data is the same as in the array
            memcpy(pos, mesh->vertices, vertex_size * sizeof(f32) * mesh->vertex_count);
            pos += vertex_size * sizeof(f32) * mesh->vertex_count;
        } else {
            f32* temp = mesh->vertices;
            f32* end = mesh->vertices + mesh->vertex_count * vertex_size;

            int32 offset;

            byte* vertice_start = pos;

            // @bug index gets increased every iteration BUT different groups and objects in the source may have different data
            //      This comes again down to how to handle hierarchal data with multiple groups and objects
            int32 index = 0;

            // iterate over all vertices to create new output format
            while (temp < end) {
                // @question why do I even need offset? couldn't I just directly manipulate temp?
                offset = 0;

                // First we save everything in one large array if that is the setting
                if (vertex_save_format & VERTEX_TYPE_POSITION) {
                    if (mesh->vertex_type & VERTEX_TYPE_POSITION) {
                        memcpy(pos, temp, sizeof(f32) * 3);
                        pos += sizeof(f32) * 3;

                        offset += 3;
                    } else {
                        memset(pos, 0, sizeof(f32) * 3);
                        pos += sizeof(f32) * 3;
                    }
                }

                // We want separate arrays for some data
                if ((mesh->vertex_type & VERTEX_TYPE_NORMAL) && !(vertex_save_format & VERTEX_TYPE_NORMAL)) {
                    // go to end of vertices * shift by previous array sizes + shift by current vertex -> creates one continous array with this data
                    memcpy(vertice_start
                        + sizeof(f32) * out_vertex_size * mesh->vertex_count
                        + index * sizeof(f32) * 3, temp + offset, sizeof(f32) * 3);

                    offset += 3;
                } else if (vertex_save_format & VERTEX_TYPE_NORMAL) {
                    if (mesh->vertex_type & VERTEX_TYPE_NORMAL) {
                        memcpy(pos, temp + offset, sizeof(f32) * 3);
                        pos += sizeof(f32) * 3;

                        offset += 3;
                    } else {
                        memset(pos, 0, sizeof(f32) * 3);
                        pos += sizeof(f32) * 3;
                    }
                }

                // We want separate arrays for some data
                if ((mesh->vertex_type & VERTEX_TYPE_TEXTURE_COORD) && !(vertex_save_format & VERTEX_TYPE_TEXTURE_COORD)) {
                    // go to end of vertices * shift by previous array sizes + shift by current vertex -> creates one continous array with this data
                    memcpy(vertice_start
                        + sizeof(f32) * out_vertex_size * mesh->vertex_count
                        + sizeof(f32) * normal_count * 3
                        + index * sizeof(f32) * 3, temp + offset, sizeof(f32) * 3);

                    offset += 2;
                } else if (vertex_save_format & VERTEX_TYPE_TEXTURE_COORD) {
                    if (mesh->vertex_type & VERTEX_TYPE_TEXTURE_COORD) {
                        memcpy(pos, temp + offset, sizeof(f32) * 2);
                        pos += sizeof(f32) * 2;

                        offset += 2;
                    } else {
                        memset(pos, 0, sizeof(f32) * 2);
                        pos += sizeof(f32) * 2;
                    }
                }

                // We want separate arrays for some data
                if ((mesh->vertex_type & VERTEX_TYPE_COLOR) && !(vertex_save_format & VERTEX_TYPE_COLOR)) {
                    // go to end of vertices * shift by previous array sizes + shift by current vertex -> creates one continous array with this data
                    memcpy(vertice_start
                        + sizeof(f32) * out_vertex_size * mesh->vertex_count
                        + sizeof(f32) * normal_count * 3
                        + sizeof(f32) * tex_coord_count * 2
                        + index * sizeof(f32) * 4, temp + offset, sizeof(f32) * 4);

                    offset += 4;
                } else if (vertex_save_format & VERTEX_TYPE_COLOR) {
                    if (mesh->vertex_type & VERTEX_TYPE_COLOR) {
                        memcpy(pos, temp + offset, sizeof(f32) * 4);
                        pos += sizeof(f32) * 4;

                        offset += 4;
                    } else {
                        memset(pos, 0, sizeof(f32) * 4);
                        pos += sizeof(f32) * 4;
                    }
                }

                temp += offset;
                ++index;
            }
        }

        // check if we have clean array data already -> output this array data directly
        if (mesh->normals && mesh->normal_count > 0) {
            memcpy(pos, mesh->normals, mesh->normal_count * sizeof(f32) * 3);
            pos += mesh->normal_count * sizeof(f32) * 3;
        }

        if (mesh->tex_coords && mesh->tex_coord_count > 0) {
            memcpy(pos, mesh->tex_coords, mesh->tex_coord_count * sizeof(f32) * 2);
            pos += mesh->tex_coord_count * sizeof(f32) * 2;
        }

        if (mesh->colors && mesh->color_count > 0) {
            memcpy(pos, mesh->colors, mesh->color_count * sizeof(f32) * 4);
            pos += mesh->color_count * sizeof(f32) * 4;
        }
    }

    // faces/indices
    memcpy(pos, &face_save_format, sizeof(face_save_format));
    pos += sizeof(face_save_format);

    memcpy(pos, &mesh->face_count, sizeof(mesh->face_count));
    pos += sizeof(mesh->face_count);

    // We are can save the mesh in a different format from the current format -> need to adjust some values
    uint32 face_normal_count = mesh->face_normal_count == 0 && (mesh->face_type & FACE_TYPE_NORMALS)
        ? mesh->face_count
        : mesh->face_normal_count;

    memcpy(pos, &face_normal_count, sizeof(mesh->face_normal_count));
    pos += sizeof(mesh->face_normal_count);

    uint32 face_tex_coord_count = mesh->face_tex_coord_count == 0 && (mesh->face_type & FACE_TYPE_TEXTURES)
        ? mesh->face_count
        : mesh->face_tex_coord_count;

    memcpy(pos, &face_tex_coord_count, sizeof(mesh->face_tex_coord_count));
    pos += sizeof(mesh->face_tex_coord_count);

    uint32 face_color_count = mesh->face_color_count == 0 && (mesh->face_type & FACE_TYPE_COLORS)
        ? mesh->face_count
        : mesh->face_color_count;

    memcpy(pos, &face_color_count, sizeof(mesh->face_color_count));
    pos += sizeof(mesh->face_color_count);

    if (mesh->face_count > 0) {
        // WARNING: Carefull, we again assume only 3 elements per face

        int32 face_size = 0;
        if (mesh->face_type & FACE_TYPE_VERTICES) {
            face_size += 3;
        }

        if (mesh->face_type & FACE_TYPE_NORMALS) {
            face_size += 3;
        }

        if (mesh->face_type & FACE_TYPE_TEXTURES) {
            face_size += 3;
        }

        if (mesh->face_type & FACE_TYPE_COLORS) {
            face_size += 3;
        }

        int32 out_face_size = 0;
        if (face_save_format & FACE_TYPE_VERTICES) {
            out_face_size += 3;
        }

        if (face_save_format & FACE_TYPE_NORMALS) {
            out_face_size += 3;
        }

        if (face_save_format & FACE_TYPE_TEXTURES) {
            out_face_size += 3;
        }

        if (face_save_format & FACE_TYPE_COLORS) {
            out_face_size += 3;
        }

        if ((mesh->face_type == FACE_TYPE_ALL && face_save_format == FACE_TYPE_ALL)
            || (mesh->face_type == FACE_TYPE_VERTICES && face_save_format == FACE_TYPE_VERTICES)
        ) {
            // data is the same as in the array
            memcpy(pos, mesh->faces, face_size * sizeof(uint32) * mesh->face_count);
            pos += face_size * sizeof(uint32) * mesh->face_count;
        } else {
            uint32* temp = mesh->faces;
            uint32* end = mesh->faces + mesh->face_count * face_size;

            int32 offset;

            byte* face_start = pos;

            // @bug index gets increased every iteration BUT different groups and objects in the source may have different data
            //      This comes again down to how to handle hierarchal data with multiple groups and objects
            int32 index = 0;

            // iterate over all faces to create new output format
            // one iteration represents 1 block (a block could be v/vt/vn or, v, or v//vn, ...)
            while (temp < end) {
                // @question why do I even need offset? couldn't I just directly manipulate temp?
                offset = 0;

                // First we save everything in one large array if that is the setting
                if (face_save_format & FACE_TYPE_VERTICES) {
                    if (mesh->face_type & FACE_TYPE_VERTICES) {
                        memcpy(pos, temp, sizeof(uint32));
                        pos += sizeof(uint32);

                        offset += 1;
                    } else {
                        memset(pos, 0, sizeof(f32));
                        pos += sizeof(f32);
                    }
                }

                // We want separate arrays for some data
                if (mesh->face_type & FACE_TYPE_NORMALS && !(face_save_format & FACE_TYPE_NORMALS)) {
                    // go to end of faces * shift by previous array sizes + shift by current face -> creates one continous array with this data
                    memcpy(face_start
                        + sizeof(uint32) * out_face_size * mesh->face_count
                        + index * sizeof(uint32), temp + offset, sizeof(uint32));

                    offset += 1;
                } else if (face_save_format & FACE_TYPE_NORMALS) {
                    if (mesh->face_type & FACE_TYPE_NORMALS) {
                        memcpy(pos, temp + offset, sizeof(uint32));
                        pos += sizeof(uint32);

                        offset += 1;
                    } else {
                        memset(pos, 0, sizeof(uint32));
                        pos += sizeof(uint32);
                    }
                }

                // We want separate arrays for some data
                if (mesh->face_type & FACE_TYPE_TEXTURES && !(face_save_format & FACE_TYPE_TEXTURES)) {
                    // go to end of faces * shift by previous array sizes + shift by current face -> creates one continous array with this data
                    memcpy(face_start
                        + sizeof(uint32) * out_face_size * mesh->face_count
                        + sizeof(uint32) * 3 * face_normal_count
                        + index * sizeof(uint32), temp + offset, sizeof(uint32));

                    offset += 1;
                } else if (face_save_format & FACE_TYPE_TEXTURES) {
                    if (mesh->face_type & FACE_TYPE_TEXTURES) {
                        memcpy(pos, temp + offset, sizeof(uint32));
                        pos += sizeof(uint32);

                        offset += 1;
                    } else {
                        memset(pos, 0, sizeof(uint32));
                        pos += sizeof(uint32);
                    }
                }

                // We want separate arrays for some data
                if (mesh->face_type & VERTEX_TYPE_COLOR && !(face_save_format & VERTEX_TYPE_COLOR)) {
                    // go to end of faces * shift by previous array sizes + shift by current face -> creates one continous array with this data
                    memcpy(face_start
                        + sizeof(uint32) * out_face_size * mesh->face_count
                        + sizeof(uint32) * 3 * face_normal_count
                        + sizeof(uint32) * 3 * face_tex_coord_count
                        + index * sizeof(uint32), temp + offset, sizeof(uint32));

                    offset += 1;
                } else if (face_save_format & VERTEX_TYPE_COLOR) {
                    if (mesh->face_type & VERTEX_TYPE_COLOR) {
                        memcpy(pos, temp + offset, sizeof(uint32));
                        pos += sizeof(uint32);

                        offset += 1;
                    } else {
                        memset(pos, 0, sizeof(uint32));
                        pos += sizeof(uint32);
                    }
                }

                temp += offset;
                ++index;
            }
        }

        // check if we have clean array data already -> output this array data directly
        if (mesh->face_normals && mesh->face_normal_count > 0) {
            memcpy(pos, mesh->face_normals, mesh->face_normal_count * sizeof(uint32) * 3);
            pos += mesh->face_normal_count * sizeof(uint32) * 3;
        }

        if (mesh->face_textures && mesh->face_tex_coord_count > 0) {
            memcpy(pos, mesh->face_textures, mesh->face_tex_coord_count * sizeof(uint32) * 3);
            pos += mesh->face_tex_coord_count * sizeof(uint32) * 3;
        }

        if (mesh->face_colors && mesh->face_color_count > 0) {
            memcpy(pos, mesh->face_colors, mesh->face_color_count * sizeof(uint32) * 3);
            pos += mesh->face_color_count * sizeof(uint32) * 3;
        }
    }

    file.size = pos - file.content;

    SWAP_ENDIAN_LITTLE_SIMD(
        (int32 *) file.content,
        (int32 *) file.content,
        file.size / 4, // everything in here is 4 bytes -> super easy to swap
        steps
    );

    /*
    FileBody file2;
    file2.content = ring_get_memory(ring, file.size, 64);
    file2.size = encode_lzp(file.content, file.size, file2.content);

    file_write(path, &file2);
    */

    file_write(path, &file);
}

#endif