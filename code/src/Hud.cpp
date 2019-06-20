#include "Hud.h"

#include <OpenGL/glu.h>

#include <limits>
#include <cassert>
#include <sstream>


////////////////////////////////////////////////////////////////////////////////
Hud::Hud()
{
    assert(font_.loadFromFile( "res/fonts/RBNo2Light.ttf" ));
}


////////////////////////////////////////////////////////////////////////////////
void Hud::startDrawing(sf::RenderWindow& window) const
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    gluOrtho2D(0, window.getSize().x, window.getSize().y, 0);
    glMatrixMode(GL_MODELVIEW);
    
    glPushAttrib(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
}


////////////////////////////////////////////////////////////////////////////////
void Hud::finishDrawing() const
{
    glPopAttrib();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


////////////////////////////////////////////////////////////////////////////////
void Hud::draw(sf::RenderWindow& window)
{
    startDrawing(window);
    
    for (auto& pair : messages_)
    {
        const auto& position = pair.first;
        int row = 0;
        
        for(auto iter = pair.second.begin(); iter != pair.second.end();)
        {
            //  delete finished messages
            if(!iter->isAlive()) {
                iter = pair.second.erase(iter);
            }
            //  draw remaining messages
            else
            {
                drawMessage(position, iter->message, row, window);
                
                ++iter;
                ++row;
            }
        }
    }
    
    //  draw message box
    for (auto& pair : boxes_) {
        if (pair.second.isAlive()) {
            drawMessageBox(pair.first, pair.second, window);
        }
    }
    
    finishDrawing();
}


////////////////////////////////////////////////////////////////////////////////
const int Hud::getFontSize(const sf::Window& window) const {
    return window.getSize().y / 20.0f;
}


////////////////////////////////////////////////////////////////////////////////
const float Hud::getSpacing(const sf::Window& window) const {
    return window.getSize().y / 200.0f;
}


////////////////////////////////////////////////////////////////////////////////
void Hud::drawMessage(
    const Position& position,
    const HudMessage& message,
    const int row,
    sf::RenderWindow& window)
{
    const float size = getFontSize(window);
    const float spacing = getSpacing(window);
    
    //  create text object
    sf::Text text;
    text.setCharacterSize(size);
    text.setColor(sf::Color::White);
    text.setFont(font_);
    text.setString(message.message);
    
    //  set position
    sf::FloatRect bounds = text.getLocalBounds();
    
    float x = 0.0f;
    float y = 0.0f;
    
    float inset = 0.0f;
    
    if (position == Left) {
        x = spacing;
        inset = 0.0f;
    } else if (position == Center) {
        x = window.getSize().x / 2.0f;
        inset = -bounds.width / 2.0f;
    } else if (position == Right) {
        x = window.getSize().x;
        inset = -bounds.width - spacing;
    }
    
    y = row * (bounds.height + spacing);
    
    //  draw and proceed
    text.setPosition(x + inset, y);
    window.draw(text);
}


////////////////////////////////////////////////////////////////////////////////
void Hud::drawMessageBox(
    const Hud::Position &position,
    const HudMessage &message,
    sf::RenderWindow &window)
{
    const int size = getFontSize(window);
    const float spacing = getSpacing(window);
    
    //  get number of rows
    string line;
    istringstream istream(message.message);
    
    int nrows = 0;
    
    while (getline(istream, line)) {
        nrows++;
    }
    
    int height = nrows * size;

    //  create text object
    sf::Text text;
    text.setCharacterSize(size);
    text.setColor(sf::Color::White);
    text.setFont(font_);
    text.setString(message.message);
    
    //  set position
    sf::FloatRect bounds = text.getGlobalBounds();
    float x = 0.0f;
    float y = window.getSize().y / 2.0f - height / 2.0f;
    
    if (position == Left) {
        x = 10 * spacing;
    } else if (position == Center) {
        x = window.getSize().x / 2.0f - bounds.width / 2.0f;
    } else if (position == Right) {
        x = window.getSize().x - bounds.width - 10 * spacing;
    }
    
    text.setPosition(x, y);
    
    sf::Vector2f sizev;
    sizev.x = bounds.width + 20 * spacing;
    sizev.y = height + 4 * spacing;
    
    sf::RectangleShape rect;
    rect.setPosition(x - 10 * spacing, y - 0.5 * spacing);
    rect.setSize(sizev);
    rect.setFillColor(sf::Color(80, 80, 80, 188));
    rect.setOutlineColor(sf::Color(200, 200, 200));
    rect.setOutlineThickness(5.0f);
    
    window.draw(rect);
    window.draw(text);
}


////////////////////////////////////////////////////////////////////////////////
void Hud::addMessage(
    const Hud::Position &position,
    const string &message,
    const float lifetime
) {
    messages_[position].push_back(HudMessage(message, lifetime));
}


////////////////////////////////////////////////////////////////////////////////
void Hud::addMessageBox(
    const Position& position,
    const string &message,
    const float lifetime)
{
    boxes_[position] = HudMessage(message, lifetime);
}


////////////////////////////////////////////////////////////////////////////////
void Hud::dismissMessageBox(const Hud::Position &position)
{
    boxes_[position] = HudMessage();
}