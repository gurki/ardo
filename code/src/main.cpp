#include "board.h"
#include "renderer.h"
#include "sound.h"

#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{
    //  create world
    Board board(8, 8);
    board.initBallsRandom(5);
    
    Renderer renderer;
    
    //  create window
    bool fullscreen = false;
    
    sf::VideoMode dmode(1440, 900);
    sf::VideoMode fmode = sf::VideoMode::getFullscreenModes()[0];
    string title = "ORDO";
    sf::ContextSettings settings(32);
    
    sf::Window window(dmode, title, sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    
    //  start main loop
    sf::Clock clock;
    
    while (window.isOpen())
    {
        //  process events
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            //  close window -> exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            //  handle keys
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    //  close on escape
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                        
                    //  fullscreen
                    case sf::Keyboard::F:
                    {
                        if (fullscreen) {
                            window.create(dmode, title, sf::Style::Default, settings);
                        } else {
                            window.create(fmode, title, sf::Style::Fullscreen, settings);
                        }
                        
                        fullscreen = !fullscreen;
                        break;
                    }
                        
                    //  move player
                    case sf::Keyboard::Right:
                        board.movePlayerRight();
                        break;
                    
                    case sf::Keyboard::Left:
                        board.movePlayerLeft();
                        break;
                        
                    //  shoot
                    case sf::Keyboard::Space:
                        cout << "SHOOT!" << endl;
                        break;
                        
                    default:
                        break;
                }
            }
            
            //  resize viewport
            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
        
        //  render everything
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        renderer.drawBoard(board);
        
        window.display();
    }
    
    
    
    
//    FMOD_RESULT result;
//    
//    //  init system
//    FMOD::System* system = 0;
//    FMOD::System_Create(&system);
//    
//    result = system->setOutput(FMOD_OUTPUTTYPE_COREAUDIO); FMOD::check(result);
//    result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO); FMOD::check(result);
//    result = system->init(64, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0); FMOD::check(result);
//    
//    //  create sound
//    FMOD_CREATESOUNDEXINFO exinfo;
//    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
//    
//    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
//    exinfo.numchannels = 2;
//    exinfo.defaultfrequency = 44100;
//    exinfo.format = FMOD_SOUND_FORMAT_PCMFLOAT;
//    exinfo.length = exinfo.defaultfrequency * sizeof(float) * exinfo.numchannels * 1.0f;
//    
//    FMOD::Sound* sound = 0;
//    result = system->createSound(0, FMOD_3D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound); FMOD::check(result);
//
//    FMOD::DSP* dsp = 0;
//    result = system->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &dsp); FMOD::check(result);
//    result = dsp->setParameter(FMOD_DSP_OSCILLATOR_RATE, 440.0f); FMOD::check(result);
//    result = dsp->setParameter(FMOD_DSP_OSCILLATOR_TYPE, 2); FMOD::check(result);
//    
//    FMOD::Channel* channel = 0;
//    result = system->playDSP(FMOD_CHANNEL_FREE, dsp, false, &channel); FMOD::check(result);
//    
//    //  create geometry
//    FMOD::Geometry* geometry;
//    result = system->setGeometrySettings(100); FMOD::check(result);
//    result = system->createGeometry(1024, 4096, &geometry); FMOD::check(result);
//    
//    int index = 0;
////    FMOD_VECTOR vertices[4] = {{-1, -1,-1}, {1, -1,-1}, {1, 1,-1}, {-1, 1,-1}};
//    FMOD_VECTOR vertices[4] = {{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}};
//    geometry->addPolygon(1.0, 1.0, true, 4, vertices, &index);
//    geometry->setPolygonAttributes(0, 0.9, 0.9, true);
//
//    //  position objects
//    FMOD_VECTOR lpos = {0, 0, 0};
//    FMOD_VECTOR lvel = {0, 0, 0};
//    FMOD_VECTOR lfwd = {0, 0, 1};
//    FMOD_VECTOR lup = {0, 1, 0};
//    result = system->set3DListenerAttributes(0, &lpos, &lvel, &lfwd, &lup);
//    
//    float angle = 0.0f;
//    std::random_device rnd;
//    std::uniform_int_distribution<> uni(0, 1);
//    int dir = 2 * (uni(rnd)) - 1;
//    float v0 = dir * 2.0f;
//    float radius = 10.0f;
//    
//    std::cout << dir << std::endl;
//    
//    FMOD_VECTOR cpos = {0, 0, radius};
//    FMOD_VECTOR cvel = {v0, 0, 0};
//    result = channel->setMode(FMOD_3D); FMOD::check(result);
//    result = channel->set3DAttributes(&cpos, &cvel); FMOD::check(result);
//    
//    //  main simulation
//    using namespace std::chrono;
//    high_resolution_clock::time_point t0 = high_resolution_clock::now();
//    
//    while (true)
//    {
//        if (kbhit()) {
//            break;
//        }
//        
//        //  update timer
//        high_resolution_clock::time_point t1 = high_resolution_clock::now();
//        float dt = duration_cast<milliseconds>(t1 - t0).count() / 1000.0f;
//        t0 = t1;
//        
//        //  move source
//        angle = angle + dt * v0;
//        cpos.x = radius * sinf(angle);
//        cpos.z = radius * cosf(angle);
//        cvel.x = v0 * cosf(angle);
//        cvel.z = v0 * sinf(angle);
//        
//        channel->set3DAttributes(&cpos, &cvel);
//        
////        std::cout << cpos << std::endl;
//        
//        //  update engine
//        result = system->update(); FMOD::check(result);
//        sleep(0.01f);
//    }
//
//    //  release resources
//    result = dsp->release(); FMOD::check(result);
//    result = system->release(); FMOD::check(result);
    
    return 0;
}