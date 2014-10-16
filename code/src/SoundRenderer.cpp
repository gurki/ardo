#include "SoundRenderer.h"
#include "Game.h"


////////////////////////////////////////////////////////////////////////////////
SoundRenderer::SoundRenderer()
{
    FMOD::System_Create(&system_);

    result_ = system_->setOutput(FMOD_OUTPUTTYPE_AUTODETECT); FMOD::check(result_);
    result_ = system_->setSpeakerMode(FMOD_SPEAKERMODE_STEREO); FMOD::check(result_);
    result_ = system_->init(64, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0); FMOD::check(result_);
    
    sound_ = createBuffer();
    system_->recordStart(0, sound_, true);
    system_->playSound(FMOD_CHANNEL_FREE, sound_, false, &channel_);
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
 
//    Sound soundObj(channel, sound, dsp);
//    sounds_.push_back(soundObj);
    
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
void SoundRenderer::clear() {
    sounds_.clear();
}


////////////////////////////////////////////////////////////////////////////////
void SoundRenderer::playSound(const vec3f &position, const vec3f& velocity, FMOD::Sound* sound)
{    
    FMOD::Channel* channel;
    system_->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
    
    FMOD_VECTOR pos = {position.x, position.y, position.z};
    FMOD_VECTOR vel = {velocity.x, velocity.y, velocity.z};
    channel->set3DAttributes(&pos, &vel);
    channel->setMode(FMOD_3D | FMOD_LOOP_OFF);
    channel->setPaused(false);
    
    channels_.push_back(channel);
}


////////////////////////////////////////////////////////////////////////////////
void SoundRenderer::update(const float dt)
{
    //  update sound balls
    if (sounds_.size() >= 64) {
        sounds_.front().stop();
        sounds_.pop_front();
    }
    
    for (auto& sound : sounds_) {
        sound.update(dt);
    }
    
    //  remove finished channels of one shot sounds
    for(auto iter = channels_.begin(); iter != channels_.end();)
    {
        bool isPlaying = false;
        (*iter)->isPlaying(&isPlaying);
        
        if(!isPlaying) {
            iter = channels_.erase(iter);
        } else {
            ++iter;
        }
    }
    
    //  detect microphone input
    static sf::Clock clock;
    
    //  synchronize channel with recording
    unsigned int position;
    system_->getRecordPosition(0, &position);
    channel_->setPosition(position, FMOD_TIMEUNIT_PCM);
    channel_->setVolume(0);
    
    //  compute mean amplitude
    const int nsamples = 1000;
    const int thresh = -7;
    
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
        //  wait a certain amount of time between shots
        if (clock.getElapsedTime().asSeconds() < 0.1f) {
            return;
        }
        
        //  record following audio to seperate buffer
        shotSound_ = createBuffer(10);
        
        system_->recordStop(0);
        system_->recordStart(0, shotSound_, false);

        channel_->stop();
        system_->playSound(FMOD_CHANNEL_REUSE, shotSound_, false, &channel_);
        
        //  spawn sound with recording
        const float velocity = 5 * (amplitude + 8);
        
        game->shoot();
        
        Sound sound(shotSound_, this);
        sound.setVelocity(velocity);
        sound.setPath(game->getBoard().getPath(false));
        
        sounds_.push_back(sound);
        
        isRecording_ = true;
//        cout << "start" << endl;
    }
    //  detect end of input
    else if (isRecording_ && amplitude <= thresh && amplitude > -100)
    {
        //  switch back to listening
        system_->recordStop(0);
        system_->recordStart(0, sound_, true);

        channel_->stop();
        system_->playSound(FMOD_CHANNEL_FREE, sound_, false, &channel_);
        
        clock.restart();
        isRecording_ = false;
//        cout << "end " << position << endl;
    }
    
    result_ = system_->update(); FMOD::check(result_);
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
