#include "common.hpp"

//----------------------------------------------------------------------------

const char * vertexSource = R"glsl(
    #version 150 core

    in vec2 position;
    in vec3 color;

    out vec3 Color;

    void main ()
    {
        Color = color;
        gl_Position = vec4 (position, 0.0, 1.0);
    }
)glsl";

//----------------------------------------------------------------------------

const char * fragmentSource = R"glsl(
    #version 150 core

    in vec3 Color;

    out vec4 outColor;

    void main ()
    {
        outColor = vec4 (Color, 1.0);
    }
)glsl";

//----------------------------------------------------------------------------

bool initUserOGL ()
{
    GLuint vertexArrayObject;
    glGenVertexArrays (1, & vertexArrayObject);
    glBindVertexArray (vertexArrayObject);
    
    //------------------------------------------------------------------------

    GLuint vertexBufferObject;
    glGenBuffers (1, & vertexBufferObject);
    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferObject);
    
    float vertices [] = {
        - 0.5f,   0.5f, 1.0f, 0.0f, 0.0f,  // Vertex 0 (X, Y) Red
          0.5f,   0.5f, 0.0f, 1.0f, 0.0f,  // Vertex 1 (X, Y) Green
          0.5f, - 0.5f, 0.0f, 0.0f, 1.0f,  // Vertex 2 (X, Y) Blue
        - 0.5f, - 0.5f, 1.0f, 1.0f, 1.0f,  // Vertex 3 (X, Y) White
    };
          
    glBufferData (GL_ARRAY_BUFFER,
        sizeof (vertices), vertices,
        GL_STATIC_DRAW);    

    //------------------------------------------------------------------------

    GLuint elementBufferObject;
    glGenBuffers (1, & elementBufferObject);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

    GLuint elements [] = {
        0, 1, 2,
        2, 3, 0
    };

    glBufferData (GL_ELEMENT_ARRAY_BUFFER,
        sizeof (elements), elements,
        GL_STATIC_DRAW);

    //------------------------------------------------------------------------

    GLint posAttrib = glGetAttribLocation (shaderProgram, "position");

    glEnableVertexAttribArray (posAttrib);

    glVertexAttribPointer
    (
        posAttrib,
        2,          // Number of the values
        GL_FLOAT,
        GL_FALSE,   // Whether the parameters should be normalized
                    // between 0.0 and 1.0
                    
        5 * sizeof (float),   // Stride - number of bytes in between
        0                     // Offset from the beginning of the array
    );

    //------------------------------------------------------------------------

    GLint colAttrib = glGetAttribLocation (shaderProgram, "color");

    glEnableVertexAttribArray (colAttrib);

    glVertexAttribPointer
    (
        colAttrib,
        3,          // Number of the values
        GL_FLOAT,
        GL_FALSE,   // Whether the parameters should be normalized
                    // between 0.0 and 1.0

        5 * sizeof (float),            // Stride - number of bytes in between
        (void *) (2 * sizeof (float))  // Offset from the beginning of the array
    );

    //------------------------------------------------------------------------

    // Optional for a single output
    glBindFragDataLocation (shaderProgram, 0, "outColor");
}

//----------------------------------------------------------------------------

void displayUserOGL ()
{
    glDrawElements
    (
        GL_TRIANGLES,
        6,
        GL_UNSIGNED_INT,
        0  // Offset of the first index in the array
    );
}

//----------------------------------------------------------------------------

void cleanupUserOGL ()
{
    glDeleteBuffers      (1, & vertexBufferObject);
    glDeleteVertexArrays (1, & vertexArrayObject);
}
