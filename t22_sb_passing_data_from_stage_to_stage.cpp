// See OpenGL Superbible 7th Edition
// Chapter 3, the subchapter Passing Data to Vertex Shader

#include "common.hpp"

//----------------------------------------------------------------------------

const char * vertexSource = R"glsl(
    #version 450 core

    // 'offset' and 'color' are input vertex attributes
    layout (location = 0) in vec4 offset;
    layout (location = 1) in vec4 color;
    
    // 'vs_color' is an output the will be sent to the next shader stage
    out vec4 vs_color;

    void main ()
    {
        const vec4 vertices [3] = vec4 [3]
        (
            vec4 (  0.25,  -0.25, 0.5, 1.0),
            vec4 ( -0.25,  -0.25, 0.5, 1.0),
            vec4 (  0.25,   0.25, 0.5, 1.0)
        );

        gl_Position = vertices [gl_VertexID] + offset;
        
        // Output a fixed color for vs_color
        vs_color = color;
    }
)glsl";

//----------------------------------------------------------------------------

const char * fragmentSource = R"glsl(
    #version 450 core

    // Input from the vertex shader
    in vec4 vs_color;

    // Output to the framebuffer
    out vec4 color;

    void main ()
    {
        color = vs_color;
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
