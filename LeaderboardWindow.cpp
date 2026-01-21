#include "LeaderboardWindow.h"
#include "WelcomeWindow.h"

#include <iostream>
#include <fstream>
#include <sstream>

LeaderboardWindow::LeaderboardWindow()
    : mWindow(sf::VideoMode(400, 400), "Leaderboard - Minesweeper", sf::Style::Close)
{
    if (!mFont.loadFromFile("files/font.ttf")) {
        std::cerr << "Error: Could not load font.\n";
    }

    mTitleText.setFont(mFont);
    mTitleText.setString("LEADERBOARD");
    mTitleText.setCharacterSize(24);
    mTitleText.setFillColor(sf::Color::White);
    mTitleText.setPosition(150, 20);

    loadLeaderboard();

    mEntriesText.setFont(mFont);
    mEntriesText.setCharacterSize(18);
    mEntriesText.setFillColor(sf::Color::White);
    mEntriesText.setPosition(50, 80);

    mNewEntryText.setFont(mFont);
    mNewEntryText.setCharacterSize(18);
    mNewEntryText.setFillColor(sf::Color::Green);
    mNewEntryText.setPosition(50, 300);

    mBackButton.setSize(sf::Vector2f(100, 50));
    mBackButton.setFillColor(sf::Color::Blue);
    mBackButton.setPosition(150, 350);
}

static int timeToSeconds(const std::string& mmss) {
    size_t p = mmss.find(':');
    return (p == std::string::npos)
           ? 999999
           : std::stoi(mmss.substr(0, p)) * 60 + std::stoi(mmss.substr(p + 1));
}

void LeaderboardWindow::run() {
    while (mWindow.isOpen()) {
        handleEvents();
        draw();
    }
}

void LeaderboardWindow::handleEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (mBackButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                mWindow.close();
                WelcomeWindow welcomeWindow;
                welcomeWindow.run();
            }
        }
    }
}

void LeaderboardWindow::draw() {
    mWindow.clear(sf::Color::Blue);
    mWindow.draw(mTitleText);
    mWindow.draw(mEntriesText);
    if (mIsNewRecord) {
        mWindow.draw(mNewEntryText);
    }
    mWindow.draw(mBackButton);
    mWindow.display();
}

void LeaderboardWindow::loadLeaderboard() {
    std::ifstream file("files/leaderboard.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open leaderboard.txt\n";
        return;
    }

    mLeaderboard.clear();
    std::string line;
    while (std::getline(file, line)) {
        mLeaderboard.push_back(line);
    }

    std::ostringstream oss;
    for (size_t i = 0; i < mLeaderboard.size(); i++) {
        oss << (i + 1) << ". " << mLeaderboard[i] << "\n";
    }
    mLeaderboardContents = oss.str();
    mEntriesText.setString(mLeaderboardContents);
}

void LeaderboardWindow::saveLeaderboard() {
    std::ofstream file("files/leaderboard.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open leaderboard.txt for saving.\n";
        return;
    }

    for (auto entry : mLeaderboard) {
        if (!entry.empty() && entry.back() == '*')
            entry.pop_back();
        file << entry << '\n';
    }
}

void LeaderboardWindow::addNewRecord(const std::string& name,
                                     const std::string& time)
{
    int bestBefore = INT_MAX;
    for (const auto& e : mLeaderboard)
        bestBefore = std::min(bestBefore,
                              timeToSeconds(e.substr(0, e.find(','))));

    int newSec = timeToSeconds(time);
    std::string entry = time + ", " + name;

    mLeaderboard.push_back(entry);

    std::sort(mLeaderboard.begin(), mLeaderboard.end(),
              [](const std::string& a, const std::string& b){
                  return timeToSeconds(a.substr(0, a.find(','))) <
                         timeToSeconds(b.substr(0, b.find(',')));
              });

    if (mLeaderboard.size() > 5) mLeaderboard.resize(5);

    for (auto& e : mLeaderboard)
        if (!e.empty() && e.back() == '*') e.pop_back();

    if (newSec < bestBefore && !mLeaderboard.empty()) {
        mLeaderboard.front() += " *";
        mIsNewRecord = true;
        mNewEntryText.setString("New record! " + mLeaderboard.front());
    } else {
        mIsNewRecord = false;
    }

    std::ostringstream oss;
    for (size_t i = 0; i < mLeaderboard.size(); ++i)
        oss << i + 1 << ". " << mLeaderboard[i] << '\n';
    mLeaderboardContents = oss.str();
    mEntriesText.setString(mLeaderboardContents);

    saveLeaderboard();
}
