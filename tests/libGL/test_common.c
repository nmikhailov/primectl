#include "test_common.h"

#include <stdio.h>
#include <stdarg.h>

#include <GL/gl.h>
#include <GL/glx.h>

void abort_(const char * s, ...) {
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}

void draw(int w, int h) {
    gl.glViewport(0, 0, w, h);
    gl.glClearColor(0.5, 0.5, 0.5, 1.0);

    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gl.glMatrixMode(GL_PROJECTION);
    gl.glLoadIdentity();
    gl.glOrtho(-10, 10, -10, 10, -1, 1);

    gl.glMatrixMode(GL_MODELVIEW);
    gl.glLoadIdentity();

    gl.glPushMatrix();
    {
        gl.glScalef(5, 5, 5);
        gl.glBegin(GL_QUADS);
        {
            gl.glColor3f(1, 0, 0);
            gl.glVertex2f(-1, -1);
            gl.glColor3f(0, 1, 0);
            gl.glVertex2f(1, -1);
            gl.glColor3f(0, 0, 1);
            gl.glVertex2f(1, 1);
            gl.glColor3f(1, 1, 1);
            gl.glVertex2f(-1, 1);
        }
        gl.glEnd();
    }
    gl.glPopMatrix();
}

void write_png(FILE* fp, png_byte* pixels, int width, int height) {
    png_structp png_ptr;
    png_infop info_ptr;

    /* initialize stuff */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        abort_("[write_png_file] png_create_write_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        abort_("[write_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during init_io");

    png_init_io(png_ptr, fp);

    /* write header */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing header");

    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB,
            PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
            PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    /* write bytes */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing bytes");

    png_bytep * row_pointers = png_malloc(png_ptr, height * sizeof(png_bytep));
    for (int i = 0; i < height; i++) {
        row_pointers[i] = &pixels[i * 3 * width];
    }
    png_write_image(png_ptr, row_pointers);

    /* end write */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during end of write");

    png_write_end(png_ptr, NULL);

    /* cleanup heap allocation */
    free(row_pointers);
}

void init_display() {
    Display* display = XOpenDisplay(NULL);
    static int visualAttribs[] = { None };
    int numberOfFramebufferConfigurations = 0;

    typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig,
            GLXContext, Bool, const int*);
    typedef Bool (*glXMakeContextCurrentARBProc)(Display*, GLXDrawable,
            GLXDrawable, GLXContext);
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;

    glXCreateContextAttribsARB =
            (glXCreateContextAttribsARBProc) gl.glXGetProcAddressARB(
                    (const GLubyte*) "glXCreateContextAttribsARB");

    GLXFBConfig* fbConfigs = gl.glXChooseFBConfig(display,
            DefaultScreen(display), visualAttribs,
            &numberOfFramebufferConfigurations);

    int context_attribs[] = { GLX_CONTEXT_MAJOR_VERSION_ARB, 2,
            GLX_CONTEXT_MINOR_VERSION_ARB, 1, None };

    GLXContext openGLContext = glXCreateContextAttribsARB(display, fbConfigs[0],
            0, True, context_attribs);
    int pbufferAttribs[] = { GLX_PBUFFER_WIDTH, 128, GLX_PBUFFER_HEIGHT, 128,
            None };
    GLXPbuffer pbuffer = gl.glXCreatePbuffer(display, fbConfigs[0],
            pbufferAttribs);

    // clean up:
    XFree(fbConfigs);
    XSync(display, False);

    if (!gl.glXMakeContextCurrent(display, pbuffer, pbuffer, openGLContext)) {
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    int width = 64, height = 64;
    init_functions();
    init_display();
    draw(width, height);

    unsigned char *pixels = (unsigned char*) malloc(width * height * 3);

    gl.glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    if (gl.glGetError() != GL_NO_ERROR) {
        exit(EXIT_FAILURE);
    }

    write_png(stdout, pixels, width, height);

    return 0;
}
