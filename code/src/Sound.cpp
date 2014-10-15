#include "Sound.h"


////////////////////////////////////////////////////////////////////////////////
Sound::Sound(FMOD::Channel* channel, FMOD::Sound* sound, FMOD::DSP* dsp)
{
    channel_ = channel;
    sound_ = sound;
    dsp_ = dsp;
    
    nextAnchor_ = -1;
    velocity_ = 10.0f;
}


////////////////////////////////////////////////////////////////////////////////
Sound::~Sound()
{   
//    if (channel_ != nullptr) {
//        channel_->stop();
//    }
    
//    if (sound_ != nullptr) {
//        sound_->release();
//    }
//    
//    if (dsp_ != nullptr) {
//        dsp_->release();
//    }
}


////////////////////////////////////////////////////////////////////////////////
void Sound::stop()
{
    if (channel_) {
        channel_->stop();
    }
}


////////////////////////////////////////////////////////////////////////////////
void Sound::setPath(const vector<vec2i>& path)
{
    if (path.empty()) {
        return;
    }
    
    position_ = vec2f(path[0].x, path[0].y);
    
    if (path.size() > 1) {
        path_ = path;
        nextAnchor_ = 1;
    }
}


////////////////////////////////////////////////////////////////////////////////
void Sound::update(const float dt)
{
//    cout << nextAnchor_ << endl;
    
    if (nextAnchor_ < 0) {
        return;
    }
    
    vec2i next = path_[nextAnchor_];
    vec2f d1 = vec2f(next.x - position_.x, next.y - position_.y);
    vec2f v = d1 / d1.norm() * velocity_;
    
    position_ += dt * v;
    
    vec2f d2 = vec2f(next.x - position_.x, next.y - position_.y);
    
//    cout << position_ << ", " << next << ", " << d1 << endl;
    
    if (dot(d1, d2) < 0)
    {
        position_ = vec2f(next.x, next.y);
        nextAnchor_++;
        
        if (nextAnchor_ >= path_.size()) {
            nextAnchor_ = -1;
            v = vec2f();
        }
    }
    
    FMOD_VECTOR fpos = {static_cast<float>(position_.x), 0.5f, static_cast<float>(position_.y)};
    FMOD_VECTOR fvel = {static_cast<float>(v.x), 0, static_cast<float>(v.y)};
    
    channel_->set3DAttributes(&fpos, &fvel);
}


////////////////////////////////////////////////////////////////////////////////
const bool Sound::isAlive() const {
    return path_.empty() || isMoving();
}


////////////////////////////////////////////////////////////////////////////////
const bool Sound::isMoving() const {
    return nextAnchor_ >= 0;
}


