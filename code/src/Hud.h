#ifndef HUD_H
#define HUD_H

#include <vector>
#include <map>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "common.h"
#include "HudMessage.h"


////////////////////////////////////////////////////////////////////////////////
class Hud
{
    public:
        
        enum Position {
            Left,
            Center,
            Right
        };
        
        Hud();
        
        void draw(sf::RenderWindow& window);
        
        void addMessage(
            const Position& position,
            const string& message,
            const float lifetime = -1);
        
        void addMessageBox(
            const Position& position,
            const string& message,
            const float lifetime = 2);
    
        void dismissMessageBox(const Position& position);
    
        
    private:
    
        const int getFontSize(const sf::Window& window) const;
        const float getSpacing(const sf::Window& window) const;
    
        void drawMessage(
            const Position& position,
            const HudMessage& message,
            const int row,
            sf::RenderWindow& window);
    
        void drawMessageBox(
            const Hud::Position &position,
            const HudMessage &message,
            sf::RenderWindow &window);
    
        void startDrawing(sf::RenderWindow& window) const;
        void finishDrawing() const;
        
        sf::Font font_;
        map<Position, vector<HudMessage>> messages_;    //  position -> [message]
        map<Position, HudMessage> boxes_;               //  position -> message
};


#endif
