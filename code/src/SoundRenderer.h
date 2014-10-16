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
#include <deque>

class Game;

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
        void playSound(const vec3f& position, const vec3f& velocity, FMOD::Sound* sound);
        Sound& createDSP();
    
        void toggleListening();
        void toggleAmmunition();
    
        void clear();
        void shoot();
        void update(const float dt);
    
        Game* game;
 
    
    private:
    
        FMOD::Sound* createBuffer(const float duration = 3.0f) const;
    
        deque<Sound> sounds_;
        vector<FMOD::Channel*> channels_;
    
        bool shouldListen_;
        bool shootsRecording_;
        bool isRecording_;
    
        FMOD::Sound* sound_;
        FMOD::Sound* shotSound_;
        FMOD::Channel* channel_;
    
        FMOD_RESULT result_;
        FMOD::System* system_;
    
        float amplitude_;
};


////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& out, const FMOD_VECTOR& v);

namespace FMOD
{
    void check(FMOD_RESULT result);
}



#endif
