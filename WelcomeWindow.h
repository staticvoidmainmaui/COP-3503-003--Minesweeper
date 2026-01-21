#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>

class WelcomeWindow {
public:
    WelcomeWindow();
    void run();

private:
    void handleEvents();
    void update();
    void draw();
    void formatPlayerName();

    sf::RenderWindow mWindow;
    sf::Font mFont;
    void setText(sf::Text &text, float x, float y);  // Declare settext


    // UI elements (texts, buttons)
    sf::Text mTitleText;
    sf::Text mEnterNameText;
    sf::Text mNameText;
    sf::Text mStartButtonText;
    sf::Text mErrorText;         // Error?
    sf::RectangleShape mStartButton;
    sf::Text mCursor;

    std::string mPlayerName;
    bool mIsValidName = false;
};
