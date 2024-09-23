/*
 * gameengine.hh
 *
 * Purpose:
 *  Core module managing game logic, player turns, and outcomes.
 *  Handles rolling dice, determining winners, and updating points.
 *  Promotes separation of concerns by encapsulating core game mechanics.
 *
 *
 * Program author:
 *  Name: Natasha Dmello
 * */


#ifndef GAMEENGINE_HH
#define GAMEENGINE_HH

#include <string>
#include <vector>
#include <map>
#include "functions.hh"

// Obvious constants
const int INITIAL_NUMBER_OF_ROLLS = 3;
const int NUMBER_OF_DICES = 5;

// Data of each player
struct Player
{
    unsigned int id_;
    unsigned int rolls_left_;
    std::vector<int> latest_point_values_;
    std::vector<int> best_point_values_;
    std::string Name;
    unsigned int latestTotalPoints;
    int lockedPoints[5]={-1,-1,-1,-1,-1};
    std::map<int, int> RESULT_POINTS =
    { { NOTHING,         0},
      { PAIR,            0},
      { TWO_PAIRS,       0},
      { THREE_OF_A_KIND, 0},
      { STRAIGHT,        0},
      { FULL_HOUSE,      0},
      { FOUR_OF_A_KIND,  0},
      { YATZY,           0} };
};

class GameEngine
{
public:
    // Constructor
    GameEngine();

    // Destructor
    ~GameEngine();

    // Adds a new player
    void add_player(const Player player);

    // Prints guide text, telling which player is in turn and how many trials
    // they have left.
    void update_guide() const;

    // Rolls all dices, i.e. draws a new series of face numbers for the player
    // currently in turn. Moreover, reports the winner, if after the draw, all
    // players have used all their turns.
    std::vector<int> roll();

    // Gives turn for the next player having turns left, i.e. for the next
    // element in the players_ vector. After the last one, turn is given for
    // the second one (since the first one is NOBODY).
    int give_turn();

    // Reports a winner based on the current situation and sets the game_over_
    // attribute as true.
    std::string report_winner();

    // Tells if the game is over, i.e. if all players have used all their
    // turns.
    bool is_game_over() const;

    // Vector of all players
    std::vector<Player> players_;

    // Tells the player currently in turn (index of players_ vector)
    unsigned int game_turn_;

private:
    // Reports the status of the player currently in turn
    void report_player_status() const;

    // Updates best and latest points of the player in turn:
    // latest_point_values_ will always be new_points,
    // best_point_values_ will be new_points, if the last_mentioned is better.
    void update_points(const std::vector<int>& new_points);

    // Returns true if all turns of all players have been used,
    // otherwise returns false.
    bool all_turns_used() const;

    // Tells if the game is over
    bool game_over_;
};

#endif // GAMEENGINE_HH
