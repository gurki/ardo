#ifndef SOUND_RENDERER_H
#define SOUND_RENDERER_H

#include "common.h"
#include "Sound.h"
#include "Board.h"
#include "Renderer.h"

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
    
        void setListenerState(const Renderer::State& state);
        void setListening(const bool listening);
    
        Sound& spawnSound();
    
        void clear();
        void update(const float dt);
 
    
    private:
    
        FMOD::Sound* createBuffer(const float duration) const;
    
        vector<Sound> sounds_;
    
        bool isListening_;
        bool isRecording_;
    
        FMOD::Sound* sound_;
        FMOD::Channel* channel_;
    
        FMOD::Sound* newSound_;
    
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
