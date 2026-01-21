#include "WelcomeWindow.h"
#include "GameWindow.h"  //
#include <iostream>
#include <SFML/Window.hpp>

WelcomeWindow::WelcomeWindow()
    : mWindow(sf::VideoMode(800, 600), "Minesweeper - Welcome", sf::Style::Close)
{
    if (!mFont.loadFromFile("files/font.ttf")) {
        std::cerr << "Error: Could not load font.\n";
    }
    mTitleText.setFont(mFont);
    mTitleText.setString("WELCOME TO MINESWEEPER!");
    mTitleText.setCharacterSize(24);
    mTitleText.setFillColor(sf::Color::White);
    mTitleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(mTitleText, 400, 150);          // width/2 , height/2 −150

    /* ── prompt ─────────────── */
    mEnterNameText.setFont(mFont);
    mEnterNameText.setString("Enter your name:");
    mEnterNameText.setCharacterSize(20);
    mEnterNameText.setFillColor(sf::Color::White);
    mEnterNameText.setStyle(sf::Text::Bold);
    setText(mEnterNameText, 400, 225);      // height/2 −75

    /* ── player name field ──── */
    mNameText.setFont(mFont);
    mNameText.setCharacterSize(18);
    mNameText.setFillColor(sf::Color::Yellow);
    mNameText.setStyle(sf::Text::Bold);
    setText(mNameText, 400, 255);           // height/2 −45

    mCursor.setFont(mFont);
    mCursor.setString("|");
    mCursor.setCharacterSize(18);
    mCursor.setFillColor(sf::Color::Yellow);
    mCursor.setStyle(sf::Text::Bold);

}

void WelcomeWindow::setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void WelcomeWindow::run() {
    while (mWindow.isOpen()) {
        handleEvents();
        update();
        draw();
    }
}

void WelcomeWindow::handleEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }
        else if (event.type == sf::Event::TextEntered) {
            // Handle text input (name entry)
            if (event.text.unicode == 8 && mPlayerName.length() > 0) {
                mPlayerName.pop_back(); // Backspace
            } else if (event.text.unicode >= 65 && event.text.unicode <= 90 || event.text.unicode >= 97 && event.text.unicode <= 122) {
                if (mPlayerName.length() < 10) {
                    mPlayerName += static_cast<char>(event.text.unicode);
                }
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // Start Game Button click
                if (mStartButtonText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (mPlayerName.length() > 0) {
                        formatPlayerName();
                        mIsValidName = true;

                        // Transition to the Game Window
                        mWindow.close(); // Close the Welcome Window
                        GameWindow gameWindow(mPlayerName); // Create a Game Window instance
                        gameWindow.run(); // Run the Game Window (start the game)
                    } else {
                        mErrorText.setString("Name cannot be empty!");
                    }
                }
            }
        }

        else if (event.type == sf::Event::KeyPressed &&
         event.key.code == sf::Keyboard::Enter)                    // d,e,g
        {
            if (!mPlayerName.empty()) {
                formatPlayerName();                                        // final case
                mWindow.close();
                GameWindow gameWindow(mPlayerName);
                gameWindow.run();
            } else {
                mErrorText.setString("Name cannot be empty!");
            }
        }

        else if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8 && !mPlayerName.empty()) {          // backspace
                mPlayerName.pop_back();
            } else if (std::isalpha(event.text.unicode) && mPlayerName.size() < 10) {
                mPlayerName += static_cast<char>(event.text.unicode);
            }
            formatPlayerName();                                            //  ← i
        }




    }
}

void WelcomeWindow::update() {
    // Updat
    mNameText.setString(mPlayerName);
    setText(mNameText, 400, 250);

    mNameText.setString(mPlayerName);
    setText(mNameText, 400, 255);

    // cursor
    auto b = mNameText.getGlobalBounds();
    mCursor.setPosition(b.left + b.width + 2.f, b.top);

}
void WelcomeWindow::draw() {
    mWindow.clear(sf::Color::Blue);

    mWindow.draw(mTitleText);
    mWindow.draw(mEnterNameText);
    mWindow.draw(mNameText);


    mWindow.display();

    mWindow.draw(mCursor);

}


void WelcomeWindow::formatPlayerName() {
    if (!mPlayerName.empty()) {
        mPlayerName[0] = std::toupper(mPlayerName[0]);
        for (size_t i = 1; i < mPlayerName.length(); i++) {
            mPlayerName[i] = std::tolower(mPlayerName[i]);
        }
    }
}
