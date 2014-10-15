#include "Game.h"


////////////////////////////////////////////////////////////////////////////////
Game::Game(
    sf::Window* window,
    const sf::VideoMode& mode,
    const string& title,
    const sf::ContextSettings& settings
) :
    window_(window),
    windowMode_(mode),
    windowTitle_(title),
    windowSettings_(settings),
    board_(8, 8, 5),
    isRunning_(true)
{
    //  initialise flags
    flags_["fullscreen"] = false;
    flags_["balls"] = true;
    flags_["path"] = true;
    flags_["fpv"] = true;
    
    //  initialise world
    board_.init(8, 8);
    board_.initBallsRandom(5);
    
    clock_.restart();
}


////////////////////////////////////////////////////////////////////////////////
void Game::update()
{
    const float dt = clock_.getElapsedTime().asSeconds();
    
    //  play sounds
    soundRenderer_.setListener(board_);
    soundRenderer_.update(dt);
    
    clock_.restart();
}


////////////////////////////////////////////////////////////////////////////////
void Game::render()
{
    //  render everything
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    renderer_.setViewActive(board_);
    renderer_.drawBoard(board_);
    renderer_.drawPlayer(board_.getPlayerCenter());
    
    if (flags_["balls"]) {
        renderer_.drawBalls(board_.getBalls());
    }
    
    if (flags_["path"]) {
        renderer_.drawPath(board_.getPath());
    }
    
    window_->display();
}


////////////////////////////////////////////////////////////////////////////////
void Game::handleEvents()
{
    //  process events
    sf::Event event;
    
    while (window_->pollEvent(event))
    {
        //  close window -> exit
        if (event.type == sf::Event::Closed) {
            window_->close();
            isRunning_ = false;
        }
        
        //  handle keys
        if (event.type == sf::Event::KeyPressed) {
            handleKeyboardEvents(event);
        }
        
        //  resize viewport
        if (event.type == sf::Event::Resized) {
            glViewport(0, 0, event.size.width, event.size.height);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void Game::handleKeyboardEvents(const sf::Event& event)
{
    switch (event.key.code)
    {
        //  close on escape
        case sf::Keyboard::Escape:
            window_->close();
            isRunning_ = false;
            break;
            
        //  move player
        case sf::Keyboard::Right:
            board_.movePlayerRight();
            break;
            
        case sf::Keyboard::Left:
            board_.movePlayerLeft();
            break;
            
        //  spawn new balls
        case sf::Keyboard::R:
            board_.initBallsRandom(5);
            break;
            
        //  shoot
        case sf::Keyboard::Space:
        {
//            cout << "SHOOT!" << endl;
//            cout << board_.getPath() << endl;
            Sound& soundObj = soundRenderer_.spawnSound();
            soundObj.setPath(board_.getPath());
            break;
        }
            
        //  fullscreen
        case sf::Keyboard::F:
        {
            if (flags_["fullscreen"]) {
                window_->create(windowMode_, windowTitle_, sf::Style::Default, windowSettings_);
            } else {
                sf::VideoMode mode = sf::VideoMode::getFullscreenModes()[0];
                window_->create(mode, windowTitle_, sf::Style::Fullscreen, windowSettings_);
            }
            
            flags_["fullscreen"] = !flags_["fullscreen"];
            break;
        }
            
        //  balls
        case sf::Keyboard::B:
            flags_["balls"] = !flags_["balls"];
            break;
            
        //  path
        case sf::Keyboard::P:
            flags_["path"] = !flags_["path"];
            break;
            
        //  first / third person view
        case sf::Keyboard::V:
            flags_["fpv"] = !flags_["fpv"];
            renderer_.setViewType(flags_["fpv"]);
            break;
            
        default:
            break;
    }
}