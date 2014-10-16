#ifndef SOUND_H
#define SOUND_H

#include "common.h"
#include "vector.h"

#include <wincompat.h>
#include <fmod.hpp>
#include <fmod_errors.h>


class SoundRenderer;


class Sound
{
    public:
    
        Sound(FMOD::Sound* sound, SoundRenderer* soundRenderer);
        Sound(FMOD::Sound* sound, FMOD::Channel* channel, FMOD::DSP* dsp);
    
        void setPath(const vector<vec2i>& path);
        void setVelocity(const float velocity) { velocity_ = velocity; };
    
        const bool isAlive() const;
        const bool isMoving() const;
    
        void stop();
        void update(const float dt);

    
    private:
    
        SoundRenderer* soundRenderer_;
    
        FMOD::Channel* channel_;
        FMOD::Sound* sound_;
        FMOD::DSP* dsp_;
    
        vec2f position_;
        float velocity_;
    
        int nextAnchor_;
        vector<vec2i> path_;
};


#endif
