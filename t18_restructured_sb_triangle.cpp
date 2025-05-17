#include "common.hpp"

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
    // Draw one triangle (three vertices)

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
