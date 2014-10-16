#include "SoundRenderer.h"


////////////////////////////////////////////////////////////////////////////////
SoundRenderer::SoundRenderer()
{
    FMOD::System_Create(&system_);

    result_ = system_->setOutput(FMOD_OUTPUTTYPE_AUTODETECT); FMOD::check(result_);
    result_ = system_->setSpeakerMode(FMOD_SPEAKERMODE_STEREO); FMOD::check(result_);
    result_ = system_->init(64, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0); FMOD::check(result_);
    
    sound_ = createBuffer();
    setListening(true);
}


////////////////////////////////////////////////////////////////////////////////
SoundRenderer::~SoundRenderer() {
    result_ = system_->release(); FMOD::check(result_);
}


////////////////////////////////////////////////////////////////////////////////
FMOD::Sound* SoundRenderer::createBuffer(const float duration) const
{
    FMOD_CREATESOUNDEXINFO exinfo;
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels = 2;
    exinfo.defaultfrequency = 44100;
    exinfo.format = FMOD_SOUND_FORMAT_PCMFLOAT;
    exinfo.length = exinfo.defaultfrequency * sizeof(float) * exinfo.numchannels * duration;
    
    FMOD_RESULT result;
    FMOD::Sound* sound = 0;
    result = system_->createSound(0, FMOD_LOOP_NORMAL | FMOD_3D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound);
    FMOD::check(result_);
    
    return sound;
}


////////////////////////////////////////////////////////////////////////////////
Sound& SoundRenderer::spawnSound()
{
    FMOD::Sound* sound = createBuffer();
    
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
void SoundRenderer::setListenerState(const Renderer::State& state)
{
    vec3f fwd = state[1] - state[0];
    fwd /= fwd.norm();
    
    FMOD_VECTOR lpos = {state[0].x, state[0].y, state[0].z};
    FMOD_VECTOR lfwd = {fwd.x, fwd.y, fwd.z};
    FMOD_VECTOR lup = {state[2].x, state[2].y, state[2].z};
    FMOD_VECTOR lvel = {0, 0, 0};
    
    result_ = system_->set3DListenerAttributes(0, &lpos, &lvel, &lfwd, &lup);
    FMOD::check(result_);
}


////////////////////////////////////////////////////////////////////////////////
void SoundRenderer::setListening(const bool listening)
{
    isListening_ = listening;
    
    if (isListening_)
    {
        system_->recordStart(0, sound_, true);
        system_->playSound(FMOD_CHANNEL_FREE, sound_, false, &channel_);
    } else {
        system_->recordStop(0);
        channel_->stop();
    }
}


////////////////////////////////////////////////////////////////////////////////
void SoundRenderer::clear() {
    sounds_.clear();
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
    
    //  record
//    system_->isRecording(0, &isListening_);
    
//    if (isListening_)
//    {
        //  synchronize channel with recording
        unsigned int position;
        system_->getRecordPosition(0, &position);
        channel_->setPosition(position, FMOD_TIMEUNIT_PCM);
        
        //  compute mean amplitude
        const int nsamples = 1000;
        const int thresh = -4;
        
        float data[nsamples];
        channel_->getWaveData(data, nsamples, 0);
    
        float max = -numeric_limits<float>::max();
        float min = numeric_limits<float>::max();
        
        for (int i = 0; i < nsamples; i++) {
            max = data[i] > max ? data[i] : max;
            min = data[i] < min ? data[i] : min;
        }
        
        float amplitude = log(max - min);

        //  detect start of input
        if (!isRecording_ && amplitude > thresh)
        {
            newSound_ = createBuffer();
            
            system_->recordStop(0);
            system_->recordStart(0, newSound_, false);

            channel_->stop();
            system_->playSound(FMOD_CHANNEL_REUSE, newSound_, false, &channel_);
            
            isRecording_ = true;
            
            cout << "start: " << position << endl;
        }
        //  detect end of input
        else if (isRecording_ && amplitude <= thresh && amplitude > -100)
        {
            system_->recordStop(0);
            system_->recordStart(0, sound_, true);

            channel_->stop();
            system_->playSound(FMOD_CHANNEL_FREE, sound_, false, &channel_);
            
            //  spawn sound with recording
            
            isRecording_ = false;
            sf::sleep(sf::seconds(0.01f));
            cout << "end: " << position << endl;
        }
        
//        sf::sleep(sf::seconds(0.1f));
//    }
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
