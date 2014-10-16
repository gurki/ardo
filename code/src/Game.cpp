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
    board_(8, 8, 5)
{
    //  initialise flags
    flags_["fullscreen"] = false;
    flags_["balls"] = false;
    flags_["path"] = false;
    flags_["fpv"] = true;
    flags_["guessing"] = false;
    flags_["running"] = true;
    
    //  initialise world
//    board_.init(8, 8);
//    board_.initBallsRandom(5);
    soundRenderer_.game = this;
    
    clock_.restart();
}


////////////////////////////////////////////////////////////////////////////////
void Game::update()
{
    const float dt = clock_.getElapsedTime().asSeconds();
    
    //  set guess orientation to player orientation
    if (renderer_.isFirstPerson()) {
        marker_.orientation = board_.getPlayerDirection();
    } else {
        marker_.orientation = Up;
    }
    
    //  update sound positions and play sounds
    Renderer::State state = renderer_.getPlayerState(board_);
    soundRenderer_.setListenerState(state);
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
        renderer_.drawPath(board_.getPath(true));
    }
    
    if (flags_["guessing"]) {
        renderer_.drawGuesses(board_.getGuesses());
        renderer_.drawMarker(marker_.position);
    }
    
    window_->display();
}


////////////////////////////////////////////////////////////////////////////////
void Game::shoot()
{
    //  get path
    const vector<vec2i> path = board_.getPath(false);
    
    //  spawn and shoot sound
//    Sound& soundObj = soundRenderer_.spawnSound();
//    soundObj.setPath(path);
    
    //  keep track of points
    const vec2i& first = *path.begin();
    const vec2i& last = *path.end();
    
    //  reflection
    if (first == last) {
        points_++;
    //  hit
    } else if (!board_.isBorder(last)) {
        points_++;
    //  detour
    } else {
        points_ += 2;
    }
}


////////////////////////////////////////////////////////////////////////////////
void Game::guess()
{
    //  one can only submit if all balls are set ...
    if (board_.getNumGuesses() < board_.getNumBalls()) {
        cout << "you need " << board_.getNumBalls() << " balls to lock in" << endl;
    }
    //  ... which would be here
    else
    {
        const auto& guesses = board_.getGuesses();
        
        //  count correct guesses
        int correct = 0;
        
        for (const auto& guess : guesses) {
            if (board_.isBall(guess)) {
                correct++;
            }
        }
        
        //  account wrong guesses
        if (correct < board_.getNumBalls()) {
            cout << "not quite. try again!" << endl;
            points_ += 5 * (board_.getNumBalls() - correct);
        }
        //  all correct
        else
        {
            cout << "================================================================================" << endl;
            cout << "yiha, nicely done, good sir!" << endl;
            cout << "your final score is " << points_ << " points." << endl;
            cout << "================================================================================" << endl;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void Game::reset()
{
    points_ = 0;
    board_.initBallsRandom(5);
    
    flags_["balls"] = false;
    flags_["path"] = false;
    flags_["guessing"] = false;
}


////////////////////////////////////////////////////////////////////////////////
void Game::toggleFullscreen()
{
    if (flags_["fullscreen"]) {
        window_->create(windowMode_, windowTitle_, sf::Style::Default, windowSettings_);
    } else {
        sf::VideoMode mode = sf::VideoMode::getFullscreenModes()[0];
        window_->create(mode, windowTitle_, sf::Style::Fullscreen, windowSettings_);
    }
    
    flags_["fullscreen"] = !flags_["fullscreen"];
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
            flags_["running"] = false;
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
            flags_["running"] = false;
            break;
            
        //  move player
        case sf::Keyboard::Right:
            board_.movePlayerRight();
            break;
            
        case sf::Keyboard::Left:
            board_.movePlayerLeft();
            break;
            
        //  spawn new balls
        case sf::Keyboard::Num1:
            board_.initBallsRandom(5);
            break;
            
        //  reset guesses
        case sf::Keyboard::R:
            board_.clearGuesses();
            break;
            
        //  shoot
        case sf::Keyboard::Space:
            shoot();
            break;
            
        //  fullscreen
        case sf::Keyboard::F:
            toggleFullscreen();
            break;
            
        //  balls
        case sf::Keyboard::B:
            flags_["balls"] = !flags_["balls"];
            break;
            
        //  guessing
        case sf::Keyboard::G:
            flags_["guessing"] = !flags_["guessing"];
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
    
    if (flags_["guessing"]) {
        handleGuessingKeyboardEvents(event);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Game::handleGuessingKeyboardEvents(const sf::Event &event)
{
    switch (event.key.code)
    {
        case sf::Keyboard::W:
            marker_.move();
            break;
            
        case sf::Keyboard::A:
            marker_.left();
            break;
            
        case sf::Keyboard::S:
            marker_.back();
            break;
            
        case sf::Keyboard::D:
            marker_.right();
            break;
        
        case sf::Keyboard::Down:
            board_.addGuess(marker_.position);
            break;
            
        case sf::Keyboard::Up:
            board_.removeGuess(marker_.position);
            break;
            
        case sf::Keyboard::Return:
            guess();
            break;
            
        default:
            break;
    }
}