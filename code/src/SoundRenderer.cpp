#include "SoundRenderer.h"


////////////////////////////////////////////////////////////////////////////////
SoundRenderer::SoundRenderer()
{
    FMOD::System_Create(&system_);

    result_ = system_->setOutput(FMOD_OUTPUTTYPE_AUTODETECT); FMOD::check(result_);
    result_ = system_->setSpeakerMode(FMOD_SPEAKERMODE_STEREO); FMOD::check(result_);
    result_ = system_->init(64, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0); FMOD::check(result_);
}


////////////////////////////////////////////////////////////////////////////////
SoundRenderer::~SoundRenderer() {
    result_ = system_->release(); FMOD::check(result_);
}


////////////////////////////////////////////////////////////////////////////////
Sound& SoundRenderer::spawnSound()
{
    FMOD_CREATESOUNDEXINFO exinfo;
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels = 2;
    exinfo.defaultfrequency = 44100;
    exinfo.format = FMOD_SOUND_FORMAT_PCMFLOAT;
    exinfo.length = exinfo.defaultfrequency * sizeof(float) * exinfo.numchannels * 1.0f;
    
    FMOD::Sound* sound = 0;
    result_ = system_->createSound(0, FMOD_3D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound); FMOD::check(result_);
    
    FMOD::DSP* dsp = 0;
    result_ = system_->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &dsp); FMOD::check(result_);
    result_ = dsp->setParameter(FMOD_DSP_OSCILLATOR_RATE, 440.0f); FMOD::check(result_);
    result_ = dsp->setParameter(FMOD_DSP_OSCILLATOR_TYPE, Sine); FMOD::check(result_);
    
    FMOD::Channel* channel = 0;
    result_ = system_->playDSP(FMOD_CHANNEL_FREE, dsp, false, &channel); FMOD::check(result_);
    result_ = channel->setMode(FMOD_3D); FMOD::check(result_);
 
    Sound soundObj(channel, sound, dsp);
    sounds_.push_back(soundObj);
    
    return sounds_.back();
}


////////////////////////////////////////////////////////////////////////////////
void SoundRenderer::setListener(const Board &board)
{
    vec2i pos = board.getPlayerCenter();
    vec2i eye = board.getPlayerEyes();
    vec2i fwd = eye - pos;
    
    FMOD_VECTOR lpos = {static_cast<float>(pos.x), 0, static_cast<float>(pos.y)};
    FMOD_VECTOR lvel = {0, 0, 0};
    FMOD_VECTOR lfwd = {static_cast<float>(fwd.x), 0, static_cast<float>(fwd.y)};
    FMOD_VECTOR lup = {0, 1, 0};
    
    result_ = system_->set3DListenerAttributes(0, &lpos, &lvel, &lfwd, &lup); FMOD::check(result_);
}


////////////////////////////////////////////////////////////////////////////////
void SoundRenderer::update(const float dt)
{    
    result_ = system_->update(); FMOD::check(result_);
    
    for(auto iter = sounds_.begin(); iter != sounds_.end();)
    {
        iter->update(dt);
        
        if(!iter->isAlive()) {
            iter->stop();
            iter = sounds_.erase(iter);
        } else {
            ++iter;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& out, const FMOD_VECTOR& v) {
    out << v.x << ", " << v.y << ", " << v.z << std::endl;
    return out;
}


////////////////////////////////////////////////////////////////////////////////
namespace FMOD
{
    void check(FMOD_RESULT result)
    {
        if (result != FMOD_OK) {
            printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            exit(-1);
        }
    }
}
