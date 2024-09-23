// gameengine.cpp

// Program author:
// Name: Natasha Dmello
// Student number: 151802762
// UserID: nrnadm
// E-Mail: natasha.dmello@tuni.fi

#include "gameengine.hh"
#include "functions.hh"
#include <iostream>
#include <sstream>

GameEngine::GameEngine():
    game_turn_(0), game_over_(false)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::add_player(const Player player)
{
    players_.push_back(player);
}

void GameEngine::update_guide() const
{
    if(players_.size() <= game_turn_)
    {
        std::cout << "Internal error: update_guide" << std::endl;
        return;
    }
    ostringstream outputstream{""};
    outputstream << "Player " << game_turn_ + 1 << " in turn, "
                 << players_.at(game_turn_).rolls_left_ << " trials left!";
    std::cout << outputstream.str() << std::endl;
}

vector<int> GameEngine::roll()
{
    if(players_.size() <= game_turn_)
    {
        std::cout << "Internal error: roll" << std::endl;
        vector<int> errors;
        errors.push_back(503);
        return errors;
    }

    if(players_.at(game_turn_).rolls_left_ == 0)
    {
        std::cout << "No more rolls left" << std::endl;
        vector<int> errors;
        errors.push_back(404);
        return errors;
    }

    ostringstream outputstream{""};
    vector<int> new_points;
    unsigned int dice = 0;
    while ( dice < NUMBER_OF_DICES )
    {
        int point_value = roll_dice();
        std::cout << point_value << " ";
        if (players_.at(game_turn_).lockedPoints[dice]>0)
            new_points.push_back(players_.at(game_turn_).lockedPoints[dice]);
       else
           new_points.push_back(point_value);
        ++dice;
    }

    update_points(new_points);
    report_player_status();

    // Decreasing rolls left
    --players_.at(game_turn_).rolls_left_;

    // Checking if the player in turn has rolls left
    if ( players_.at(game_turn_).rolls_left_ == 0 )
    {
        outputstream << "Turn of " << players_.at(game_turn_).id_
                     << " has ended!";
        std::cout << outputstream.str() << std::endl;
    }

    // Checking if any player has turns left
    if ( all_turns_used() )
    {
        report_winner();
        vector<int> errors;
        errors.push_back(200);
        return errors;
    }
    return new_points;
}

int GameEngine::give_turn()
{
    // Searching for the next player among those, whose id_ is greater than
    // that of the current player
    for ( unsigned int i = game_turn_ + 1; i < players_.size(); ++i )
    {
        if ( players_.at(i).rolls_left_ > 0 )
        {
            game_turn_ = i;
            return 0;
        }
    }

    // A suitable next player couldn't be found in the previous search, so
    // searching for the next player among those, whose id_ is less than
    // or equal to that of the current player
    // (perhaps the current player is the only one having turns left)
    for(unsigned int i = 0; i <= game_turn_; ++i)
    {
        if(players_.at(i).rolls_left_ > 0)
        {
            game_turn_ = i;
            return 0;
        }
    }

    // No player has turns left
    report_winner();
    return 1;
}

string GameEngine::report_winner()
{
    vector<vector<int>> all_point_values;
    for ( auto player : players_ )
    {
        all_point_values.push_back(player.best_point_values_);
    }
    string winner_text = decide_winner(all_point_values);
    std::cout << winner_text << std::endl;

    game_over_ = true;
    return winner_text;
}


bool GameEngine::is_game_over() const
{
    return game_over_;
}

void GameEngine::report_player_status() const
{
    if ( players_.size() <= game_turn_ )
    {
        std::cout << "Internal error: report_player_status" << std::endl;
        return;
    }
    string textual_description = "";
    construe_result(players_.at(game_turn_).latest_point_values_,
                    textual_description);
    std::cout << textual_description << std::endl;
}

void GameEngine::update_points(const std::vector<int>& new_points)
{
    if ( players_.size() <= game_turn_ )
    {
        std::cout << "Internal error: update_points" << std::endl;
        return;
    }
    string dummy = "";
    int new_result = construe_result(new_points, dummy);
    players_.at(game_turn_).RESULT_POINTS[new_result]++;
    players_.at(game_turn_).latestTotalPoints=new_result;
    int best_result_so_far
            = construe_result(players_.at(game_turn_).best_point_values_,
                              dummy);

    if ( new_result > best_result_so_far )
    {
        players_.at(game_turn_).best_point_values_ = new_points;
    }
    players_.at(game_turn_).latest_point_values_ = new_points;
}

bool GameEngine::all_turns_used() const
{
    for ( auto player : players_ )
    {
        if ( player.rolls_left_ > 0 )
        {
            return false;
        }
    }
    return true;
}
