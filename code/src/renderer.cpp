#include "renderer.h"

#include <GLUT/GLUT.h>


////////////////////////////////////////////////////////////////////////////////
void drawFloorTile()
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
void Renderer::setBoardFPV(const Board& board) const
{
    glLoadIdentity();
    gluPerspective(70, 800.0f / 600.0f, 0.01, 100);
    
    vec2i center = board.getPlayerCenter();
    vec2i eyes = board.getPlayerEyes();
    
    gluLookAt(center.x, 0.5f, center.y, eyes.x , 0.5f, eyes.y, 0.0f, 1.0f, 0.0f);
}


////////////////////////////////////////////////////////////////////////////////
void Renderer::drawBoard(const Board& board) const
{
    glLineWidth(2.0f);
    
    const int w = board.getWidth();
    const int h = board.getHeight();
    
    glPushMatrix();
    
        glTranslatef(-0.5f, 0,-0.5f);
        
        glBegin(GL_LINES);
        
            for (int row = 0; row <= h; row++)
            {
                glColor3f(0, row, 0);
                glVertex3i(0, 0, row);
                
                glColor3f(w, 0, 0);
                glVertex3i(w, 0, row);
            }
            
            for (int col = 0; col <= w; col++)
            {
                glColor3f(col, 0, 0);
                glVertex3i(col, 0, 0);
                
                glColor3f(0, h, 0);
                glVertex3i(col, 0, h);
            }
        
        glEnd();
    
    glPopMatrix();
}


////////////////////////////////////////////////////////////////////////////////
void Renderer::drawBalls(const unordered_set<vec2i>& balls) const
{
    glLineWidth(2.0f);
    
    for (const auto& ball : balls)
    {
        glPushMatrix();
        
            glTranslatef(ball.x, 0.0f, ball.y);
            glColor3f(0.6f, 0.6f, 0);
            drawFloorTile();
        
            glTranslatef(0.0f, 0.5f, 0.0f);
            glColor3f(0.9f, 0.9f, 0);
            glutWireSphere(0.3f, 20, 20);
        
        glPopMatrix();
    }
}


////////////////////////////////////////////////////////////////////////////////
void Renderer::drawPath(const vector<vec2i>& path) const
{
    static const float scale = 0.4f;
    
    glLineWidth(20.0f);
    
    if (path.size() < 2) {
        return;
    }

    for (const auto& tile : path)
    {
        glColor3f(0.8f, 0, 0);
        
        glPushMatrix();
        
            glTranslatef(tile.x, 0, tile.y);
            glScalef(scale, 1, scale);
            drawFloorTile();
        
        glPopMatrix();
    }
    
//    for (int i = 0; i < path.size() - 1; i++)
//    {
//        glBegin(GL_LINES);
//        
//            glColor3f(0.7f, 0, 0);
//            glVertex3f(path[i].x, 0, path[i].y);
//            glVertex3f(path[i+1].x, 0, path[i+1].y);
//        
//        glEnd();
//    }
}





