/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_OPENGL_WIN32_H
#define TOS_GPUAPI_OPENGL_WIN32_H

#include <windows.h>
#include "../../platform/win32/Window.h"
#include "../../stdlib/Types.h"

typedef void WINAPI type_glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void WINAPI type_glBindFramebuffer(GLenum target, GLuint framebuffer);
typedef void WINAPI type_glGenFramebuffers(GLsizei n, GLuint *framebuffers);
typedef void WINAPI type_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLenum WINAPI type_glCheckFramebufferStatus(GLenum target);
typedef void WINAPI type_glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void WINAPI type_glAttachShader(GLuint program, GLuint shader);
typedef void WINAPI type_glCompileShader(GLuint shader);
typedef GLuint WINAPI type_glCreateProgram(void);
typedef GLuint WINAPI type_glCreateShader(GLenum type);
typedef void WINAPI type_glLinkProgram(GLuint program);
typedef void WINAPI type_glShaderSource(GLuint shader, GLsizei count, GLchar **string, GLint *length);
typedef void WINAPI type_glUseProgram(GLuint program);
typedef void WINAPI type_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void WINAPI type_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void WINAPI type_glValidateProgram(GLuint program);
typedef void WINAPI type_glGetProgramiv(GLuint program, GLenum pname, GLint *params);
typedef GLint WINAPI type_glGetUniformLocation(GLuint program, const GLchar *name);
typedef void WINAPI type_glUniform4fv(GLint location, GLsizei count, const GLfloat *value);
typedef void WINAPI type_glUniform1i(GLint location, GLint v0);
typedef void WINAPI type_glUniform1f(GLint location, GLfloat v0);
typedef void WINAPI type_glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
typedef void WINAPI type_glUniform3fv(GLint location, GLsizei count, const GLfloat *value);
typedef void WINAPI type_glEnableVertexAttribArray(GLuint index);
typedef void WINAPI type_glDisableVertexAttribArray(GLuint index);
typedef GLint WINAPI type_glGetAttribLocation(GLuint program, const GLchar *name);
typedef void WINAPI type_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void WINAPI type_glVertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void WINAPI type_glBindVertexArray(GLuint array);
typedef void WINAPI type_glGenVertexArrays(GLsizei n, GLuint *arrays);
typedef void WINAPI type_glBindBuffer(GLenum target, GLuint buffer);
typedef void WINAPI type_glGenBuffers(GLsizei n, GLuint *buffers);
typedef void WINAPI type_glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void WINAPI type_glActiveTexture(GLenum texture);
typedef void WINAPI type_glDeleteProgram(GLuint program);
typedef void WINAPI type_glDeleteShader(GLuint shader);
typedef void WINAPI type_glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
typedef void WINAPI type_glDrawBuffers(GLsizei n, const GLenum *bufs);
typedef void WINAPI type_glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void WINAPI type_glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
typedef void WINAPI type_glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void WINAPI type_glGenerateMipmap(GLenum target);
typedef void WINAPI type_glDetachShader(GLuint program, GLuint shader);
typedef void WINAPI type_glDeleteBuffers(GLsizei n, const GLuint* buffers);
typedef void WINAPI type_glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void WINAPI type_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void WINAPI type_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void WINAPI type_glGetShaderiv(GLuint shader, GLenum pname, GLint* param);
//typedef void WINAPI type_glTexImage2D(GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126

#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_PIXEL_TYPE_ARB                      0x2013

#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_FULL_ACCELERATION_ARB               0x2027

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB        0x20A9

#define WGL_RED_BITS_ARB                        0x2015
#define WGL_GREEN_BITS_ARB                      0x2017
#define WGL_BLUE_BITS_ARB                       0x2019
#define WGL_ALPHA_BITS_ARB                      0x201B
#define WGL_DEPTH_BITS_ARB                      0x2022

typedef HGLRC WINAPI wgl_create_context_attribs_arb(HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL WINAPI wgl_get_pixel_format_attrib_iv_arb(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
typedef BOOL WINAPI wgl_get_pixel_format_attrib_fv_arb(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
typedef BOOL WINAPI wgl_choose_pixel_format_arb(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef const char * WINAPI wgl_get_extensions_string_ext(void);

struct OpenGL {
    type_glTexImage2DMultisample* glTexImage2DMultisample;
    type_glBindFramebuffer* glBindFramebuffer;
    type_glGenFramebuffers* glGenFramebuffers;
    type_glFramebufferTexture2D* glFramebufferTexture2D;
    type_glCheckFramebufferStatus* glCheckFramebufferStatus;
    type_glBlitFramebuffer* glBlitFramebuffer;
    type_glAttachShader* glAttachShader;
    type_glCompileShader* glCompileShader;
    type_glCreateProgram* glCreateProgram;
    type_glCreateShader* glCreateShader;
    type_glLinkProgram* glLinkProgram;
    type_glShaderSource* glShaderSource;
    type_glUseProgram* glUseProgram;
    type_glGetProgramInfoLog* glGetProgramInfoLog;
    type_glGetShaderInfoLog* glGetShaderInfoLog;
    type_glValidateProgram* glValidateProgram;
    type_glGetProgramiv* glGetProgramiv;
    type_glGetUniformLocation* glGetUniformLocation;
    type_glUniform4fv* glUniform4fv;
    type_glUniform1i* glUniform1i;
    type_glUniform1f* glUniform1f;
    type_glUniform2fv* glUniform2fv;
    type_glUniform3fv* glUniform3fv;
    type_glEnableVertexAttribArray* glEnableVertexAttribArray;
    type_glDisableVertexAttribArray* glDisableVertexAttribArray;
    type_glGetAttribLocation* glGetAttribLocation;
    type_glVertexAttribPointer* glVertexAttribPointer;
    type_glVertexAttribIPointer* glVertexAttribIPointer;
    type_glBindVertexArray* glBindVertexArray;
    type_glGenVertexArrays* glGenVertexArrays;
    type_glBindBuffer* glBindBuffer;
    type_glGenBuffers* glGenBuffers;
    type_glBufferData* glBufferData;
    type_glActiveTexture* glActiveTexture;
    type_glDeleteProgram* glDeleteProgram;
    type_glDeleteShader* glDeleteShader;
    type_glDeleteFramebuffers* glDeleteFramebuffers;
    type_glDrawBuffers* glDrawBuffers;
    type_glTexImage3D* glTexImage3D;
    type_glTexSubImage3D* glTexSubImage3D;
    type_glDrawElementsBaseVertex* glDrawElementsBaseVertex;
    type_glGenerateMipmap* glGenerateMipmap;
    type_glDetachShader* glDetachShader;
    type_glDeleteBuffers* glDeleteBuffers;
    type_glUniformMatrix2fv* glUniformMatrix2fv;
    type_glUniformMatrix3fv* glUniformMatrix3fv;
    type_glUniformMatrix4fv* glUniformMatrix4fv;
    type_glGetShaderiv* glGetShaderiv;

    wgl_choose_pixel_format_arb* wglChoosePixelFormatARB;
    wgl_create_context_attribs_arb* wglCreateContextAttribsARB;
    wgl_get_extensions_string_ext* wglGetExtensionsStringEXT;
};

void set_pixel_format(HDC hdc, OpenGL* gl)
{
    int suggested_pixel_format_idx = 0;
    unsigned int extended_pick = 0;

    if (gl->wglChoosePixelFormatARB) {
        int attr_list[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
            0,
        };

        gl->wglChoosePixelFormatARB(hdc, attr_list, 0, 1, &suggested_pixel_format_idx, &extended_pick);
    }

    if(!extended_pick) {
        PIXELFORMATDESCRIPTOR desired_pixel_format = {};
        desired_pixel_format.nSize = sizeof(desired_pixel_format);
        desired_pixel_format.nVersion = 1;
        desired_pixel_format.iPixelType = PFD_TYPE_RGBA;
        desired_pixel_format.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
        desired_pixel_format.cColorBits = 32;
        desired_pixel_format.cAlphaBits = 8;
        desired_pixel_format.cDepthBits = 24;
        desired_pixel_format.iLayerType = PFD_MAIN_PLANE;

        suggested_pixel_format_idx = ChoosePixelFormat(hdc, &desired_pixel_format);
    }

    PIXELFORMATDESCRIPTOR suggested_pixel_format;
    DescribePixelFormat(hdc, suggested_pixel_format_idx, sizeof(suggested_pixel_format), &suggested_pixel_format);
    SetPixelFormat(hdc, suggested_pixel_format_idx, &suggested_pixel_format);
}

bool gl_extensions_load(OpenGL* gl)
{
    WNDCLASSA wc = {};

    wc.lpfnWndProc = DefWindowProcA;
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = "WGLLoader";

    if (!RegisterClassA(&wc)) {
        return false;
    }

    HWND window = CreateWindowExA(
        0,
        wc.lpszClassName,
        "ExtensionLoader",
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        wc.hInstance,
        0
    );

    HDC hdc = GetDC(window);
    set_pixel_format(hdc, gl);

    HGLRC openGLRC = wglCreateContext(hdc);

    if (!wglMakeCurrent(hdc, openGLRC) || !gl->wglGetExtensionsStringEXT) {
        return false;
    }

    char *extension = (char *) gl->wglGetExtensionsStringEXT();
    char *pos = extension;

    while(*pos) {
        while(*pos == ' ' || *pos == '\t' || *pos == '\r' || *pos == '\n') {
            ++pos;
        }

        char *end = pos;
        while(*end && !(*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) {
            ++end;
        }

        umm count = end - pos;

        // OpenGL->SupportsSRGBFramebuffer = strcmp(count, pos, "WGL_EXT_framebuffer_sRGB") == 0 || strcmp(count, pos, "WGL_ARB_framebuffer_sRGB") == 0;

        pos = end;
    }

    wglMakeCurrent(0, 0);

    wglDeleteContext(openGLRC);
    ReleaseDC(window, hdc);
    DestroyWindow(window);

    return true;
}

const int win32_opengl_attribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0,
};

void opengl_init(Window* window, OpenGL* gl)
{
    gl_extensions_load(gl);

    gl->wglChoosePixelFormatARB = (wgl_choose_pixel_format_arb *) wglGetProcAddress("wglChoosePixelFormatARB");
    gl->wglCreateContextAttribsARB = (wgl_create_context_attribs_arb *) wglGetProcAddress("wglCreateContextAttribsARB");
    gl->wglGetExtensionsStringEXT = (wgl_get_extensions_string_ext *) wglGetProcAddress("wglGetExtensionsStringEXT");

    set_pixel_format(window->hdc, gl);

    HGLRC openGLRC = 0;
    if (gl->wglCreateContextAttribsARB) {
        openGLRC = gl->wglCreateContextAttribsARB(window->hdc, 0, win32_opengl_attribs);
    }

    if (!openGLRC) {
        openGLRC = wglCreateContext(window->hdc);
    }

    if(!wglMakeCurrent(window->hdc, openGLRC)) {
        return;
    }

    gl->glTexImage2DMultisample = (type_glTexImage2DMultisample *) wglGetProcAddress("glTexImage2DMultisample");
    gl->glBindFramebuffer = (type_glBindFramebuffer *) wglGetProcAddress("glBindFramebuffer");
    gl->glGenFramebuffers = (type_glGenFramebuffers *) wglGetProcAddress("glGenFramebuffers");
    gl->glFramebufferTexture2D = (type_glFramebufferTexture2D *) wglGetProcAddress("glFramebufferTexture2D");
    gl->glCheckFramebufferStatus = (type_glCheckFramebufferStatus *) wglGetProcAddress("glCheckFramebufferStatus");
    gl->glBlitFramebuffer = (type_glBlitFramebuffer *) wglGetProcAddress("glBlitFramebuffer");
    gl->glAttachShader = (type_glAttachShader *) wglGetProcAddress("glAttachShader");
    gl->glCompileShader = (type_glCompileShader *) wglGetProcAddress("glCompileShader");
    gl->glCreateProgram = (type_glCreateProgram *) wglGetProcAddress("glCreateProgram");
    gl->glCreateShader = (type_glCreateShader *) wglGetProcAddress("glCreateShader");
    gl->glLinkProgram = (type_glLinkProgram *) wglGetProcAddress("glLinkProgram");
    gl->glShaderSource = (type_glShaderSource *) wglGetProcAddress("glShaderSource");
    gl->glUseProgram = (type_glUseProgram *) wglGetProcAddress("glUseProgram");
    gl->glGetProgramInfoLog = (type_glGetProgramInfoLog *) wglGetProcAddress("glGetProgramInfoLog");
    gl->glGetShaderInfoLog = (type_glGetShaderInfoLog *) wglGetProcAddress("glGetShaderInfoLog");
    gl->glValidateProgram = (type_glValidateProgram *) wglGetProcAddress("glValidateProgram");
    gl->glGetProgramiv = (type_glGetProgramiv *) wglGetProcAddress("glGetProgramiv");
    gl->glGetUniformLocation = (type_glGetUniformLocation *) wglGetProcAddress("glGetUniformLocation");
    gl->glUniform4fv = (type_glUniform4fv *) wglGetProcAddress("glUniform4fv");
    gl->glUniform1i = (type_glUniform1i *) wglGetProcAddress("glUniform1i");
    gl->glUniform1f = (type_glUniform1f *) wglGetProcAddress("glUniform1f");
    gl->glUniform2fv = (type_glUniform2fv *) wglGetProcAddress("glUniform2fv");
    gl->glUniform3fv = (type_glUniform3fv *) wglGetProcAddress("glUniform3fv");
    gl->glEnableVertexAttribArray = (type_glEnableVertexAttribArray *) wglGetProcAddress("glEnableVertexAttribArray");
    gl->glDisableVertexAttribArray = (type_glDisableVertexAttribArray *) wglGetProcAddress("glDisableVertexAttribArray");
    gl->glGetAttribLocation = (type_glGetAttribLocation *) wglGetProcAddress("glGetAttribLocation");
    gl->glVertexAttribPointer = (type_glVertexAttribPointer *) wglGetProcAddress("glVertexAttribPointer");
    gl->glVertexAttribIPointer = (type_glVertexAttribIPointer *) wglGetProcAddress("glVertexAttribIPointer");
    gl->glBindVertexArray = (type_glBindVertexArray *) wglGetProcAddress("glBindVertexArray");
    gl->glGenVertexArrays = (type_glGenVertexArrays *) wglGetProcAddress("glGenVertexArrays");
    gl->glBindBuffer = (type_glBindBuffer *) wglGetProcAddress("glBindBuffer");
    gl->glGenBuffers = (type_glGenBuffers *) wglGetProcAddress("glGenBuffers");
    gl->glBufferData = (type_glBufferData *) wglGetProcAddress("glBufferData");
    gl->glActiveTexture = (type_glActiveTexture *) wglGetProcAddress("glActiveTexture");
    gl->glDeleteProgram = (type_glDeleteProgram *) wglGetProcAddress("glDeleteProgram");
    gl->glDeleteShader = (type_glDeleteShader *) wglGetProcAddress("glDeleteShader");
    gl->glDeleteFramebuffers = (type_glDeleteFramebuffers *) wglGetProcAddress("glDeleteFramebuffers");
    gl->glDrawBuffers = (type_glDrawBuffers *) wglGetProcAddress("glDrawBuffers");
    gl->glTexImage3D = (type_glTexImage3D *) wglGetProcAddress("glTexImage3D");
    gl->glTexSubImage3D = (type_glTexSubImage3D *) wglGetProcAddress("glTexSubImage3D");
    gl->glDrawElementsBaseVertex = (type_glDrawElementsBaseVertex *) wglGetProcAddress("glDrawElementsBaseVertex");
    gl->glGenerateMipmap = (type_glGenerateMipmap *) wglGetProcAddress("glGenerateMipmap");
    gl->glDetachShader = (type_glDetachShader *) wglGetProcAddress("glDetachShader");
    gl->glDeleteBuffers = (type_glDeleteBuffers *) wglGetProcAddress("glDeleteBuffers");
    gl->glUniformMatrix2fv = (type_glUniformMatrix2fv *) wglGetProcAddress("glUniformMatrix2fv");
    gl->glUniformMatrix3fv = (type_glUniformMatrix3fv *) wglGetProcAddress("glUniformMatrix3fv");
    gl->glUniformMatrix4fv = (type_glUniformMatrix4fv *) wglGetProcAddress("glUniformMatrix4fv");
    gl->glGetShaderiv = (type_glGetShaderiv *) wglGetProcAddress("glGetShaderiv");

    // @todo now do: OpenGLInit
}

#endif