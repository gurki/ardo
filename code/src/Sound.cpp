#include "Sound.h"


////////////////////////////////////////////////////////////////////////////////
Sound::Sound(FMOD::Channel* channel, FMOD::Sound* sound, FMOD::DSP* dsp)
{
    channel_ = channel;
    sound_ = sound;
    dsp_ = dsp;
    
    nextAnchor_ = 0;
    velocity_ = 10.0f;
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
        cout << "empty path" << endl;
        nextAnchor_ = -1;
        return;
    }
    
    nextAnchor_ = 0;
    position_ = vec2f(path[0].x, path[0].y);
    
    if (path.size() > 1) {
        path_ = path;
        nextAnchor_ = 1;
    }
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
        
        float value;
        dsp_->getParameter(FMOD_DSP_OSCILLATOR_RATE, &value, nullptr, 0);
        dsp_->setParameter(FMOD_DSP_OSCILLATOR_RATE, value * 1.141f);
    }
    
    //  update sound position and velocity accordingly
    FMOD_VECTOR fpos = {static_cast<float>(position_.x), 0.5f, static_cast<float>(position_.y)};
    FMOD_VECTOR fvel = {static_cast<float>(v.x), 0, static_cast<float>(v.y)};
    channel_->set3DAttributes(&fpos, &fvel);
}


////////////////////////////////////////////////////////////////////////////////
const bool Sound::isAlive() const {
    return nextAnchor_ >= 0;
}


////////////////////////////////////////////////////////////////////////////////
const bool Sound::isMoving() const {
    return nextAnchor_ > 0;
}


