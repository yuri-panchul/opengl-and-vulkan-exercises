// See OpenGL Superbible 7th Edition
// Chapter 3, the subchapter Interface Blocks

#include "common.hpp"

//----------------------------------------------------------------------------

const char * computeSource        = NULL;
const char * tessControlSource    = NULL;
const char * tessEvaluationSource = NULL;
const char * geometrySource       = NULL;

//----------------------------------------------------------------------------

const char * vertexSource = R"glsl(
    #version 450 core

    // 'offset' and 'color' are input vertex attributes
    layout (location = 0) in vec4 offset;
    layout (location = 1) in vec4 color;

    // Interface block

    out VS_OUT
    {
        vec4 color;
    }
    vs_out;

    void main ()
    {
        const vec4 vertices [3] = vec4 [3]
        (
            vec4 (  0.25,  -0.25, 0.5, 1.0),
            vec4 ( -0.25,  -0.25, 0.5, 1.0),
            vec4 (  0.25,   0.25, 0.5, 1.0)
        );

        gl_Position = vertices [gl_VertexID] + offset;

        vs_out.color = color;
    }
)glsl";

//----------------------------------------------------------------------------

const char * fragmentSource = R"glsl(
    #version 450 core

    // Input from the vertex shader

    in VS_OUT
    {
        vec4 color;
    }
    fs_in;

    // Output to the framebuffer
    out vec4 color;

    void main ()
    {
        color = fs_in.color;
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

    return true;
}

//----------------------------------------------------------------------------

void displayUserOGL ()
{
    const GLfloat color1 [] =
    {
        (float) sin (curTime) * 0.5f + 0.5f,
        (float) cos (curTime) * 0.5f + 0.5f,
        0.0f,
        1.0f
    };

    glClearBufferfv (GL_COLOR, 0, color1);

    GLfloat attrib [] =
    {
        (float) sin (curTime) * 0.5f,
        (float) cos (curTime) * 0.6f,
        0.0f,
        0.0f
    };

    GLfloat color2 [] =
    {
        0.0f,
        (float) sin (curTime) * 0.5f + 0.5f,
        (float) cos (curTime) * 0.5f + 0.5f,
        1.0f
    };

    glVertexAttrib4fv (0, attrib);  // First arg is an attribute id
    glVertexAttrib4fv (1, color2);

    glDrawArrays
    (
        GL_TRIANGLES,
        0,  // How many vertices to skip
        3   // Number of vertices
    );
}

//----------------------------------------------------------------------------

void cleanupUserOGL ()
{
    glDeleteVertexArrays (1, & vertexArrayObject);
}
