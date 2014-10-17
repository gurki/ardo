#ifndef HUD_MESSAGE_H
#define HUD_MESSAGE_H

#include <SFML/System.hpp>

#include "common.h"


////////////////////////////////////////////////////////////////////////////////
class HudMessage
{
    public:
        
        enum LifeTime {
            Permanent = -1,
            Expired = 0,
            Temporary = 1
        };
        
        HudMessage();
        HudMessage(
           const string message__,
           const float lifetime__ = Permanent
       );
    
        const bool isAlive() const;
        const float getLifetime() const;
        
        string message;
        
        
    private:
        
        sf::Clock timer_;
        float lifetime_;
};


#endif