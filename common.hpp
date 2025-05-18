#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//----------------------------------------------------------------------------

extern const char * computeSource;
extern const char * vertexSource;
extern const char * tessControlSource;
extern const char * tessEvaluationSource;
extern const char * geometrySource;
extern const char * fragmentSource;

GLuint shaderProgram;

bool initUserOGL    ();
void displayUserOGL ();
void cleanupUserOGL ();

//----------------------------------------------------------------------------

GLuint shadersToDelete [6];
int    nShadersToDelete = 0;

bool addShader (GLenum shaderType, const char * shaderSource)
{
    if (shaderSource == NULL)
        return true;

    GLuint shader = glCreateShader (shaderType);

    assert (  nShadersToDelete
            < sizeof (shadersToDelete) / sizeof (* shadersToDelete));

    shadersToDelete [nShadersToDelete ++] = shader;

    glShaderSource (shader, 1, & shaderSource, NULL);

    // The last argument of glShaderSource is an array of index length,
    // not needed here.

    glCompileShader (shader);

    GLint status;
    glGetShaderiv (shader, GL_COMPILE_STATUS, & status);

    char buffer [512];

    // The third argument of glGetShaderInfoLog
    // is a pointer to string length, not needed here.

    glGetShaderInfoLog (shader, sizeof (buffer), NULL, buffer);

    if (buffer [0] != '\0')
    {
        printf ("glGetShaderInfoLog (vertexShader, ...): %s\n", buffer);
        return false;
    }

    glAttachShader (shaderProgram, shader);
    return true;
}

//----------------------------------------------------------------------------

void deleteAllAddedShaders ()
{
    for (int i = 0; i < nShadersToDelete; i ++)
        glDeleteShader (shadersToDelete [i]);

    nShadersToDelete = 0;
}

//----------------------------------------------------------------------------

void cleanupShaders ()
{
    deleteAllAddedShaders ();
    glDeleteProgram (shaderProgram);
}

//----------------------------------------------------------------------------

bool initShaders ()
{
    shaderProgram = glCreateProgram ();

    if (! (   addShader (GL_COMPUTE_SHADER         , computeSource        )
           && addShader (GL_VERTEX_SHADER          , vertexSource         )
           && addShader (GL_TESS_CONTROL_SHADER    , tessControlSource    )
           && addShader (GL_TESS_EVALUATION_SHADER , tessEvaluationSource )
           && addShader (GL_GEOMETRY_SHADER        , geometrySource       )
           && addShader (GL_FRAGMENT_SHADER        , fragmentSource       )))
    {
        cleanupShaders ();
        return false;
    }

    glLinkProgram (shaderProgram);
    deleteAllAddedShaders ();
    glUseProgram  (shaderProgram);

    return true;
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
