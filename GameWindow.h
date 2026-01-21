#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <chrono>

class GameWindow {
public:
    // GameWindow.h
    explicit GameWindow(const std::string& name = "Player");

    void run();

private:
    void readConfig();
    void loadTextures();
    void setupUI();
    void buildAdjacency();
    void drawTimer();
    void drawCounter();

    void handleEvents();
    void draw();
    void restartGame();
    void togglePause();
    void showLeaderboard();
    void toggleDebugMode();
    void updateTimer();
    void resetBoard();

    void generateMines();
    void toggleFlag(int index);
    void revealTile(int index);
    void revealRecursive(int index);
    void endGame(bool win);

    sf::RenderWindow mWindow;
    sf::Font mFont;


    sf::Sprite mFaceButton, mPauseButton, mLeaderboardButton, mDebugButton;
    sf::Sprite mFaceSprite;

    sf::Texture mTileTexture, mMineTexture, mFlagTexture, mTileRevealedTexture;
    sf::Texture mFaceTexture, mFaceSadTexture, mFaceWinTexture;
    sf::Texture mPauseTexture, mPlayTexture, mLeaderboardTexture, mDebugTexture;
    sf::Texture mDigitTexture;
    std::array<sf::Texture, 8> mNumberTextures;

    std::chrono::high_resolution_clock::time_point mPauseStart;
    std::chrono::high_resolution_clock::duration   mPausedTotal{0};
    std::chrono::high_resolution_clock::duration mElapsedFinal{0};  // time when game ends


    std::vector<sf::Sprite> mTiles;

    int mColumns = 25;
    int mRows = 16;
    int mTotalMines = 50;

    std::vector<bool> mIsMine;
    std::vector<bool> mHasFlag;
    std::vector<bool> mIsRevealed;
    std::vector<int> mAdjCount;

    bool  mTimerFrozen  = false;
    bool mDebugMode = false;
    bool mGamePaused = false;
    bool mGameRunning = true;
    bool mGameWon = false;
    bool mGameOver = false;
    bool mGameOverHandled = false;
    int mRevealedSafe = 0;
    std::string mPlayerName;

    std::chrono::high_resolution_clock::time_point mStartTime;
};

#endif // GAMEWINDOW_H
