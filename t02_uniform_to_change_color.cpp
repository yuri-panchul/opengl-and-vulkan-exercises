#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//----------------------------------------------------------------------------

const char * vertexSource = R"glsl(
    #version 150 core

    in vec2 position;

    void main ()
    {
        gl_Position = vec4 (position, 0.0, 1.0);
    }
)glsl";

//----------------------------------------------------------------------------

const char * fragmentSource = R"glsl(
    #version 150 core

    uniform vec3 triangleColor;

    out vec4 outColor;

    void main ()
    {
        outColor = vec4 (triangleColor, 1.0);
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

    GLuint vertexArrayObject;
    glGenVertexArrays (1, & vertexArrayObject);
    glBindVertexArray (vertexArrayObject);

    //------------------------------------------------------------------------

    GLuint vertexBufferObject;
    glGenBuffers (1, & vertexBufferObject);
    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferObject);

    float vertices [] = {
          0.0f,   0.5f,  // Vertex 1 (X, Y)
          0.5f, - 0.5f,  // Vertex 2 (X, Y)
        - 0.5f, - 0.5f,  // Vertex 3 (X, Y)
    };

    glBufferData (GL_ARRAY_BUFFER,
        sizeof (vertices), vertices,
        GL_STATIC_DRAW);

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
    glBindFragDataLocation (shaderProgram, 0, "outColor");

    glLinkProgram  (shaderProgram);

    glDeleteShader (fragmentShader);
    glDeleteShader (vertexShader);

    glUseProgram   (shaderProgram);

    //------------------------------------------------------------------------

    GLint posAttrib = glGetAttribLocation (shaderProgram, "position");

    glVertexAttribPointer
    (
        posAttrib,
        2,          // Number of the values
        GL_FLOAT,
        GL_FALSE,   // Whether the parameters should be normalized
                    // between 0.0 and 1.0

        0,          // Stride - number of bytes in between
        0           // Offset from the beginning of the array
    );

    glEnableVertexAttribArray (posAttrib);

    //------------------------------------------------------------------------

    GLint uniColor = glGetUniformLocation (shaderProgram, "triangleColor");
    glUniform3f (uniColor, 1.0f, 0.0f, 0.0f);

    //------------------------------------------------------------------------

    while (! glfwWindowShouldClose (window))
    {
        glDrawArrays
        (
            GL_TRIANGLES,
            0,  // How many vertices to skip
            3
        );

        glfwSwapBuffers (window);
        glfwPollEvents  ();

        if (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose (window, GL_TRUE);
    }

    //------------------------------------------------------------------------

    glDeleteProgram      (shaderProgram);
    glDeleteBuffers      (1, & vertexBufferObject);
    glDeleteVertexArrays (1, & vertexArrayObject);

    //------------------------------------------------------------------------

    glfwTerminate ();

    //------------------------------------------------------------------------

    return 0;
}
