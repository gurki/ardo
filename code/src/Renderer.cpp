#include "Renderer.h"

#include <GLUT/GLUT.h>


////////////////////////////////////////////////////////////////////////////////
Renderer::Renderer() :
    fpv_(true)
{
    initOpenGL();
}


////////////////////////////////////////////////////////////////////////////////
void Renderer::initOpenGL() const
{
    glEnable(GL_DEPTH_TEST);
    
//    static float amb[] =  {1.0, 1.0, 1.0, 0.0};
//    static float dif[] =  {1, 0, 0, 0};
//
//    float light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
//    float light_position[] = {100.0, 100.0, 100.0, 0.0};
//
////     set lights
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//
//    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHT1);
//    glEnable(GL_LIGHTING);

//    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
}


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
const Renderer::State Renderer::getPlayerState(const Board& board) const
{
    State state;
    
    if (fpv_)
    {
        vec2i c = board.getPlayerCenter();
        vec2i e = board.getPlayerEyes();
        
        state[0] = vec3f(c.x, 0.5f, c.y);
        state[1] = vec3f(e.x, 0.5f, e.y);
        state[2] = vec3f(0.0f, 1.0f, 0.0f);
    }
    else
    {
        const int w = board.getWidth();
        const int h = board.getHeight();

        state[0] = vec3f(w / 2.0f - 0.5f, 6.0f, h + 1.0f);
        state[1] = vec3f(w / 2.0f - 0.5f, 0.5f, h / 2.0f + 1.0f);
        state[2] = vec3f(0.0f, 1.0f, 0.0f);
    }
    
    return state;
}


////////////////////////////////////////////////////////////////////////////////
void Renderer::setViewActive(const Board& board) const
{
    initOpenGL();
    glLoadIdentity();
    gluPerspective(70, 800.0f / 600.0f, 0.01, 100);
    
    State state = getPlayerState(board);
    
    gluLookAt(
        state[0].x, state[0].y, state[0].z,
        state[1].x, state[1].y, state[1].z,
        state[2].x, state[2].y, state[2].z
    );
}


////////////////////////////////////////////////////////////////////////////////
void Renderer::drawBoard(const Board& board) const
{
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    
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
    
    glPopAttrib();
}


////////////////////////////////////////////////////////////////////////////////
void Renderer::drawPlayer(const vec2i& position) const
{
    if (fpv_) {
        return;
    }
    
    glPushMatrix();
    
        glTranslatef(position.x, 0.5f, position.y);
        glColor3f(0.9f, 0, 0);
        glutSolidCube(0.4f);
    
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
            glScalef(0.9f, 1.0f, 0.9f);
            drawFloorTile();
        
        glPopMatrix();
        
        glPushMatrix();
        
            glTranslatef(ball.x, 0.5f, ball.y);
            glColor3f(0.9f, 0.9f, 0);
            glutSolidSphere(0.3f, 20, 20);
        
        glPopMatrix();
    }
}


////////////////////////////////////////////////////////////////////////////////
void Renderer::drawGuesses(const unordered_set<vec2i> &guesses) const
{
    glLineWidth(2.0f);
    
    for (const auto& ball : guesses)
    {
        glPushMatrix();
        
            glTranslatef(ball.x, 0.01f, ball.y);
            glColor3f(0.3f, 0.7f, 0.7f);
            glScalef(0.7f, 1.0f, 0.7f);
            drawFloorTile();
        
        glPopMatrix();
        
        glPushMatrix();
        
            glTranslatef(ball.x, 0.5f, ball.y);
            glColor3f(0.3f, 0.7f, 0.7f);
            glutWireSphere(0.35f, 20, 20);
            
        glPopMatrix();
    }
}


////////////////////////////////////////////////////////////////////////////////
void Renderer::drawCurrentGuess(const vec2i &guess) const
{
    glLineWidth(2.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glPushMatrix();
        
         glTranslatef(guess.x, 0.01f, guess.y);
         glColor3f(0.3f, 0.8f, 0.8f);
         glScalef(0.8f, 1.0f, 0.8f);
         drawFloorTile();
        
    glPopMatrix();
    
    glPushMatrix();
        
        glTranslatef(guess.x, 0.5f, guess.y);
        glColor3f(0.3f, 1.0f, 1.0f);
        glutWireSphere(0.4f, 20, 20);
        
    glPopMatrix();
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


////////////////////////////////////////////////////////////////////////////////
void Renderer::drawPath(const vector<vec2i>& path) const
{
    static const float scale = 0.4f;
    
    glLineWidth(20.0f);
    
//    if (path.size() < 2) {
//        return;
//    }

    for (const auto& tile : path)
    {
        glColor3f(0.8f, 0, 0);
        
        glPushMatrix();
        
            glTranslatef(tile.x, 0.01f, tile.y);
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





