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
    result_ = system_->init(64, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0); FMOD::check(result_);

    //  create sound
    FMOD_CREATESOUNDEXINFO exinfo;
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels = 2;
    exinfo.defaultfrequency = 44100;
    exinfo.format = FMOD_SOUND_FORMAT_PCMFLOAT;
    exinfo.length = exinfo.defaultfrequency * sizeof(float) * exinfo.numchannels * 2.0f;
    
    FMOD::Sound* sound = 0;
    result_ = system_->createSound(0, FMOD_LOOP_NORMAL | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound); FMOD::check(result_);

    //  record and play
    FMOD::Channel* channel = 0;
    system_->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
    system_->recordStart(0, sound, true);
    
    sf::Clock clock;
    int rec = 0;
    unsigned int start = 0;
    unsigned int end = 0;
    
    while (true)
    {
        system_->update();
        
        unsigned int position;
        system_->getRecordPosition(0, &position);
        
        if (rec < 2) {
            channel->setPosition(position, FMOD_TIMEUNIT_PCM);
        }
        
        //  compute mean amplitude
        const int nsamples = 1000;
        const int thresh = -4;
        
        float data[nsamples];
        channel->getWaveData(data, nsamples, 0);
        
        float max = -numeric_limits<float>::max();
        float min = numeric_limits<float>::max();
        
        for (int i = 0; i < nsamples; i++) {
            max = data[i] > max ? data[i] : max;
            min = data[i] < min ? data[i] : min;
        }
        
        float amp = log(max - min);
        
        if (rec == 0 && amp > thresh)
        {
            cout << position << endl;
//            system_->recordStop(0);
//            system_->recordStart(0, sound1, true);
            start = position;
            rec = 1;
            
            cout << clock.getElapsedTime().asSeconds() << ": start" << endl;
        }
        else if (rec == 1 && amp <= thresh)
        {
            end = position;
            cout<< position << endl;
            
            system_->recordStop(0);
            channel->setLoopPoints(start, FMOD_TIMEUNIT_PCM, end, FMOD_TIMEUNIT_PCM);
            channel->setLoopCount(-1);
            rec = 2;
            
            cout << clock.getElapsedTime().asSeconds() << ": stop" << endl;
        }
        
        sf::sleep(sf::seconds(0.01f));
    }
    
    return 0;
}








