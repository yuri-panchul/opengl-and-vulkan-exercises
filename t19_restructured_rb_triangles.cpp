#include "common.hpp"

//----------------------------------------------------------------------------

#define BUFFER_OFFSET(a) ((void *) (a))

//----------------------------------------------------------------------------

const char * vertexSource = R"glsl(
    #version 400 core

    layout (location = 0) in vec4 vPosition;

    void main ()
    {
        gl_Position = vPosition;
    }
)glsl";

//----------------------------------------------------------------------------

const char * fragmentSource = R"glsl(
    #version 450 core

    out vec4 fColor;

    void main ()
    {
        fColor = vec4 (0.5, 0.4, 0.8, 1.0);
    }
)glsl";

//----------------------------------------------------------------------------

enum VAO_IDs    { Triangles   , NumVAOs    };
enum Buffer_IDs { ArrayBuffer , NumBuffers };

GLuint VAOs    [ NumVAOs    ];
GLuint Buffers [ NumBuffers ];

//----------------------------------------------------------------------------

bool initUserOGL ()
{
    enum Attrib_IDs { vPosition = 0 };
    const GLuint NumVertices = 6;

    //------------------------------------------------------------------------

    glGenVertexArrays (NumVAOs, VAOs);
    glBindVertexArray (VAOs [Triangles]);

    GLfloat vertices [NumVertices][2] =
    {
        { -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
        {  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f }   // Triangle 2
    };

    glCreateBuffers (NumBuffers, Buffers);
    glBindBuffer    (GL_ARRAY_BUFFER, Buffers [ArrayBuffer]);
    glBufferStorage (GL_ARRAY_BUFFER, sizeof (vertices), vertices, 0);

    //------------------------------------------------------------------------

    glVertexAttribPointer
    (
        vPosition,         // index
        2,                 // size
        GL_FLOAT,          // type
        GL_FALSE,          // normalized
        0,                 // stride between attributes
        BUFFER_OFFSET (0)  // offset
    );

    glEnableVertexAttribArray (vPosition);

    //------------------------------------------------------------------------

    // Optional for a single output
    glBindFragDataLocation (shaderProgram, 0, "fColor");

    return true;
}

//----------------------------------------------------------------------------

void displayUserOGL ()
{
    static const float black [] = { 0.0f, 0.0f, 0.0f, 0.0f };

    glClearBufferfv (GL_COLOR, 0, black);

    // Not sure if we need to bind it again
    glBindVertexArray (VAOs [Triangles]);

    glDrawArrays (GL_TRIANGLES, 0, NumVertices);
}

//----------------------------------------------------------------------------

void cleanupUserOGL ()
{
}
