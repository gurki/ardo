#include "common.h"
#include "Game.h"

#include <limits>


int main(int argc, char* argv[])
{
//    //  create window
//    //      this must be done first, so that the opengl context can initialise
//    //      before the Game() constructor initialises any rendering states
//    sf::VideoMode mode = sf::VideoMode(1440, 900);
//    string title = "ORDO";
//    sf::ContextSettings settings = sf::ContextSettings(32);
//    
//    sf::Window window(
//        mode,
//        title,
//        sf::Style::Default,
//        settings
//    );
//
//    window.setVerticalSyncEnabled(true);
//    window.setActive();
//    
//    
//    //  create and run game
//    Game game(&window, mode, title, settings);
//    
//    while (game.isRunning())
//    {
//        game.handleEvents();
//        game.update();
//        game.render();
//    }
    
    //  create system
    FMOD_RESULT result_;
    
    FMOD::System* system_;
    FMOD::System_Create(&system_);
    
    result_ = system_->setOutput(FMOD_OUTPUTTYPE_AUTODETECT); FMOD::check(result_);
    result_ = system_->setSpeakerMode(FMOD_SPEAKERMODE_STEREO); FMOD::check(result_);
    result_ = system_->init(64, FMOD_3D | FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0); FMOD::check(result_);

    //  create sound
    FMOD_CREATESOUNDEXINFO exinfo;
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels = 2;
    exinfo.defaultfrequency = 44100;
    exinfo.format = FMOD_SOUND_FORMAT_PCMFLOAT;
    exinfo.length = exinfo.defaultfrequency * sizeof(float) * exinfo.numchannels * 2.0f;
    
    FMOD::Sound* sound = 0;
    result_ = system_->createSound(0, FMOD_3D | FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound); FMOD::check(result_);

    //  record and play
    FMOD::Channel* channel = 0;
    system_->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
    system_->recordStart(0, sound, true);
    channel->setSpeakerMix(5, 5, 5, 5, 5, 5, 5, 5);

    FMOD_VECTOR listenerpos = { 0.0f, 0.0f, 0.0f };
    system_->set3DListenerAttributes(0, &listenerpos, 0, 0, 0);
    
    //  add massive reverb
    FMOD::Reverb* reverb;
    system_->createReverb(&reverb);
    reverb->setActive(true);
    
    FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
    float mindist = 0.0f;
    float maxdist = 100.0f;
    reverb->set3DAttributes(&pos, mindist, maxdist);

    FMOD_REVERB_PROPERTIES properties;
    properties.DecayTime = 100;
    properties.Reverb = 100;
    reverb->setProperties(&properties);
    
    //  loop
    sf::Clock clock;
    
    while (true)
    {
        system_->update();
        
        unsigned int position;
        system_->getRecordPosition(0, &position);
        channel->setPosition(position, FMOD_TIMEUNIT_PCM);

        
        
        sf::sleep(sf::seconds(0.01f));
    }
    
    return 0;
}








