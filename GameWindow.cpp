#include "GameWindow.h"
#include "LeaderboardWindow.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <chrono>

GameWindow::GameWindow(const std::string& name)
   : mWindow(sf::VideoMode(800, 650), "Minesweeper", sf::Style::Close),
    mPlayerName(name)
{

   readConfig();
   loadTextures();
   setupUI();
}

void GameWindow::readConfig() {
   std::ifstream configFile("files/config.cfg");
   if (!configFile.is_open()) {
       std::cerr << "Error: Could not open config file!" << std::endl;
       return;
   }
   std::string line;
   std::getline(configFile, line);
   std::istringstream(line) >> mColumns;
   std::getline(configFile, line);
   std::istringstream(line) >> mRows;
   std::getline(configFile, line);
   std::istringstream(line) >> mTotalMines;
   configFile.close();
   if (mTotalMines > mColumns * mRows) {
       std::cerr << "Error: Number of mines exceeds the total number of tiles!" << std::endl;
       mTotalMines = mColumns * mRows;
   }
}
//
// void GameWindow::timer() {
//
// }
void GameWindow::drawTimer() {
    using clock = std::chrono::high_resolution_clock;

    int elapsed;    // seconds

    if (mTimerFrozen) {      // timer stopped
        elapsed = static_cast<int>(
            std::chrono::duration_cast<std::chrono::seconds>(mElapsedFinal).count());
    } else {// live counter
        auto now    = clock::now();
        auto paused = mPausedTotal +
                      (mGamePaused ? now - mPauseStart : clock::duration::zero());
        elapsed = static_cast<int>(
            std::chrono::duration_cast<std::chrono::seconds>(now - mStartTime - paused).count());
    }

    if (elapsed > 5999) elapsed = 5999;  // 99 min 59 sec max

    int minutes  = elapsed / 60;
    int seconds  = elapsed % 60;

    constexpr int w = 21;  // digit width
    float y = (mRows + 0.5f) * 32.f + 16.f;
    float xMin = mColumns * 32.f - 97.f;  // minutes start-X
    float xSec = mColumns * 32.f - 54.f;  // seconds start-X

    std::array<int,4> d{ minutes / 10, minutes % 10,
                         seconds / 10, seconds % 10 };

    for (int i = 0; i < 2; ++i) {// draw minutes
        sf::Sprite s(mDigitTexture);
        s.setTextureRect({ d[i] * w, 0, w, 32 });
        s.setPosition(xMin + i * w, y);
        mWindow.draw(s);
    }
    for (int i = 0; i < 2; ++i) {  // draw seconds
        sf::Sprite s(mDigitTexture);
        s.setTextureRect({ d[2 + i] * w, 0, w, 32 });
        s.setPosition(xSec + i * w, y);
        mWindow.draw(s);
    }
}



void GameWindow::drawCounter() {
    auto flagCount = static_cast<int>(std::count(mHasFlag.begin(), mHasFlag.end(), true));
    int value = mTotalMines - flagCount;

    bool negative = value < 0;
    if (negative) value = -value;
    if (value > 999) value = 999;

    int hundreds = value / 100;
    int tens     = (value / 10) % 10;
    int ones     = value % 10;

    int w  = 21;    // glyph width
    float y = (mRows + 0.5f) * 32.f + 16.f; // baseline
    float xDigits = 33.f;   // first digit x-pos

    std::array<int,3> d{hundreds, tens, ones};

    if (negative) {      // draw minus sign
        sf::Sprite minus;
        minus.setTexture(mDigitTexture);
        minus.setTextureRect(sf::IntRect(10 * w, 0, w, 32)); // minus glyph
        minus.setPosition(12.f, y);
        mWindow.draw(minus);
    }

    for (int i = 0; i < 3; ++i) {// draw three digits
        sf::Sprite s;
        s.setTexture(mDigitTexture);
        s.setTextureRect(sf::IntRect(d[i] * w, 0, w, 32));
        s.setPosition(xDigits + i * w, y);
        mWindow.draw(s);
    }
}



void GameWindow::loadTextures() {
   if (!mTileTexture.loadFromFile("files/images/tile_hidden.png") ||
       !mMineTexture.loadFromFile("files/images/mine.png") ||
       !mFlagTexture.loadFromFile("files/images/flag.png") ||
       !mTileRevealedTexture.loadFromFile("files/images/tile_revealed.png") ||
       !mFaceTexture.loadFromFile("files/images/face_happy.png") ||
       !mFaceSadTexture.loadFromFile("files/images/face_lose.png") ||
       !mFaceWinTexture.loadFromFile("files/images/face_win.png") ||
       !mPauseTexture.loadFromFile("files/images/pause.png") ||
       !mPlayTexture.loadFromFile("files/images/play.png") ||
       !mLeaderboardTexture.loadFromFile("files/images/leaderboard.png") ||
       !mDebugTexture.loadFromFile("files/images/debug.png") ||
       !mDigitTexture.loadFromFile("files/images/digits.png")) {
       std::cerr << "Error: Could not load textures!" << std::endl;
   }
   for (int n = 1; n <= 8; ++n)
   {
       if (!mNumberTextures[n-1].loadFromFile(
               "files/images/number_" + std::to_string(n) + ".png"))
       {
           std::cerr << "Error: Could not load number_" << n << ".png\n";
       }
   }
}

void GameWindow::setupUI() {
    mStartTime = std::chrono::high_resolution_clock::now();

    const float boardWidth = static_cast<float>(mColumns) * 32.f;
    const float yPos       = (mRows + 0.5f) * 32.f;

    mFaceButton.setTexture(mFaceTexture);
    mFaceButton.setPosition(boardWidth * 0.5f - 32.f, yPos);

    mPauseButton.setTexture(mPauseTexture);
    mPauseButton.setPosition(boardWidth - 240.f, yPos);

    mLeaderboardButton.setTexture(mLeaderboardTexture);
    mLeaderboardButton.setPosition(boardWidth - 176.f, yPos);

    mDebugButton.setTexture(mDebugTexture);
    mDebugButton.setPosition(boardWidth - 304.f, yPos);

    mFaceSprite.setTexture(mFaceTexture);
    mFaceSprite.setPosition(boardWidth * 0.5f - 32.f, yPos);

    mIsMine.assign(mColumns * mRows, false);
    mHasFlag.assign(mColumns * mRows, false);
    mIsRevealed.assign(mColumns * mRows, false);
    mAdjCount.assign(mColumns * mRows, 0);

    generateMines();
    buildAdjacency();

    mTiles.clear();
    for (int i = 0; i < mColumns * mRows; ++i) {
        sf::Sprite tileSprite;
        tileSprite.setTexture(mTileTexture);
        tileSprite.setPosition((i % mColumns) * 32, (i / mColumns) * 32);
        mTiles.push_back(tileSprite);
    }
}

void GameWindow::toggleDebugMode() {
   if (mGameOver || mGameWon) {
       return;
   }
   mDebugMode = !mDebugMode;
}

void GameWindow::toggleFlag(int index) {
   if (mHasFlag[index]) {
       mHasFlag[index] = false;
       mTiles[index].setTexture(mTileTexture);
   } else {
       mHasFlag[index] = true;
       mTiles[index].setTexture(mFlagTexture);
   }
}

void GameWindow::generateMines() {
   int mineCount = 0;
   while (mineCount < mTotalMines) {
       int index = rand() % (mColumns * mRows);
       if (!mIsMine[index]) {
           mIsMine[index] = true;
           mineCount++;
       }
   }
}

void GameWindow::buildAdjacency() {
    auto inBounds = [&](int r, int c) { return 0 <= r && r < mRows && 0 <= c && c < mColumns; };
    for (int r = 0; r < mRows; ++r)
        for (int c = 0; c < mColumns; ++c) {
            int idx = r * mColumns + c;
            if (mIsMine[idx]) continue;
            int count = 0;
            for (int dr = -1; dr <= 1; ++dr)
                for (int dc = -1; dc <= 1; ++dc)
                    if ((dr || dc) && inBounds(r + dr, c + dc) && mIsMine[(r + dr) * mColumns + (c + dc)])
                        ++count;
            mAdjCount[idx] = count;
        }
}

void GameWindow::revealTile(int index) {
    if (index < 0 || index >= mColumns * mRows) return;
    if (mIsRevealed[index] || mHasFlag[index] || mGameOver) return;

    if (mIsMine[index]) {
        mTiles[index].setTexture(mMineTexture);
        mGameOver = true;
        endGame(false);
        return;
    }

    if (mAdjCount[index] == 0) {
        revealRecursive(index);
    } else {
        mIsRevealed[index] = true;
        ++mRevealedSafe;
        mTiles[index].setTexture(mNumberTextures[mAdjCount[index] - 1]);
    }

    int totalSafe = (mColumns * mRows) - mTotalMines;
    if (mRevealedSafe == totalSafe) endGame(true);
}

void GameWindow::revealRecursive(int index) {
    if (index < 0 || index >= mColumns * mRows) return;
    if (mIsRevealed[index] || mHasFlag[index]) return;

    mIsRevealed[index] = true;
    ++mRevealedSafe;

    if (mAdjCount[index] == 0) {
        mTiles[index].setTexture(mTileRevealedTexture);
        int r = index / mColumns;
        int c = index % mColumns;
        for (int dr = -1; dr <= 1; ++dr)
            for (int dc = -1; dc <= 1; ++dc)
                if (dr || dc) {
                    int rr = r + dr, cc = c + dc;
                    if (0 <= rr && rr < mRows && 0 <= cc && cc < mColumns)
                        revealRecursive(rr * mColumns + cc);
                }
    } else {
        mTiles[index].setTexture(mNumberTextures[mAdjCount[index] - 1]);
    }
}
void GameWindow::endGame(bool win) {
    if (mGameOverHandled) return;
    mGameOverHandled = true;
    mGameOver        = true;
    mGameWon         = win;

    /* ───────────── freeze timer elapsed ───────────── */
    using clock = std::chrono::high_resolution_clock;
    auto now = clock::now();
    mElapsedFinal = now - mStartTime - mPausedTotal;   // excludes paused time
    mTimerFrozen  = true;  // drawTimer() will stop

    //─────────────  flgsas ─────────────
    if (win) {
        for (size_t i = 0; i < mIsMine.size(); ++i)
            if (mIsMine[i]) {
                mHasFlag[i]   = true;
                mIsRevealed[i] = false;
            }
        mFaceSprite.setTexture(mFaceWinTexture);
    } else {
        for (size_t i = 0; i < mIsMine.size(); ++i)
            if (mIsMine[i])  mIsRevealed[i] = true;
        mFaceSprite.setTexture(mFaceSadTexture);
    }

    mPauseButton.setTexture(mPlayTexture);
    mPauseButton.setColor(sf::Color(128, 128, 128));

    /* ───────────── format MM:SS  ───────────── */
    int seconds = static_cast<int>(
        std::chrono::duration_cast<std::chrono::seconds>(mElapsedFinal).count());
    int mm = seconds / 60, ss = seconds % 60;
    char buffer[6];
    std::snprintf(buffer, sizeof(buffer), "%02d:%02d", mm, ss);
    std::string timeStr(buffer);

    if (win) {                                          // only record victories
        LeaderboardWindow lb;// ensure current list
        lb.addNewRecord(mPlayerName, timeStr);
    }
}


void GameWindow::draw() {
    mWindow.clear(sf::Color::White);

    for (std::size_t i = 0; i < mTiles.size(); ++i) {
        auto &t = mTiles[i];

        // pause overlay -------------------------------------
        if (mGamePaused) {
            t.setTexture(mTileRevealedTexture);
            mWindow.draw(t);
            continue;
        }

        //victory overlay: every remaining tile
        if (mGameOver && mGameWon && mIsMine[i]) {
            t.setTexture(mTileTexture);           // keep hidden background
            mWindow.draw(t);

            sf::Sprite flag;
            flag.setTexture(mFlagTexture);        // overlay flag sprite
            flag.setPosition(t.getPosition());
            mWindow.draw(flag);

            continue;
        }

        //
        if (mIsRevealed[i]) {
            if (mIsMine[i]) {
                t.setTexture(mMineTexture);
                mWindow.draw(t);
            } else {
                t.setTexture(mTileRevealedTexture);
                mWindow.draw(t);

                int cnt = mAdjCount[i];
                if (cnt > 0) {
                    sf::Sprite num;
                    num.setTexture(mNumberTextures[cnt - 1]);
                    num.setPosition(t.getPosition());
                    mWindow.draw(num);
                }
            }
        } else if (mHasFlag[i]) {
            t.setTexture(mTileTexture);  // hidden background first
            mWindow.draw(t);

            sf::Sprite flag;
            flag.setTexture(mFlagTexture);        // flag icon on top
            flag.setPosition(t.getPosition());
            mWindow.draw(flag);
        } else {
            t.setTexture((mDebugMode && mIsMine[i]) ? mMineTexture : mTileTexture);
            mWindow.draw(t);
        }
    }


    if (mGameWon && mFaceSprite.getTexture() != &mFaceWinTexture)
        mFaceSprite.setTexture(mFaceWinTexture);
    else if (mGameOver && !mGameWon &&
             mFaceSprite.getTexture() != &mFaceSadTexture)
        mFaceSprite.setTexture(mFaceSadTexture);

    mWindow.draw(mFaceButton);
    mWindow.draw(mPauseButton);
    mWindow.draw(mLeaderboardButton);
    mWindow.draw(mDebugButton);
    mWindow.draw(mFaceSprite);

    drawCounter();
    drawTimer();

    mWindow.display();
}


void GameWindow::run() {
    while (mWindow.isOpen()) {
        handleEvents();
        if (mGameOver && !mGameOverHandled)
            endGame(false);
        draw();
    }
}

void GameWindow::handleEvents() {
    sf::Event ev;
    while (mWindow.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed) { mWindow.close(); }

        if (ev.type != sf::Event::MouseButtonPressed) continue;

        int mx = ev.mouseButton.x, my = ev.mouseButton.y;

       //buttons optimized asf
        if (mPauseButton.getGlobalBounds().contains(mx, my)) { togglePause();  continue; }
        if (mFaceButton .getGlobalBounds().contains(mx, my)) { restartGame();  continue; }
        if (mLeaderboardButton.getGlobalBounds().contains(mx, my)) { showLeaderboard(); continue; }


        if (mGamePaused || mGameOver) continue;

        if (mDebugButton.getGlobalBounds().contains(mx, my)) { toggleDebugMode(); continue; }

        if (ev.mouseButton.button == sf::Mouse::Left) {
            for (size_t i = 0; i < mTiles.size(); ++i)
                if (mTiles[i].getGlobalBounds().contains(mx, my)) { revealTile(static_cast<int>(i)); break; }
        } else if (ev.mouseButton.button == sf::Mouse::Right) {
            for (size_t i = 0; i < mTiles.size(); ++i)
                if (mTiles[i].getGlobalBounds().contains(mx, my)) { toggleFlag(static_cast<int>(i)); break; }
        }
    }
}

void GameWindow::restartGame() {
    using clock = std::chrono::high_resolution_clock;

    mGameOver   = false;
    mGameWon    = false;
    mGamePaused = false;
    mGameRunning = true;

    mFaceSprite.setTexture(mFaceTexture);

    /* reset timer bookkeeping */
    mPausedTotal   = clock::duration::zero();
    mElapsedFinal  = clock::duration::zero();
    mTimerFrozen   = false;
    mStartTime     = clock::now();        // timer restarts from 0
    mPauseStart    = mStartTime;          // keeps pause math consistent

    setupUI();                            // rebuild board, flags, etc.
}


void GameWindow::togglePause() {
    using clock = std::chrono::high_resolution_clock;

    if (!mGamePaused) {  // going *into* pause
        mGamePaused  = true;
        mPauseStart  = clock::now();
        mPauseButton.setTexture(mPlayTexture);
    } else {         // leaving pause
        mPausedTotal += clock::now() - mPauseStart;
        mGamePaused   = false;
        mPauseButton.setTexture(mPauseTexture);
    }
}


void GameWindow::showLeaderboard() {
    using clock = std::chrono::high_resolution_clock;

    //
    bool   wasPaused     = mGamePaused;
    auto   oldPauseStart = mPauseStart;

    //
    if (!mGamePaused) {
        mGamePaused = true;
        mPauseStart = clock::now();
    }

    draw();                          // one immediate repaint (grey board)

    //
    LeaderboardWindow lb;            // loads and displays top-5 list
    lb.run();                        // blocks until user closes window

    //
    if (!wasPaused) {
        mPausedTotal += clock::now() - mPauseStart;
        mGamePaused   = false;       // we weren’t paused before, resume
    } else {
        mPauseStart = oldPauseStart; // still paused from earlier
    }
}


