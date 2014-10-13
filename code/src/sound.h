#ifndef SOUND_H
#define SOUND_H

#include <wincompat.h>
#include <fmod.hpp>
#include <fmod_errors.h>

#include <iostream>


std::ostream& operator<< (std::ostream& out, const FMOD_VECTOR& v);

namespace FMOD
{
    void check(FMOD_RESULT result);
}


class Sound
{
public:
    
    Sound() {};
};



#endif
