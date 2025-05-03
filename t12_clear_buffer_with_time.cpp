#include <chrono>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    auto t_start = std::chrono::high_resolution_clock::now ();

    while (! glfwWindowShouldClose (window))
    {
        auto t_now = std::chrono::high_resolution_clock::now ();

        float time = std::chrono::duration_cast
            <std::chrono::duration <float>>
                (t_now - t_start).count ();

        const GLfloat red []
            = {
                  (float) sin (time) * 0.5f + 0.5f,
                  (float) cos (time) * 0.5f + 0.5f,
                  0.0f,
                  1.0f  // The last number is opacity
              };

        // The second argument, GLint drawBuffer
        // is for the multiple buffer situation
            
        glClearBufferfv (GL_COLOR, 0, red);

        glfwSwapBuffers (window);
        glfwPollEvents  ();
        
        if (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose (window, GL_TRUE);
    }

    //------------------------------------------------------------------------

    glfwTerminate ();

    //------------------------------------------------------------------------

    return 0;
}
     