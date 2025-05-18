#include "common.hpp"

//----------------------------------------------------------------------------

const char * computeSource        = NULL;
const char * tessControlSource    = NULL;
const char * tessEvaluationSource = NULL;
const char * geometrySource       = NULL;

//----------------------------------------------------------------------------

const char * vertexSource = R"glsl(
    #version 150 core

    void main ()
    {
        gl_Position = vec4 (0.0, 0.0, 0.5, 1.0);
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

GLuint vertexArrayObject;

//----------------------------------------------------------------------------

bool initUserOGL ()
{
    glGenVertexArrays (1, & vertexArrayObject);
    glBindVertexArray (vertexArrayObject);

    // Optional for a single output
    glBindFragDataLocation (shaderProgram, 0, "color");

    glPointSize (100.0f);

    return true;
}

//----------------------------------------------------------------------------

void displayUserOGL ()
{
    glDrawArrays
    (
        GL_POINTS,
        0,  // How many vertices to skip
        1
    );
}

//----------------------------------------------------------------------------

void cleanupUserOGL ()
{
    glDeleteVertexArrays (1, & vertexArrayObject);
}
