/*
 * Yatzy GUI Project
 *
 * Desc:
 *   This project implements a graphical user interface for the Yatzy game. Yatzy is a dice game
 *   where players roll five dice in desired order, lock some of them, and aim for specific combinations
 *   to earn points. The GUI provides a visual representation of the game, allowing players to interact
 *   with dice rolls, locking mechanisms, and game status.
 *
 * Implemented Features:
 *   - Displaying the faces of the rolled dice using graphical components (e.g., labels or buttons).
 *   - Allowing players to roll again or give a turn to the next player.
 *   - Enabling the locking of dice for the next roll.
 *   - Showing the current game status, including which player is in turn and the number of rolls used.
 *   - Providing the option to start the game again (reset) after it is over or in the middle of the game.
 *   - Displaying a timer indicating the time used during the game.
 *   - Reporting the winner or a tie between players after all turns.
 *
 * Implemented Extra Features:
 *   - Usage of dice figures from PNG files for a visually appealing representation of dice.
 *   - Allowing users to set the number of players and give names to players.
 *   - Incorporating a score board.
 *   - Dynamic background color changes based on the game status.
 *   - Addition of a pause button to halt the game and inform users about the pause.
 *
 * Program author
 * Name: Natasha Dmello
 * */


#include "mainwindow.hh"
#include <QInputDialog>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool ok;
    int numberOfPlayers = QInputDialog::getInt(nullptr, "Number of Players", "Enter number of players:", 2, 2, 10, 1, &ok);

    if (!ok) return 0;

    MainWindow w;
    w.setNumberOfPlayers(numberOfPlayers);
    w.show();
    return a.exec();
}
