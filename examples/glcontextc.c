#define HAVE_EGL
#define GLCONTEXT_IMPLEMENTATION
#include "glcontext.h"
#include "stopwatch.h"
#include <stdio.h>

#if defined(_WIN32)
	#include <Windows.h>

	void SetDPIAware()
	{
	#if defined(WSDISPLAY_NOWINXP)
		SetProcessDPIAware();
	#else
		typedef BOOL(WINAPI* SETPROCESSWSDPIAWAREOPTPROC) ();
		HINSTANCE user32 = LoadLibraryA("user32.dll");
		SETPROCESSWSDPIAWAREOPTPROC lpSetProcessDPIAware;
		lpSetProcessDPIAware =
			(SETPROCESSWSDPIAWAREOPTPROC)GetProcAddress(
				user32, "SetProcessDPIAware");
		if (lpSetProcessDPIAware)
			(*lpSetProcessDPIAware)();
		FreeLibrary(user32);
	#endif
	}
#else
	#define SetDPIAware()
#endif

#if !defined(GLMACIOS)
	#if defined(macintosh)
		#include "gl.h"
		#include "glu.h"
	#elif defined(__MACH__)
		#include <OpenGL/gl.h> /* has to be caps */
		#include <OpenGL/glu.h> /* has to be caps */
	#else
		#include <GL/gl.h> /* has to be caps */
		#include <GL/glu.h> /* has to be caps */
	#endif
void drawCube()
{
	/* Draw front face */
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f); /* Red */
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();

	/* Draw back face */
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f); /* Green */
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();

	/* Draw top face */
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f); /* Blue */
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();

	/* Draw bottom face */
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.0f); /* Yellow */
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();

	/* Draw right face */
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f); /* Magenta */
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glEnd();

	/* Draw left face */
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f); /* Cyan */
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();
}
void drawCubev2()
{
	glBegin(GL_QUADS);
	/* Front face */
	glColor3f(1.0, 0.0, 0.0); /* Red */
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);

	/* Back face */
	glColor3f(0.0, 1.0, 0.0); /* Green */
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);

	/* Top face */
	glColor3f(0.0, 0.0, 1.0); /* Blue */
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, -0.5);

	/* Bottom face */
	glColor3f(1.0, 1.0, 0.0); /* Yellow */
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);

	/* Right face */
	glColor3f(1.0, 0.0, 1.0); /* Magenta */
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);

	/* Left face */
	glColor3f(0.0, 1.0, 1.0); /* Cyan */
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glEnd();
}

void drawRubiksCube(float rotationAngle)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.1, 100.0); /* Set up a perspective projection */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 5.0, /* Camera is now placed at an angle */
			  0.0, 0.0, 0.0, /* Looking at the origin */
			  0.0, 1.0, 0.0); /* Up is in the positive Y direction */

	glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); /* Apply rotation around the y-axis */

	float offset = 1.025f; /* Offset to separate the individual cubes slightly */
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int z = -1; z <= 1; z++)
			{
				glPushMatrix();
				/* Move the cube to the correct position */
				glTranslatef(x * offset, y * offset, z * offset);
				drawCubev2(); /* Draw the individual cube */
				glPopMatrix();
			}
		}
	}
}
void update(float* rotationAngle)
{
	*rotationAngle += 2.0f; /* Increase the angle by 2 degrees */
	if (*rotationAngle > 360.0f)
		*rotationAngle -= 360.0f; /* Keep the angle within 0-360 degrees */
}
void drawscene(GLCONTEXT* glc, float rotationAngle)
{
#if 0 /* draw a simple triangle */
			glClear(GL_COLOR_BUFFER_BIT);
			glBegin(GL_TRIANGLES);
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(0.0, 1.0);
			glColor3f(0.0, 1.0, 0.0);
			glVertex2f(-1.0, -1.0);
			glColor3f(0.0, 0.0, 1.0);
			glVertex2f(1.0, -1.0);
			glEnd();
#else
			drawRubiksCube(rotationAngle);
#endif
			glFlush();
			if(glc)
				GLContext_SwapBuffers(glc);
}
void begindrawscene(GLCONTEXT* glc, float rotationAngle)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	drawscene(glc, rotationAngle);
}
int main(int argc, char** argv)
{
	float rotationAngle = 0.0;
	GLCONTEXT glc;
	GLPIXELATTRIBUTES pa;
	SetDPIAware(); /* Otherwise the gl coordinates are wrong */
	GLPixelAttributes_Create(&pa);
	GLPixelAttributes_PlatformDefaults(&pa);
	GLPixelAttributes_Depth(&pa, 24);
	GLPixelAttributes_EndList(&pa);
	if(GLContext_Create(&glc, NULL, NULL, &pa))
	{
		size_t lastupdate = 0, curval;
		WSSTOPWATCH sw;
		if(GLContext_SetCurrent(&glc))
		{
			/*glViewport(0, 0, (r.right * x) / def, (r.bottom * y) / def);*/
			/*glViewport(0, 0, r.right, r.bottom);*/
			/*glMatrixMode(GL_PROJECTION);*/
			/*glLoadIdentity();*/
			/*glOrtho(0.0, r.right, 0.0, r.bottom, -1.0, 1.0);*/
			/*glMatrixMode(GL_MODELVIEW);*/
			begindrawscene(&glc, rotationAngle);
			WSStopWatch_Construct(&sw);

			while((curval = WSStopWatch_Value(&sw)) < 2000)
			{
				#if defined(_WIN32) /* process an event for windows */
					MSG msg;
					if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)
						&& msg.message != WM_QUIT)
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					if(msg.message == WM_QUIT)
						break; /* quit */
				#endif
				if(curval > lastupdate + 15)
				{
					printf("updating at %zu...\n", curval);
					update(&rotationAngle);
					drawscene(&glc, rotationAngle);
					lastupdate = curval;
				}
			}
			WSStopWatch_Destruct(&sw);
		}
		else
			printf("GLContext_SetCurrent() failed\n");
		GLContext_Destroy(&glc); /* call when done */
	}
	else
		printf("GLContext_Create() failed\n");
	return 0;
}
#endif /* !GLMACIOS */