#include "Board.h"
#include "Renderer.h"
#include "Sound.h"
#include "SoundRenderer.h"

#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{
    //  create world
    Board board(8, 8, 5);
    Renderer renderer;
    SoundRenderer soundRenderer;
    
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
                        
                    //  spawn new balls
                    case sf::Keyboard::R:
                        board.initBallsRandom(5);
                        break;
                        
                    //  shoot
                    case sf::Keyboard::Space:
                    {
//                        cout << "SHOOT!" << endl;
//                        cout << board.getPath() << endl;
                        Sound& soundObj = soundRenderer.spawnSound();
                        soundObj.setPath(board.getPath());
                        break;
                    }
                        
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
        
        renderer.setBoardFPV(board);
        renderer.drawBoard(board);
        renderer.drawBalls(board.getBalls());
        renderer.drawPath(board.getPath());
        
        window.display();
        
        //  play sounds
        soundRenderer.setListener(board);
        soundRenderer.update(clock.getElapsedTime().asSeconds());
        
        //  reset timer
        clock.restart();
    }
    
    
    return 0;
}