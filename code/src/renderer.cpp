#include "renderer.h"

#include <GLUT/GLUT.h>

////////////////////////////////////////////////////////////////////////////////
void drawFloorTile(const vec2i& pos)
{
    glBegin(GL_QUADS);
    
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, 0,-0.5f);
        glVertex3f( 0.5f, 0,-0.5f);
        glVertex3f( 0.5f, 0, 0.5f);
        glVertex3f(-0.5f, 0, 0.5f);
    
    glEnd();
}


////////////////////////////////////////////////////////////////////////////////
void Renderer::drawBoard(const Board& board) const
{
    //  set camera
    glLoadIdentity();
    gluPerspective(70, 800.0f / 600.0f, 0.01, 100);
    
    vec2i center = board.getPlayerCenter();
    vec2i eyes = board.getPlayerEyes();
    
    gluLookAt(center.x, 0.5f, center.y, eyes.x , 0.5f, eyes.y, 0.0f, 1.0f, 0.0f);
    
    //  draw board
    glLineWidth(5.0f);
    
    const int w = board.getWidth();
    const int h = board.getHeight();

    glPushMatrix();
    
        glTranslatef(-0.5f, 0,-0.5f);

        glBegin(GL_LINES);
        
            for (int row = 0; row <= h; row++) {
                glColor3f(0, row, 0);
                glVertex3i(0, 0, row);
                glColor3f(w, 0, 0);
                glVertex3i(w, 0, row);
            }
            
            for (int col = 0; col <= w; col++) {
                glColor3f(col, 0, 0);
                glVertex3i(col, 0, 0);
                glColor3f(0, h, 0);
                glVertex3i(col, 0, h);
            }
            
        glEnd();
    
    glPopMatrix();
    
    //  draw balls
    for (const auto& ball : board.getBalls())
    {
        glPushMatrix();
        
            glTranslatef(ball.x, 0.0f, ball.y);
            glColor3f(0.6f, 0.6f, 0);
            drawFloorTile(ball);

            glTranslatef(0.0f, 0.5f, 0.0f);
            glColor3f(0.9f, 0.9f, 0);
            glutWireSphere(0.3f, 20, 20);
        
        glPopMatrix();
    }
}