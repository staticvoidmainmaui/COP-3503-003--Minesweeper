#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class LeaderboardWindow {
public:
    LeaderboardWindow();
    void run();
    void addNewRecord(const std::string& name, const std::string& time);
private:
    void handleEvents();
    void draw();
    void loadLeaderboard();
    void saveLeaderboard();


    sf::RenderWindow mWindow;
    sf::Font mFont;
    sf::Text mTitleText;
    sf::Text mEntriesText;
    sf::Text mNewEntryText;
    std::string mLeaderboardContents;
    std::vector<std::string> mLeaderboard;
    bool mIsNewRecord = false;

    // Button for returning to the main menu
    sf::RectangleShape mBackButton;
};



