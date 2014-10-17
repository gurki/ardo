#include "Game.h"

#include <sstream>


////////////////////////////////////////////////////////////////////////////////
Game::Game(
    sf::RenderWindow* window,
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
    flags_["shouldDismissMessageBox"] = false;
    
    //  initialise world
    board_.setPlayerPosition(3);
    soundRenderer_.game = this;
    showWelcomeMessage();
    
    clock_.restart();
}


////////////////////////////////////////////////////////////////////////////////
void Game::showWelcomeMessage()
{
    stringstream stream;
    stream << "Welcome to ARDO!" << endl;
    stream << "This is a remake of the 1970's Boardgame" << endl;
    stream << "'BlackBox' or later 'ORDO'. Your goal is" << endl;
    stream << "to guess the five balls located some-" << endl;
    stream << "where on the board. But unlike the ori-" << endl;
    stream << "ginal game, you send beams of sound into" << endl;
    stream << "the field and have to listen carefully," << endl;
    stream << "in order to make out the balls, which" << endl;
    stream << "guide the sound along the board." << endl;
    stream << endl;
    stream << "Good luck!" << endl;
    stream << endl;
    stream << "PRESS ANY KEY TO CONTINUE" << endl;
    
    hud_.addMessageBox(Hud::Center, stream.str(), HudMessage::Permanent);
    flags_["shouldDismissMessageBox"] = true;
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
    
    hud_.draw(*window_);
    
    window_->display();
}


////////////////////////////////////////////////////////////////////////////////
void Game::shoot()
{
    hud_.addMessage(Hud::Center, "pew", 0.2f);
    
    shots_++;
    
    //  get path
    const vector<vec2i> path = board_.getPath(false);
    
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
    if (board_.getNumGuesses() < board_.getNumBalls())
    {
        stringstream stream;
        stream << "YOU NEED " << board_.getNumBalls() << " GUESSED BALLS FOR SUBMISSION";
        
        hud_.addMessageBox(Hud::Center, stream.str(), HudMessage::Permanent);
        flags_["shouldDismissMessageBox"] = true;
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
            hud_.addMessageBox(Hud::Center, "TRY AGAIN", 2.0f);
            points_ += 5 * (board_.getNumBalls() - correct);
        }
        //  all correct
        else
        {
            stringstream stream;
            stream << "BOOYA! NICELY DONE!" << endl;
            stream << "YOUR FINAL SCORE IS " << points_ << "." << endl;
            stream << "IT TOOK YOU " << shots_ << " SHOTS" << endl;
            stream << "TO START A NEW GAME, PRESS 'BACKSPACE'";
            
            hud_.addMessageBox(Hud::Center, stream.str(), HudMessage::Permanent);
            flags_["shouldDismissMessageBox"] = true;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void Game::reset()
{
//    hud_.addMessageBox(Hud::Center, "STARTING NEW GAME", 2.0f);
    showWelcomeMessage();
    
    shots_ = 0;
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
void Game::showHelp()
{
    stringstream stream;
    stream << "#KEYBOARD" << endl;
    stream << "PLAYER" << endl;
    stream << "  Left/Right:  Move Player" << endl;
    stream << "  Space:       Shoot" << endl;
    stream << "  I:           Activate/Deactivate Voice Input" << endl;
    stream << "  O:           Switch Synthetic/Recorded Sound" << endl;
    stream << "GUESSING" << endl;
    stream << "  G:           Enter/Leave Guess" << endl;
    stream << "  WASD:        Move Marker" << endl;
    stream << "  Up/Down:     Place/Remove Guess" << endl;
    stream << "  Enter:       Submit Guess" << endl;
    stream << "  R:           Remove All Guesses" << endl;
    stream << "VARIOUS" << endl;
    stream << "  Backspace:   Start New Game" << endl;
    stream << "  F:           Switch Fullscreen" << endl;
    stream << "  V:           Activate FP/Static Camera" << endl;
    stream << "  B:           Show/Hide Balls" << endl;
    stream << "  P:           Show/Hide Paths";
    
    hud_.addMessageBox(Hud::Center, stream.str(), 0.5f);
}


////////////////////////////////////////////////////////////////////////////////
void Game::handleKeyboardEvents(const sf::Event& event)
{
    if (flags_["shouldDismissMessageBox"]) {
        flags_["shouldDismissMessageBox"] = false;
        hud_.dismissMessageBox(Hud::Center);
    }
    
    switch (event.key.code)
    {
        case sf::Keyboard::H:
            showHelp();
            break;
            
        case sf::Keyboard::BackSpace:
            reset();
            break;
            
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
            soundRenderer_.shoot();
            break;
            
        //  fullscreen
        case sf::Keyboard::F:
            toggleFullscreen();
            break;
            
        //  toggle input mode
        case sf::Keyboard::I:
            hud_.addMessage(Hud::Center, "toggleListening", 1.0f);
            soundRenderer_.toggleListening();
            break;
            
        //  toggle soudn type
        case sf::Keyboard::O:
            hud_.addMessage(Hud::Center, "toggleAmmunition", 1.0f);
            soundRenderer_.toggleAmmunition();
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