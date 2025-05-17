#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//----------------------------------------------------------------------------

extern const char * vertexSource;
extern const char * fragmentSource;

GLuint shaderProgram;

bool initUserOGL    ();
void displayUserOGL ();
void cleanupUserOGL ();

//----------------------------------------------------------------------------

bool initShaders ()
{
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

    shaderProgram = glCreateProgram ();

    glAttachShader (shaderProgram, vertexShader);
    glAttachShader (shaderProgram, fragmentShader);
    
    glLinkProgram  (shaderProgram);

    glDeleteShader (fragmentShader);
    glDeleteShader (vertexShader);

    glUseProgram   (shaderProgram);

    return true;
}

//----------------------------------------------------------------------------

void cleanupShaders ()
{
    glDeleteProgram (shaderProgram);
}

//----------------------------------------------------------------------------

std::chrono::high_resolution_clock::time_point t_start
    = std::chrono::high_resolution_clock::now();

float curTime    = 0.0;
float curTimeAlt = 0.0;

void updateCurTime ()
{
    auto t_now = std::chrono::high_resolution_clock::now ();

    curTime = std::chrono::duration_cast
        <std::chrono::duration <float>>
            (t_now - t_start).count ();

    curTimeAlt = (float) clock () * 100 / CLOCKS_PER_SEC;
}

//----------------------------------------------------------------------------

int main ()
{
    glfwInit ();
    
    //------------------------------------------------------------------------

    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);

    //------------------------------------------------------------------------

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

    if (! (initShaders () && initUserOGL ()))
        return 1;

    //------------------------------------------------------------------------

    while (! glfwWindowShouldClose (window))
    {
        updateCurTime   ();
        displayUserOGL  ();

        glfwSwapBuffers (window);
        glfwPollEvents  ();
        
        if (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose (window, GL_TRUE);
    }

    //------------------------------------------------------------------------

    cleanupUserOGL ();
    cleanupShaders ();

    //------------------------------------------------------------------------

    glfwTerminate  ();

    return 0;
}
