// See OpenGL Superbible 7th Edition
// Chapter 3, the subchapter Passing Data to Vertex Shader

#include "common.hpp"

//----------------------------------------------------------------------------

const char * vertexSource = R"glsl(
    #version 450 core

    layout (location = 0) in vec4 offset;

    void main ()
    {
        const vec4 vertices [3] = vec4 [3]
        (
            vec4 (  0.25,  -0.25, 0.5, 1.0),
            vec4 ( -0.25,  -0.25, 0.5, 1.0),
            vec4 (  0.25,   0.25, 0.5, 1.0)
        );

        gl_Position = vertices [gl_VertexID] + offset;
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

    return true;
}

//----------------------------------------------------------------------------

void displayUserOGL ()
{
    const GLfloat color [] =
    {
        (float) sin (curTimeAlt) * 0.5f + 0.5f,
        (float) cos (curTimeAlt) * 0.5f + 0.5f,
        0.0f,
        1.0f
    };

    glClearBufferfv (GL_COLOR, 0, color);

    GLfloat attrib [] =
    {
        (float) sin (curTimeAlt) * 0.5f,
        (float) cos (curTimeAlt) * 0.6f,
        0.0f,
        0.0f
    };

    glVertexAttrib4fv (0, attrib);  // First arg is an attribute id

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
