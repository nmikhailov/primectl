#ifndef TESTCOMMON_H__
#define TESTCOMMON_H__

#include <stdlib.h>
#include <png.h>
#include <GL/gl.h>
#include <GL/glx.h>

typedef struct {
	// gl
	void (*glViewport)(GLint, GLint, GLsizei, GLsizei);
	void (*glClearColor)(GLclampf, GLclampf, GLclampf, GLclampf);
	void (*glClear)(GLbitfield);
	void (*glMatrixMode)(GLenum);
	void (*glLoadIdentity)();
	void (*glOrtho)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
	void (*glPushMatrix)();
	void (*glScalef)(GLfloat, GLfloat, GLfloat);
	void (*glBegin)(GLenum);
	void (*glEnd)();
	void (*glColor3f)(GLfloat, GLfloat, GLfloat);
	void (*glVertex2f)(GLfloat, GLfloat);
	void (*glPopMatrix)();
	void (*glReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum,
			GLvoid*);
	GLenum (*glGetError)();

	// glx
	__GLXextFuncPtr (*glXGetProcAddressARB)(const GLubyte*);
	GLXFBConfig* (*glXChooseFBConfig)(Display*, int, const int*, int*);
	GLXPbuffer (*glXCreatePbuffer)(Display*, GLXFBConfig, const int*);Bool (*glXMakeContextCurrent)(
			Display*, GLXDrawable, GLXDrawable, GLXContext);

} gldefs;

void init_display();
void init_functions();

void draw(int w, int h);

void write_png(FILE* fp, png_byte* pixels, int width, int height);

gldefs gl;

#endif // PNG_WRITE_H__
