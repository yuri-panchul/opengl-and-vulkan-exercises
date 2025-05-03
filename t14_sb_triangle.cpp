#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//----------------------------------------------------------------------------

const char * vertexSource = R"glsl(
    #version 150 core

    void main ()
    {
        const vec4 vertices[3] = vec4[3]
        (
            vec4 (  0.25,  -0.25, 0.5, 1.0),
            vec4 ( -0.25,  -0.25, 0.5, 1.0),
            vec4 (  0.25,   0.25, 0.5, 1.0)
        );

        gl_Position = vertices [gl_VertexID];
    }
)glsl";

//----------------------------------------------------------------------------

const char * fragmentSource = R"glsl(
    #version 150 core

    out vec4 color;

    void main ()
    {
        color = vec4 (0.0, 0.8, 1.0, 1.0);
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
    glBindFragDataLocation (shaderProgram, 0, "color");

    glLinkProgram  (shaderProgram);

    glDeleteShader (fragmentShader);
    glDeleteShader (vertexShader);

    glUseProgram   (shaderProgram);

    //------------------------------------------------------------------------

    glPointSize (100.0f);

    //------------------------------------------------------------------------

    while (! glfwWindowShouldClose (window))
    {
        // Draw one triangle (three vertices)

        glDrawArrays
        (
            GL_TRIANGLES,
            0,  // How many vertices to skip
            3   // Number of vertices
        );

        glfwSwapBuffers (window);
        glfwPollEvents  ();
        
        if (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose (window, GL_TRUE);
    }

    //------------------------------------------------------------------------

    glDeleteProgram      (shaderProgram);
    glDeleteVertexArrays (1, & vertexArrayObject);

    //------------------------------------------------------------------------

    glfwTerminate ();

    //------------------------------------------------------------------------

    return 0;
}
     