#include "SoundRenderer.h"
#include "Game.h"


////////////////////////////////////////////////////////////////////////////////
SoundRenderer::SoundRenderer()
{
    //  init fmod
    FMOD::System_Create(&system_);

    result_ = system_->setOutput(FMOD_OUTPUTTYPE_AUTODETECT); FMOD::check(result_);
//    result_ = system_->setSpeakerMode(FMOD_SPEAKERMODE_STEREO); FMOD::check(result_);
    result_ = system_->init(64, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0); FMOD::check(result_);
    
    //  start listening
    sound_ = createBuffer(10.0f);
    amplitude_ = 0;
    shouldListen_ = false;
    
    //  add massive reverb
    FMOD::Reverb3D* reverb;
    system_->createReverb3D(&reverb);
    reverb->setActive(true);
    
    FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
    float mindist = 0.0f;
    float maxdist = 100.0f;
    reverb->set3DAttributes(&pos, mindist, maxdist);
    
    FMOD_REVERB_PROPERTIES properties;
    properties.DecayTime = 10;
    properties.EarlyDelay = 10;
    reverb->setProperties(&properties);
}


////////////////////////////////////////////////////////////////////////////////
SoundRenderer::~SoundRenderer() {
    result_ = system_->release(); FMOD::check(result_);
}


////////////////////////////////////////////////////////////////////////////////
void SoundRenderer::shoot()
{
    game->shoot();
    
    const vector<vec2i> path = game->getBoard().getPath(false);
    
    createDSP();
    sounds_.back().setVelocity(10);
    sounds_.back().setPath(path);
}


////////////////////////////////////////////////////////////////////////////////
void SoundRenderer::toggleListening() {
    shouldListen_ = !shouldListen_;
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
    result = system_->createSound(0, FMOD_LOOP_NORMAL | FMOD_3D | FMOD_OPENUSER, &exinfo, &sound);
    FMOD::check(result_);
    
    return sound;
}


////////////////////////////////////////////////////////////////////////////////
Sound& SoundRenderer::createDSP()
{
    FMOD::Sound* sound = createBuffer();
    
    FMOD::DSP* dsp = 0;
    result_ = system_->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &dsp); FMOD::check(result_);
    result_ = dsp->setParameterFloat(FMOD_DSP_OSCILLATOR_RATE, 440.0f); FMOD::check(result_);
    result_ = dsp->setParameterInt(FMOD_DSP_OSCILLATOR_TYPE, Sine); FMOD::check(result_);

    FMOD::Channel* channel = 0;
    result_ = system_->playDSP(dsp, 0, false, &channel); FMOD::check(result_);
    result_ = channel->setMode(FMOD_3D); FMOD::check(result_);
 
    Sound soundObj(sound, channel, dsp);
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
void SoundRenderer::clear() {
    sounds_.clear();
}


////////////////////////////////////////////////////////////////////////////////
void SoundRenderer::playSound(const vec3f &position, const vec3f& velocity, FMOD::Sound* sound)
{    
    FMOD::Channel* channel;
    
    system_->playSound(sound, 0, false, &channel);
    
    FMOD_VECTOR pos = {position.x, position.y, position.z};
    FMOD_VECTOR vel = {velocity.x, velocity.y, velocity.z};
    
    channel->set3DAttributes(&pos, &vel);
    channel->setMode(FMOD_3D | FMOD_LOOP_OFF);
    channel->setPaused(false);
    channel->setMixLevelsOutput(5, 5, 5, 5, 5, 5, 5, 5);
    
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
    
    if ( ! sounds_.empty() ) {
        for (auto& sound : sounds_) {
            sound.update(dt);
        }
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
    
    if (shouldListen_)
    {
        //  detect microphone input
//        static sf::Clock clock;
        
        //  synchronize channel with recording
//        unsigned int position;
//        system_->getRecordPosition(0, &position);
//        channel_->setPosition(position, FMOD_TIMEUNIT_PCM);
//        channel_->setVolume(0);
    }
    
    result_ = system_->update(); FMOD::check(result_);
}


////////////////////////////////////////////////////////////////////////////////
void SoundRenderer::notify(const string &event)
{
    cout << event << endl;
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
