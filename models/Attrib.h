/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ATTRIB_H
#define TOS_ATTRIB_H

#if OPENGL
    #include "../../EngineDependencies/opengl/glew/include/GL/glew.h"

    struct Attrib {
        GLuint program;
        GLuint position;
        GLuint normal;
        GLuint uv;
        GLuint matrix;
        GLuint sampler;
        GLuint camera;
        GLuint timer;
        GLuint extra1;
        GLuint extra2;
        GLuint extra3;
        GLuint extra4;
    };
#else
    struct Attrib {};
#endif

#endif