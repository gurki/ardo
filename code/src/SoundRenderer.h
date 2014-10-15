#ifndef SOUND_RENDERER_H
#define SOUND_RENDERER_H

#include "common.h"
#include "Sound.h"
#include "Board.h"

#include <wincompat.h>
#include <fmod.hpp>
#include <fmod_errors.h>

#include <vector>


////////////////////////////////////////////////////////////////////////////////
class SoundRenderer
{
    public:
    
        enum Waveform
        {
            Sine,
            Square,
            SawUp,
            SawDown,
            Triangle,
            Noise
        };
    
        SoundRenderer();
        ~SoundRenderer();
    
        void setListener(const Board& board);
    
        Sound& spawnSound();
    
        void update(const float dt);
 
    
    private:
    
        vector<Sound> sounds_;
    
        FMOD_RESULT result_;
        FMOD::System* system_;
};


////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& out, const FMOD_VECTOR& v);

namespace FMOD
{
    void check(FMOD_RESULT result);
}



#endif
