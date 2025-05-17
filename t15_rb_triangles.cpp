#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//----------------------------------------------------------------------------

#define BUFFER_OFFSET(a) ((void *) (a))

//----------------------------------------------------------------------------

const char * vertexSource = R"glsl(
    #version 400 core

    layout (location = 0) in vec4 vPosition;

    void main ()
    {
        gl_Position = vPosition;
    }
)glsl";

//----------------------------------------------------------------------------

const char * fragmentSource = R"glsl(
    #version 450 core

    out vec4 fColor;

    void main ()
    {
        fColor = vec4 (0.5, 0.4, 0.8, 1.0);
    }
)glsl";

//----------------------------------------------------------------------------

int main ()
{
    glfwInit ();

    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow * window = glfwCreateWindow
    (
        800,
        600,
        "OpenGL",  // title
        nullptr,   // or glfwGetPrimaryMonitor () for full screen
        nullptr    // share (resources with another window)
    );

    glfwMakeContextCurrent (window);

    //------------------------------------------------------------------------

    glewExperimental = GL_TRUE;
    glewInit ();

    //------------------------------------------------------------------------

    enum VAO_IDs    { Triangles   , NumVAOs    };
    enum Buffer_IDs { ArrayBuffer , NumBuffers };
    enum Attrib_IDs { vPosition = 0 };

    GLuint VAOs    [ NumVAOs    ];
    GLuint Buffers [ NumBuffers ];

    const GLuint NumVertices = 6;

    //------------------------------------------------------------------------

    glGenVertexArrays (NumVAOs, VAOs);
    glBindVertexArray (VAOs [Triangles]);

    GLfloat vertices [NumVertices][2] =
    {
        { -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
        {  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f }   // Triangle 2
    };

    glCreateBuffers (NumBuffers, Buffers);
    glBindBuffer    (GL_ARRAY_BUFFER, Buffers [ArrayBuffer]);
    glBufferStorage (GL_ARRAY_BUFFER, sizeof (vertices), vertices, 0);

    //------------------------------------------------------------------------

    GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vertexShader, 1, & vertexSource, NULL);

    // The last argument of glShaderSource is an array of index length,
    // not needed here.

    glCompileShader (vertexShader);

    GLint status;
    glGetShaderiv (vertexShader, GL_COMPILE_STATUS, & status);

    char buffer [512];

    // The third argument of glGetShaderInfoLog
    // is a pointer to string length, not needed here.

    glGetShaderInfoLog (vertexShader, sizeof (buffer), NULL, buffer);

    if (buffer [0] != '\0')
        printf ("glGetShaderInfoLog (vertexShader, ...): %s\n", buffer);

    //------------------------------------------------------------------------

    GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);

    glShaderSource     (fragmentShader, 1, & fragmentSource, NULL);
    glCompileShader    (fragmentShader);
    glGetShaderiv      (fragmentShader, GL_COMPILE_STATUS, & status);
    glGetShaderInfoLog (fragmentShader, sizeof (buffer), NULL, buffer);

    if (buffer [0] != '\0')
        printf ("glGetShaderInfoLog (fragmentShader, ...): %s\n", buffer);

    //------------------------------------------------------------------------

    GLuint shaderProgram = glCreateProgram ();
    glAttachShader (shaderProgram, vertexShader);
    glAttachShader (shaderProgram, fragmentShader);

    // Optional for a single output
    glBindFragDataLocation (shaderProgram, 0, "fColor");

    glLinkProgram  (shaderProgram);

    glDeleteShader (fragmentShader);
    glDeleteShader (vertexShader);

    glUseProgram   (shaderProgram);

    //------------------------------------------------------------------------

    glVertexAttribPointer
    (
        vPosition,         // index
        2,                 // size
        GL_FLOAT,          // type
        GL_FALSE,          // normalized
        0,                 // stride between attributes
        BUFFER_OFFSET (0)  // offset
    );

    glEnableVertexAttribArray (vPosition);

    //------------------------------------------------------------------------

    while (! glfwWindowShouldClose (window))
    {
        static const float black [] = { 0.0f, 0.0f, 0.0f, 0.0f };

        glClearBufferfv (GL_COLOR, 0, black);

        // Not sure if we need to bind it again
        glBindVertexArray (VAOs [Triangles]);

        glDrawArrays (GL_TRIANGLES, 0, NumVertices);

        glfwSwapBuffers (window);
        glfwPollEvents  ();

        if (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose (window, GL_TRUE);
    }

    //------------------------------------------------------------------------

    glDeleteProgram (shaderProgram);

    //------------------------------------------------------------------------

    glfwTerminate ();

    //------------------------------------------------------------------------

    return 0;
}
