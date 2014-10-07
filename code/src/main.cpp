#include "board.h"
#include "renderer.h"
#include "sound.h"

#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <chrono>


int main(int argc, char* argv[])
{
    // Request a 32-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    
    
    

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "ORDO", sf::Style::Default, contextSettings);
    window.setVerticalSyncEnabled(true);

    // Make it the active window for OpenGL calls
    window.setActive();
    
    // Create a clock for measuring the time elapsed
    sf::Clock clock;
    
//    // Start game loop
//    while (window.isOpen())
//    {
//        // Process events
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            // Close window : exit
//            if (event.type == sf::Event::Closed)
//                window.close();
//            
//            // Escape key : exit
//            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
//                window.close();
//            
//            // Adjust the viewport when the window is resized
//            if (event.type == sf::Event::Resized)
//                glViewport(0, 0, event.size.width, event.size.height);
//        }
//        
//        // Clear the depth buffer
//        glClear(GL_COLOR_BUFFER_BIT);
//        
//        // Draw the cube
//        sf::CircleShape circle(100.0f);
//        circle.setFillColor(sf::Color::Green);
//        
//        window.draw(circle);
//        
//        // Finally, display the rendered frame on screen
//        window.display();
//    }
//    
    
    
    
    FMOD_RESULT result;
    
    //  init system
    FMOD::System* system = 0;
    FMOD::System_Create(&system);
    
    result = system->setOutput(FMOD_OUTPUTTYPE_COREAUDIO); FMOD::check(result);
    result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO); FMOD::check(result);
    result = system->init(64, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0); FMOD::check(result);
    
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

    FMOD::DSP* dsp = 0;
    result = system->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &dsp); FMOD::check(result);
    result = dsp->setParameter(FMOD_DSP_OSCILLATOR_RATE, 440.0f); FMOD::check(result);
    result = dsp->setParameter(FMOD_DSP_OSCILLATOR_TYPE, 2); FMOD::check(result);
    
    FMOD::Channel* channel = 0;
    result = system->playDSP(FMOD_CHANNEL_FREE, dsp, false, &channel); FMOD::check(result);
    
    //  create geometry
    FMOD::Geometry* geometry;
    result = system->setGeometrySettings(100); FMOD::check(result);
    result = system->createGeometry(1024, 4096, &geometry); FMOD::check(result);
    
    int index = 0;
//    FMOD_VECTOR vertices[4] = {{-1, -1,-1}, {1, -1,-1}, {1, 1,-1}, {-1, 1,-1}};
    FMOD_VECTOR vertices[4] = {{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}};
    geometry->addPolygon(1.0, 1.0, true, 4, vertices, &index);
    geometry->setPolygonAttributes(0, 0.9, 0.9, true);

    //  position objects
    FMOD_VECTOR lpos = {0, 0, 0};
    FMOD_VECTOR lvel = {0, 0, 0};
    FMOD_VECTOR lfwd = {0, 0, 1};
    FMOD_VECTOR lup = {0, 1, 0};
    result = system->set3DListenerAttributes(0, &lpos, &lvel, &lfwd, &lup);
    
    float angle = 0.0f;
    std::random_device rnd;
    std::uniform_int_distribution<> uni(0, 1);
    int dir = 2 * (uni(rnd)) - 1;
    float v0 = dir * 2.0f;
    float radius = 10.0f;
    
    std::cout << dir << std::endl;
    
    FMOD_VECTOR cpos = {0, 0, radius};
    FMOD_VECTOR cvel = {v0, 0, 0};
    result = channel->setMode(FMOD_3D); FMOD::check(result);
    result = channel->set3DAttributes(&cpos, &cvel); FMOD::check(result);
    
    //  main simulation
    using namespace std::chrono;
    high_resolution_clock::time_point t0 = high_resolution_clock::now();
    
    while (true)
    {
        if (kbhit()) {
            break;
        }
        
        //  update timer
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        float dt = duration_cast<milliseconds>(t1 - t0).count() / 1000.0f;
        t0 = t1;
        
        //  move source
        angle = angle + dt * v0;
        cpos.x = radius * sinf(angle);
        cpos.z = radius * cosf(angle);
        cvel.x = v0 * cosf(angle);
        cvel.z = v0 * sinf(angle);
        
        channel->set3DAttributes(&cpos, &cvel);
        
//        std::cout << cpos << std::endl;
        
        //  update engine
        result = system->update(); FMOD::check(result);
        sleep(0.01f);
    }

    //  release resources
    result = dsp->release(); FMOD::check(result);
    result = system->release(); FMOD::check(result);
    
    return 0;
}