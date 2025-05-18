// See OpenGL Superbible 7th Edition
// Chapter 3, the subchapter Geometry Shaders

#include "common.hpp"

//----------------------------------------------------------------------------

const char * computeSource        = NULL;
const char * tessControlSource    = NULL;
const char * tessEvaluationSource = NULL;

//----------------------------------------------------------------------------

const char * vertexSource = R"glsl(
    #version 450 core

    void main ()
    {
        const vec4 vertices [] = vec4 []
        (
            vec4 (  0.25,  -0.25, 0.5, 1.0),
            vec4 ( -0.25,  -0.25, 0.5, 1.0),
            vec4 (  0.25,   0.25, 0.5, 1.0)
        );

        gl_Position = vertices [gl_VertexID];
    }
)glsl";

//----------------------------------------------------------------------------

const char * geometrySource = R"glsl(
    #version 450 core
    
    layout (triangles) in;
    layout (points, max_vertices = 3) out;

    void main ()
    {
        int i;

        for (i = 0; i < gl_in.length (); i++)
        {
            gl_Position = gl_in [i].gl_Position;
            EmitVertex ();
        }
    }
)glsl";

//----------------------------------------------------------------------------

const char * fragmentSource = R"glsl(
    #version 450 core

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

    glPointSize (20.0f);
    return true;
}

//----------------------------------------------------------------------------

void displayUserOGL ()
{
    static const GLfloat green [] = { 0.0f, 0.25f, 0.0f, 1.0f };
    glClearBufferfv (GL_COLOR, 0, green);

    glDrawArrays (GL_TRIANGLES, 0, 3);
}

//----------------------------------------------------------------------------

void cleanupUserOGL ()
{
    glDeleteVertexArrays (1, & vertexArrayObject);
}
