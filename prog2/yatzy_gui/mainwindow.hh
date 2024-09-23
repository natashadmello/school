/*
 * mainwindow.hh
 *
 * Purpose:
 *  Implements the GUI for a Yatzy game.
 *  Handles player interactions, updates game status, and provides features
 *  such as editing player names, locking/unlocking dice, and managing game flow.
 *  Integrates with the underlying game engine for gameplay logic.
 *
 * Program author:
 *  Name: Natasha Dmello
 *  Student number: 151802762
 *  UserID: nrnadm
 *  E-Mail: natasha.dmello@tuni.fi
 *
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QLabel>
#include "gameengine.hh"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Sets the number of players, updates UI, and initializes players for a new game.
    void setNumberOfPlayers(unsigned int players);



private slots:

    // Update the GUI elements with information about the current player, remaining rolls,
    // total points, result type, individual point categories, locked points on dice, and
    // the visual representation of rolled dice.
    void updateGameStatus();

    // Enable player name editing: prompts for a new name, updates in the game engine,
    // and refreshes the displayed game status in the GUI.
    void enableNameEdit();

    // Handles the button click event for rolling the dice.
    // Calls the game engine to roll the dice and updates the game status based on the result.
    void on_Roll_clicked();

    // Handles the button click event for ending the current player's turn.
    // Calls the game engine to pass the turn to the next player and updates the game status.
    void on_Turn_clicked();

    // Handles the button click event for quitting the game.
    // Disables UI elements, pauses the game, and displays winner information.
    void on_Quit_clicked();

    // Updates the displayed time on the UI.
    // If the game is not paused, increments the elapsed time and formats it as HH:MM:SS.
    void updateTime();

    // Toggles the pause state of the game. When the game is paused, it disables certain UI elements
    // to prevent user interaction. When resumed, it enables those UI elements back.
    void togglePause();

    // Closes the current MainWindow and restarts the application.
    // Allows users to reset the game state at any point during the game or after it has ended.
    void on_Reset_clicked();

    // This group of functions enables the player to lock or unlock dice after rolling.
    // It checks if the player has rolled before attempting to lock or unlock a category
    // and updates the UI accordingly.
    void on_Lock_1_clicked();
    void on_Lock_2_clicked();
    void on_Lock_3_clicked();
    void on_Lock_4_clicked();
    void on_Lock_5_clicked();

    // Closes the main window. This button allows the user to close the game window at any point during the game.
    void on_Close_clicked();

private:
    Ui::MainWindow *ui;
    void set_players();
    void disableUIElements();
    int elapsedTimeInSeconds = 0;
    QTimer *timer;
    bool paused = false;
    std::vector<QLabel*> Dices;

    GameEngine eng;
};
#endif // MAINWINDOW_HH
