#include "HudMessage.h"


////////////////////////////////////////////////////////////////////////////////
HudMessage::HudMessage() :
HudMessage("", Expired)
{}


////////////////////////////////////////////////////////////////////////////////
HudMessage::HudMessage(
   const string message__,
   const float lifetime__)
:
    message(message__),
    lifetime_(lifetime__)
{};


////////////////////////////////////////////////////////////////////////////////
const bool HudMessage::isAlive() const
{
    if (lifetime_ < 0) {
        return true;
    }
    
    return getLifetime() > 0;
}


////////////////////////////////////////////////////////////////////////////////
const float HudMessage::getLifetime() const
{
    if (lifetime_ < 0) {
        return numeric_limits<float>::infinity();
    }
    
    const float dt = lifetime_ - timer_.getElapsedTime().asSeconds();
    
    if (dt < 0) {
        return 0;
    }
    
    return dt;
}