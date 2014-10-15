#include "renderer.h"


////////////////////////////////////////////////////////////////////////////////
void Renderer::drawBoard(const Board& board) const
{
    struct point {
        GLfloat x;
        GLfloat y;
    };
    
    const int w = board.getWidth();
    const int h = board.getHeight();
    
    point vertices[h][w];
    
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            vertices[i][j].x = i;
            vertices[i][j].y = j;
        }
    }
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLushort indices[2 * (w - 1) * h * 2];
    int i = 0;
    
    // Horizontal grid lines
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w - 1; x++) {
            indices[i++] = y * w + x;
            indices[i++] = y * w + x + 1;
        }
    }
    
    // Vertical grid lines
    for(int x = 0; x < w; x++) {
        for(int y = 0; y < h - 1; y++) {
            indices[i++] = y * w + x;
            indices[i++] = (y + 1) * w + x;
        }
    }
    
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(GL_LINES, 2 * (w - 1) * h * 2, GL_UNSIGNED_SHORT, 0);
}