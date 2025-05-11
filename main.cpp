#include "Stronghold.h"
#include <iostream>
#include <limits>
using namespace std;

int main() {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Welcome message
    cout << "Welcome to Stronghold: Rule Your Medieval Kingdom!" << endl;

    // Initialize kingdom
    string kingdomName;
    cout << "Enter your kingdom's name: ";
    getline(cin, kingdomName);
    if (kingdomName.empty()) {
        kingdomName = "Default Kingdom";
    }

    string kingName;
    cout << "Enter your king's name: ";
    getline(cin, kingName);
    if (kingName.empty()) {
        kingName = "King Ali";
    }

    Kingdom kingdom(kingdomName);
    kingdom.setRuler(make_unique<King>(kingName, 70, 60, 50, 80));

    // Main game loop
    bool running = true;
    int choice;

    while (running && !kingdom.isGameOver()) {
        clearScreen();
        displayMainMenu();

        // Get user input with validation
        if (cin >> choice && choice >= 1 && choice <= 13) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            running = processMenuChoice(choice, kingdom);
            if (choice != 11) {
                clearScreen();
            }
            else if (choice != 12) {
                clearScreen();
            }
        }
        else {
            cout << "Invalid input! Please enter a number between 1 and 13." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pauseScreen();
            clearScreen();
        }

        // Check game over conditions
        if (kingdom.isGameOver()) {
            cout << "\n===== GAME OVER =====" << endl;
            cout << "Your kingdom has fallen!" << endl;
            cout << "Final Score: " << kingdom.getScore() << endl;
            cout << "Years Ruled: " << kingdom.getGameYear() - 1 << endl;
            running = false;
        }
    }

    // Final message
    if (!kingdom.isGameOver()) {
        cout << "\nThank you for playing Stronghold!" << endl;
    }

    return 0;
}