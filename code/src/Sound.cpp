#include "Sound.h"
#include "SoundRenderer.h"


////////////////////////////////////////////////////////////////////////////////
Sound::Sound(FMOD::Sound* sound, SoundRenderer* soundRenderer)
{
    channel_ = nullptr;
    sound_ = sound;
    soundRenderer_ = soundRenderer;
    dsp_ = nullptr;
    
    velocity_ = 10;
}


////////////////////////////////////////////////////////////////////////////////
Sound::Sound(FMOD::Sound* sound, FMOD::Channel* channel, FMOD::DSP* dsp)
{
    channel_ = channel;
    sound_ = sound;
    dsp_ = dsp;
    soundRenderer_ = nullptr;
    
    velocity_ = 10;
}


////////////////////////////////////////////////////////////////////////////////
void Sound::stop()
{
    if (channel_) {
        channel_->stop();
    }
    
//    if (sound_) {
//        sound_->release();
//    }
}


////////////////////////////////////////////////////////////////////////////////
void Sound::setPath(const vector<vec2i>& path)
{
    if (path.size() <= 1) {
//        cout << "empty path" << endl;
        nextAnchor_ = -1;
        stop();
        return;
    }
    
    nextAnchor_ = 1;
    position_ = vec2f(path[0].x, path[0].y);
    path_ = path;
}


////////////////////////////////////////////////////////////////////////////////
void Sound::update(const float dt)
{
    //  done when path is completely travelled
    if (nextAnchor_ < 0 || nextAnchor_ >= path_.size()) {
        nextAnchor_ = -1;
        return;
    }
    
    //  get direction to next anchor
    vec2i next = path_[nextAnchor_];
    vec2f d1 = vec2f(next.x - position_.x, next.y - position_.y);
    
    //  if we are at an anchor, continue to next one
    if (d1.x == 0 && d1.y == 0) {
        nextAnchor_++;
        return;
    }
    
    //  move forward
    vec2f v = d1 / d1.norm() * velocity_;
    position_ += dt * v;
    
    //  test if we shot over the anchor
    vec2f d2 = vec2f(next.x - position_.x, next.y - position_.y);
    
    if (dot(d1, d2) <= 0)
    {
        position_ = vec2f(next.x, next.y);
        nextAnchor_++;
    
        if (soundRenderer_)
        {
            //  turn! play sound
            vec3f pos(position_.x, 0.5f, position_.y);
            vec3f vel(0, 0, 0);
            
            soundRenderer_->playSound(pos, vel, sound_);
        }
        else
        {
            if (nextAnchor_ >= path_.size()) {
                stop();
            }
        }
        
        if (dsp_ != nullptr) {
            float value;
            dsp_->getParameterFloat(FMOD_DSP_OSCILLATOR_RATE, &value, nullptr, 0);
            dsp_->setParameterFloat(FMOD_DSP_OSCILLATOR_RATE, value * 1.1f);
        }
    }
    
    if (channel_ != nullptr)
    {
        //  update sound position and velocity accordingly
        FMOD_VECTOR fpos = {static_cast<float>(position_.x), 0.5f, static_cast<float>(position_.y)};
        FMOD_VECTOR fvel = {static_cast<float>(v.x), 0, static_cast<float>(v.y)};
        channel_->set3DAttributes(&fpos, &fvel);
    }
}


////////////////////////////////////////////////////////////////////////////////
const bool Sound::isAlive() const {
    return nextAnchor_ >= 0;
}


////////////////////////////////////////////////////////////////////////////////
const bool Sound::isMoving() const {
    return nextAnchor_ > 0;
}


