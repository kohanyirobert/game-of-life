#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "gol_config.h"
#include "gol.h"

#define WINDOW_TITLE_PREFIX "Game of Life"
#define WINDOW_TITLE_FORMAT (WINDOW_TITLE_PREFIX " (%u FPS)")

int current_width = 500;
int current_height = 500;
int window_handle = 0;

unsigned frame_count = 0;
GLuint vertex_shared_id;
GLuint fragment_shader_id;
GLuint program_id;
GLuint vao_id;
GLuint vbo_id;
GLuint color_buffer_id;

const GLchar * vertex_shader = {
    "#version 400\n"\

    "layout(location=0) in vec4 in_Position;\n"\
    "layout(location=1) in vec4 in_Color;\n"\
    "out vec4 ex_Color;\n"\

    "void main(void)\n"\
    "{\n"\
    "  gl_Position = in_Position;\n"\
    "  ex_Color = in_Color;\n"\
    "}\n"
};

const GLchar * fragment_shader = {
    "#version 400\n"\

    "in vec4 ex_Color;\n"\
    "out vec4 out_Color;\n"\

    "void main(void)\n"\
    "{\n"\
    "  out_Color = ex_Color;\n"\
    "}\n"
};

void initialize(int, char * []);
void init_window(int, char * []);
void resize_function(const int, const int);
void render_function(void);
void timer_function(const int);
void idle_function(void);
void keyboard_function(unsigned char, int, int);
void cleanup(void);
void create_vbo(void);
void destroy_vbo(void);
void create_shaders(void);
void destroy_shaders(void);

int main(int argc, char * argv[argc])
{
    initialize(argc, argv);

    glutMainLoop();

    return EXIT_SUCCESS;
}

void initialize(int argc, char * argv[argc])
{
    puts("INFO: Project version: " gol_VERSION);

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

    create_shaders();
    create_vbo();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(3.0f);
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
    glutKeyboardFunc(keyboard_function);
    glutCloseFunc(cleanup);
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
    glDrawArrays(GL_POINTS, 0, 3);
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

void keyboard_function(unsigned char key, int x, int y)
{
    printf("x=%d y=%d key=%c\n", x, y, key);
}

void cleanup(void)
{
    destroy_vbo();
    destroy_shaders();
}

void create_shaders(void)
{
    GLenum error_value = glGetError();

    vertex_shared_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shared_id, 1, &vertex_shader, NULL);
    glCompileShader(vertex_shared_id);

    fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader, NULL);
    glCompileShader(fragment_shader_id);

    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shared_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);
    glUseProgram(program_id);

    error_value = glGetError();
    if (GL_NO_ERROR != error_value) {
        fprintf(
            stderr,
            "ERROR: Could not create the shaders: %s \n",
            gluErrorString(error_value)
        );
        exit(EXIT_FAILURE);
    }
}

void create_vbo(void)
{
    // *INDENT-OFF*
    GLfloat vertices[] = {
       -0.8f, -0.4f, 0.0f, 1.0f,
        0.0f,  0.4f, 0.0f, 1.0f,
        0.8f, -0.4f, 0.0f, 1.0f,
    };
    // *INDENT-ON*

    GLfloat colors[] = {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    };

    GLenum error_value = glGetError();

    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &color_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof colors, colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    error_value = glGetError();
    if (GL_NO_ERROR != error_value) {
        fprintf(
            stderr,
            "ERROR: Could not create a VBO: %s \n",
            gluErrorString(error_value)
        );
        exit(EXIT_FAILURE);
    }
}

void destroy_vbo(void)
{
    GLenum error_value = glGetError();

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &color_buffer_id);
    glDeleteBuffers(1, &vbo_id);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_id);

    error_value = glGetError();
    if (GL_NO_ERROR != error_value) {
        fprintf(
            stderr,
            "ERROR: Could not destroy the VBO: %s \n",
            gluErrorString(error_value)
        );
        exit(EXIT_FAILURE);
    }
}

void destroy_shaders(void)
{
    GLenum error_value = glGetError();

    glUseProgram(0);

    glDetachShader(program_id, vertex_shared_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(fragment_shader_id);
    glDeleteShader(vertex_shared_id);

    glDeleteProgram(program_id);

    error_value = glGetError();
    if (GL_NO_ERROR != error_value) {
        fprintf(
            stderr,
            "ERROR: Could not destroy the shaders: %s \n",
            gluErrorString(error_value)
        );
        exit(EXIT_FAILURE);
    }
}
