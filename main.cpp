#include <SFML/Graphics.hpp>
#include "WelcomeWindow.h"  // Include the WelcomeWindow header
#include "GameWindow.h"     // Include the GameWindow header

int main() {
    // Create and run the WelcomeWindow
    WelcomeWindow welcomeWindow;
    welcomeWindow.run(); // This will transition to the GameWindow after start game is pressed

    // Create and run the GameWindow
    GameWindow gameWindow;
    gameWindow.run();    // Game logic is in the GameWindow
    return 0;
}
