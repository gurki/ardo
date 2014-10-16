#include "common.h"
#include "Game.h"

#include <limits>
#include <deque>


int main(int argc, char* argv[])
{
    //  create window
    //      this must be done first, so that the opengl context can initialise
    //      before the Game() constructor initialises any rendering states
    sf::VideoMode mode = sf::VideoMode(1440, 900);
    string title = "ORDO";
    sf::ContextSettings settings = sf::ContextSettings(32);
    
    sf::Window window(
        mode,
        title,
        sf::Style::Default,
        settings
    );

    window.setVerticalSyncEnabled(true);
    window.setActive();
    
    //  create and run game
    Game game(&window, mode, title, settings);
    
    while (game.isRunning())
    {
        game.handleEvents();
        game.update();
        game.render();
    }
    
    
//    const float v = 1.0f;
//    FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
//    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
//    FMOD_VECTOR chvel = { 0.0f, 0.0f, -v };
//    FMOD_VECTOR fwd = { 0.0f, 0.0f,-1.0f };
//    FMOD_VECTOR up = { 0.0f, 1.0f, 1.0f };
//    
//    //  create system
//    FMOD_RESULT result_;
//    
//    FMOD::System* system_;
//    FMOD::System_Create(&system_);
//    
//    result_ = system_->setOutput(FMOD_OUTPUTTYPE_AUTODETECT); FMOD::check(result_);
//    result_ = system_->setSpeakerMode(FMOD_SPEAKERMODE_STEREO); FMOD::check(result_);
//    result_ = system_->init(64, FMOD_3D | FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0); FMOD::check(result_);
//
//    //  create sound
//    FMOD_CREATESOUNDEXINFO exinfo;
//    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
//    
//    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
//    exinfo.numchannels = 2;
//    exinfo.defaultfrequency = 44100;
//    exinfo.format = FMOD_SOUND_FORMAT_PCMFLOAT;
//    exinfo.length = exinfo.defaultfrequency * sizeof(float) * exinfo.numchannels * 2.0f;
//    
//    FMOD::Sound* sound = 0;
//    result_ = system_->createSound(0, FMOD_3D | FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound); FMOD::check(result_);
//
//    //  record and play
//    system_->recordStart(0, sound, true);
//    system_->set3DListenerAttributes(0, &pos, 0, &fwd, &up);
//    
//    //  add massive reverb
//    FMOD::Reverb* reverb;
//    system_->createReverb(&reverb);
//    reverb->setActive(true);
//    
//    float mindist = 0.0f;
//    float maxdist = 100.0f;
//    reverb->set3DAttributes(&pos, mindist, maxdist);
//
//    FMOD_REVERB_PROPERTIES properties;
//    properties.DecayTime = 10;
//    properties.Reverb = 10;
//    reverb->setProperties(&properties);
//    
//    //  loop
//    sf::Clock clock1, clock2;
//    deque<FMOD::Channel*> channels;
//    
//    while (true)
//    {
//        const float dt1 = clock1.getElapsedTime().asSeconds();
//        const float dt2 = clock2.getElapsedTime().asSeconds();
//        
//        if (dt1 > 0.2f)
//        {
//            if (channels.size() >= 64) {
//                channels.front()->stop();
//                channels.pop_front();
//            }
//            
//            channels.push_back(nullptr);
//            system_->playSound(FMOD_CHANNEL_FREE, sound, false, &channels.back());
//            channels.back()->setSpeakerMix(5, 5, 5, 5, 5, 5, 5, 5);
//            
//            clock1.restart();
//        }
//        
//        unsigned int position;
//        system_->getRecordPosition(0, &position);
//        
//        for (int i = 0; i < channels.size(); i++)
//        {
//            FMOD::Channel* channel = channels[i];
//            
//            channel->setPosition(position, FMOD_TIMEUNIT_PCM);
//            
//            FMOD_VECTOR oldPos;
//            channel->get3DAttributes(&oldPos, &chvel);
//            
//            oldPos.x += v * dt2;
//            cout << oldPos.x << endl;
//            channel->set3DAttributes(&oldPos, &chvel);
//        }
//        
//        clock2.restart();
//        system_->update();
//        
//        sf::sleep(sf::seconds(0.01f));
//    }
    
    return 0;
}








