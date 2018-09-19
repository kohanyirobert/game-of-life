#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "gol_config.h"

#define WINDOW_TITLE_PREFIX "Game of Life"
#define WINDOW_TITLE_FORMAT (WINDOW_TITLE_PREFIX " (%u FPS)")

int current_width = 500;
int current_height = 500;
int window_handle = 0;
unsigned frame_count = 0;

void initialize(int, char * []);
void init_window(int, char * []);
void resize_function(const int, const int);
void render_function(void);
void timer_function(const int);
void idle_function(void);

int main(int argc, char * argv[argc])
{
    initialize(argc, argv);

    glutMainLoop();

    return EXIT_SUCCESS;
}

void initialize(int argc, char * argv[argc])
{
    printf(
        "INFO: Project version: %d.%d.%d.%d\n",
        gol_VERSION_MAJOR,
        gol_VERSION_MINOR,
        gol_VERSION_PATCH,
        gol_VERSION_TWEAK
    );

    GLenum result;

    init_window(argc, argv);

    result = glewInit();

    if (GLEW_OK != result) {
        fprintf(
            stderr,
            "ERROR: %s\n",
            glewGetErrorString(result)
        );
        exit(EXIT_FAILURE);
    }

    printf(
        "INFO: OpenGL version: %s\n",
        glGetString(GL_VERSION)
    );

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void init_window(int argc, char * argv[argc])
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 0);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );

    glutInitWindowSize(current_width, current_height);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    window_handle = glutCreateWindow(WINDOW_TITLE_PREFIX);

    if (window_handle <= 0) {
        fprintf(
            stderr,
            "ERROR: Could not create a new rendering window.\n"
        );
        exit(EXIT_FAILURE);
    }

    glutReshapeFunc(resize_function);
    glutDisplayFunc(render_function);
    glutIdleFunc(idle_function);
    glutTimerFunc(0, timer_function, 0);
}

void resize_function(const int width, const int height)
{
    current_width = width;
    current_height = height;
    glViewport(0, 0, current_width, current_height);
}

void render_function(void)
{
    ++frame_count;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
    glutPostRedisplay();
}

void idle_function(void)
{
    glutPostRedisplay();
}

void timer_function(const int timer_id)
{
    if (0 == glutGetWindow()) {
        return;
    }

    if (1 == timer_id) {
        unsigned fps = frame_count;
        int size = snprintf(NULL, 0, WINDOW_TITLE_FORMAT, fps);
        char * buffer = malloc((size + 1) * sizeof (char *));
        sprintf_s(buffer, size + 1, WINDOW_TITLE_FORMAT, fps);
        glutSetWindowTitle(buffer);
        free(buffer);
    } else {
        glutSetWindowTitle(WINDOW_TITLE_PREFIX);
    }

    frame_count = 0;
    glutTimerFunc(1000, timer_function, 1);
}
