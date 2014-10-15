#ifndef SOUND_H
#define SOUND_H

#include "common.h"
#include "vector.h"

#include <wincompat.h>
#include <fmod.hpp>
#include <fmod_errors.h>


class Sound
{
    public:
    
        Sound(FMOD::Channel* channel, FMOD::Sound* sound = nullptr, FMOD::DSP* dsp = nullptr);
    
        void setPath(const vector<vec2i>& path);
        void update(const float dt);
    
        const bool isAlive() const;
        const bool isMoving() const;
    
        void stop();

    
    private:
    
        FMOD::Channel* channel_;
        FMOD::Sound* sound_;
        FMOD::DSP* dsp_;
    
        vec2f position_;
        float velocity_;
    
        int nextAnchor_;
        vector<vec2i> path_;
};


#endif
