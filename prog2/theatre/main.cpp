/* Theatre
 *
 * Desc:
 * This program implements a database interface that stores the information regarding theatres and
 * allows the user to carry out some enquiry about the plays, players and available seats in different towns.
 * The data is stored in a complex data structure based on nested maps and vectors.
 *
 *  The program first prompts the user to specify an input file containing theatre data.
 *  It ensures input file validity and requests the user to input commands to perform
 *  various queries on the database.
 *
 * The program offers a comprehensive set of commands for users to explore theater-related information:
 *  - 'theatres': Retrieve a list of known theaters in alphabetical order.
 *  - 'plays': Obtain a unique list of plays, including aliases.
 *  - 'theatres_of_play <play>': Discover theaters offering a specific play.
 *  - 'plays_in_theatre <theatre>': View plays available in a given theater.
 *  - 'plays_in_town <town>': Find plays in a town with available seats, including aliases.
 *  - 'players_in_play <play> [<theatre>]': List actors in a play, optionally within a specific theater.
 *
 * The user can terminate the program using the 'quit' command, and the program
 * issues appropriate error messages for invalid inputs and unknown data entities.
 *
 * Program authors - Group Project
 * Name: Sithumini Premarathne
 * Name: Natasha Dmello
 *
 * Note -  the split function in the program is not the same as the one provided in the templates.
 * We have customized the split function to suit our specific needs.
 *
*/


#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;

// Fields in the input file
const int NUMBER_OF_FIELDS = 5;

// Data structure to store theater-related information. Uses a nested map and vector structure
// to organize data by town, theater, play, players, and the number of free seats. This design
// offers flexibility and efficient data retrieval for theater-related queries.
map<string, vector<pair<string, vector<pair<string, vector<pair<string, int>>>>>>> theatreData;


// Command prompt
const string PROMPT = "the> ";

// Error and other messages
const string EMPTY_FIELD = "Error: empty field in line ";
const string FILE_ERROR = "Error: input file cannot be opened";
const string WRONG_PARAMETERS = "Error: wrong number of parameters";
const string THEATRE_NOT_FOUND = "Error: unknown theatre";
const string PLAY_NOT_FOUND = "Error: unknown play";
const string PLAYER_NOT_FOUND = "Error: unknown player";
const string TOWN_NOT_FOUND = "Error: unknown town";
const string COMMAND_NOT_FOUND = "Error: unknown command";
const string NOT_AVAILABLE = "No plays available";

// Print a unique list of theaters in alphabetical order.
// This function iterates through the theater data and prints the names of theaters.
// It ensures that each theater is printed only once, even if it appears in multiple towns.
void printTheatres()
{
    set<string> printedTheatres;

    // Iterate through each town entry in the 'theatreData' data structure.
    for (const auto &townEntry : theatreData)
    {
        // For each town entry, iterate through the theaters within that town.
        // 'theatreEntry' represents an entry in the inner map.
        // '.second' is the value part of the key-value pair in the outer map,
        // containing information about various theaters within the current town.
        // '.first' is the theater name, the key within this inner map.
        for (const auto &theatreEntry : townEntry.second)
        {
            const string &theatreName = theatreEntry.first;

            // Check if the theater (theatreName) has already been printed.
            // If it hasn't been printed before, insert it into the set of printed theaters (printedTheatres).
            if (printedTheatres.find(theatreName) == printedTheatres.end())
            {
                printedTheatres.insert(theatreName);
            }
        }
    }

    // Print the theatres in alphabetical order
    for (const auto &theatre : printedTheatres)
    {
        cout << theatre << endl;
    }
}


// This function retrieves a list of unique plays available across all theaters, including any aliases,
// and prints them in alphabetical order.
void printPlays()
{
    set<string> uniquePlays;

    // Collects unique play names from the theater data structure by iterating through towns,
    // theaters, and plays, and stores them in the 'uniquePlays' set.
    // 'theatreEntry.second' represents the play name within the current theater, and this loop
    // iterates through various plays available in that theater.
    for (const auto &townEntry : theatreData)
    {
        for (const auto &theatreEntry : townEntry.second)
        {
            for (const auto &playEntry : theatreEntry.second)
            {
                uniquePlays.insert(playEntry.first);
            }
        }
    }

    // Prints the unique play names in alphabetical order
    for (const auto &play : uniquePlays)
    {
        // If a play name contains a slash '/', it splits the name into two parts
        // and prints them as separate play names with '***' between.
        size_t slashPos = play.find('/');
        if (slashPos != string::npos)
        {
            string firstPart = play.substr(0, slashPos);
            string secondPart = play.substr(slashPos + 1);

            cout << firstPart << " *** " << secondPart;
        }
        else
        {
            cout << play;
        }
        cout << endl;
    }
}


// This function prints all theatres offering a given play, including aliases.
// It also prints an error message if no theatre offers the play. It takes a
// reference parameter 'actualPlay': the play for which theaters are being searched, including aliases
// of type string.
void printTheatresOfPlay(const string &actualPlay)
{
    set<string> theatres; // A set to store unique theater names offering the actual play.

    // Iterate through the theatre data to find theaters offering the actual play.
    for (const auto &townEntry : theatreData)
    {
        for (const auto &theatreEntry : townEntry.second)
        {
            // Use find_if to check if actualPlay is in the theatre's lists of plays.
            auto playEntry = find_if(theatreEntry.second.begin(), theatreEntry.second.end(),
                                     [actualPlay](const pair<string, vector<pair<string, int>>> &entry)
                                     {
                                         // Check if the entry's play name contains a slash (indicating an alias).
                                         size_t slashPos = entry.first.find('/');
                                         if (slashPos != string::npos)
                                         {
                                             // Split the play name into two parts and compare them with actualPlay.
                                             string firstPart = entry.first.substr(0, slashPos);
                                             string secondPart = entry.first.substr(slashPos + 1);
                                             return actualPlay == firstPart || actualPlay == secondPart;
                                         }
                                         else
                                         {
                                             // If there's no slash, compare the entry's play name with actualPlay.
                                             return entry.first == actualPlay;
                                         }
                                     });

            if (playEntry != theatreEntry.second.end())
            {
                theatres.insert(theatreEntry.first); // Add the theater to the set if it offers the play.
            }
        }
    }

    // Print theatres that offer the actual play in alphabetical order.
    for (const auto &theatre : theatres)
    {
        cout << theatre << endl;
    }

    if (theatres.empty())
    {
        cout << PLAY_NOT_FOUND << endl; // If no theaters offer the play, display an error message.
    }
}


// This function prints a list of all plays available in a given theater. It takes a reference
// parameter 'theatre' of type string, representing the name of the theater for which plays are
// to be displayed. It also displays error message 'THEATRE_NOT_FOUND' if the specified 'theatre'
// not found and error message 'NOT_AVAILABLE' if no plays are available in the specified 'theatre'.
void printPlaysInTheatre(const string &theatre)
{
    bool theaterExist = false;
    set<string> plays;  // A set to store the unique play names found in the theater.
    for (const auto &townEntry : theatreData)
    {
        for (const auto &theatreEntry : townEntry.second)
        {
            // This will check if the current theater name matches the specified 'theatre'. Set the flag
            // to true if the theater exists. And, will collect the unique play names in the theater.
            if (theatreEntry.first == theatre)
            {
                theaterExist = true;
                for (const auto &playEntry : theatreEntry.second)
                {
                    plays.insert(playEntry.first);
                }
            }
        }
    }

    // Display an error message if the theater is not found.
    if (!theaterExist)
    {
        cout << THEATRE_NOT_FOUND << endl;
    }

    else if (plays.empty())
    {

        cout << NOT_AVAILABLE << endl; // Display a message if no plays are available in the theater.
    }
    else
    {
        // Print the list of play names in alphabetical order.
        for (const auto &play : plays)
        {
            cout << play << endl;
        }
    }
}


// Define a structure to store the aggregated information about plays, theaters, actors, and values.
struct PlayData
{
    string theater;
    string play;
    string actor;
    int value;
};


// This function prints plays in specific town after processing.
// It takes a reference parameter 'town' of type string. It also displays error message
// 'TOWN_NOT_FOUND' if specified 'town' does not exist and error message
// 'NOT_AVAILABLE' if plays are not available in the specified 'town'.
void printPlaysInTown(const string &town)
{
    // Create a list to store aggregated PlayData information.
    vector<PlayData> playDataList;

    // Initialise a flag to check if the specified town exists in the data structure.
    bool townExist = false;

    // Attempt to find the specified town within the theatreData map.
    const auto &townData = theatreData.find(town);

    // Check if the town exists in the data structure.
    if (townData != theatreData.end())
    {
        townExist = true;

        // Iterate through theaters, plays, and players within the town data.
        for (const auto &theatreEntry : townData->second)
        {
            for (const auto &playEntry : theatreEntry.second)
            {
                for (const auto &playerEntry : playEntry.second)
                {
                    // Create a PlayData structure to store the aggregated information.
                    PlayData playData;
                    playData.theater = theatreEntry.first;
                    playData.play = playEntry.first;
                    playData.actor = playerEntry.first;
                    playData.value = playerEntry.second;

                    // Check if an equivalent entry already exists in the list.
                    bool entryExists = false;

                    // Iterate through existing data in the playDataList.
                    for (auto &existingData : playDataList)
                    {
                        // Check if the theater and play names match.
                        if (existingData.theater == playData.theater &&
                            existingData.play == playData.play)
                        {
                            // Update the actor and value if the entry exists.
                            existingData.actor = playData.actor;
                            existingData.value = playData.value;
                            entryExists = true;
                            break;
                        }
                    }

                    // If the entry doesn't exist, add it to the list
                    if (!entryExists)
                    {
                        playDataList.push_back(playData);
                    }
                }
            }
        }
    }

    // Remove elements from 'playDataList' that have a 'value' equal to 0.
    // The 'remove_if' function is used to move all elements matching the condition to the end of the list.
    // Then, 'erase' is used to erase those elements from the list.
    playDataList.erase(
        remove_if(playDataList.begin(), playDataList.end(),
                  [](const PlayData &data)
                  { return data.value == 0; }),
        playDataList.end());

    // Sort the list first by theater name, then by play name.
    /* The lambda function is employed as a custom comparison criterion
     * for sorting the 'playDataList' container. It prioritizes sorting by theater name,
     * and within the same theater, it further sorts by play name. This custom
     * sorting logic ensures that the 'playDataList' is arranged alphabetically
     * first by theater and then by play for a neat presentation of data.
     */
    sort(playDataList.begin(), playDataList.end(),
         [](const PlayData &a, const PlayData &b)
         {
             // If 'theater' names are different, sort by 'theater' name.
             if (a.theater != b.theater)
                 return a.theater < b.theater;

             // If 'theater' names are the same, sort by 'play' name.
             return a.play < b.play;
         });

    if (!townExist)
    {

        cout << TOWN_NOT_FOUND << endl; // Print a town not found error message.
    }
    else if (playDataList.empty())
    {
        cout << NOT_AVAILABLE << endl; // Print a message for no available plays.
    }
    else
    {
        // Print the aggregated play data for town
        for (const auto &data : playDataList)
        {
            // Split play if it contains "/"
            size_t pos = data.play.find('/');
            if (pos != string::npos)
            {
                // If a slash is found in the play name, it indicates an alias.
                // Split the play name into two parts: before and after the slash.
                string firstPart = data.play.substr(0, pos); // Extract the first part of the play name.
                string secondPart = data.play.substr(pos + 1); // Extract the second part of the play name.

                // Print the theater, the first and second parts of the play name with " --- " in between,
                // and the value associated with this play data.
                cout << data.theater << " : " << firstPart << " --- " << secondPart;
            }
            else
            {
                // If there's no slash in the play name, it's a regular play name.
                // Print the theater and the play name as is.
                cout << data.theater << " : " << data.play;
            }

            // Print the associated value for this play data (i.e., the number of seats).
            cout << " : " << to_string(data.value) << endl;
        }
    }
}


// This function print the list of players in a given play optionally filtered by theatre,
// The function takes two reference parameters, 'play' and 'theatre' of type string.
// It creates a map to group players by theater and processes the theater data structure to find players
// who are associated with the specified play. If the 'theatre' parameter is provided, only the specified theater
// is considered. The function handles play names that include aliases (separated by '/').
// If no players are associated with the specified play, an error message is displayed as 'PLAY_NOT_FOUND'
// If the specified theater is not found, an error message is displayed as 'THEATRE_NOT_FOUND'.
void printPlayersInPlay(const string &play, const string &theatre = "")
{
    bool theaterExist = false;

    // Create a map to group players by theater. Each theater maps to a set of players.
    map<string, set<string>> playersByTheatre;

    // Iterate through town entries in the theater data
    for (const auto &townEntry : theatreData)
    {
        // Iterate through theaters within the current town
        for (const auto &theatreEntry : townEntry.second)
        {
            // Check if the specified theater is empty or matches the current theater
            if (theatre.empty() || theatreEntry.first == theatre)
            {
                theaterExist = true;

                // Iterate through the plays within the current theater
                for (const auto &playEntry : theatreEntry.second)
                {
                    // We use the 'find' function to search for the position of the first occurrence of a '/'
                    // in the play name.
                    size_t slashPos = playEntry.first.find('/');

                    // Check if the play name contains a slash
                    if (slashPos != string::npos)
                    {
                        // If the play name contains a slash, split it into two parts
                        string firstPart = playEntry.first.substr(0, slashPos);
                        string secondPart = playEntry.first.substr(slashPos + 1);

                        // Check if either part matches the provided play
                        if (firstPart == play || secondPart == play)
                        {
                            // We iterate through the players associated with this play
                            // For each player, we insert their name into the 'playersByTheatre' map under the
                            // theater they belong to.
                            for (const auto &playerEntry : playEntry.second)
                            {
                                playersByTheatre[theatreEntry.first].insert(playerEntry.first);
                            }
                        }
                    }
                    else
                    {
                        // If there's no slash in the play name, compare the entire play name
                        if (playEntry.first == play)
                        {
                            // Iterate through the players associated with this play
                            for (const auto &playerEntry : playEntry.second)
                            {
                                // Insert the player into the map under the theater they belong to
                                playersByTheatre[theatreEntry.first].insert(playerEntry.first);
                            }
                        }
                    }
                }
            }
        }
    }

    if (!theaterExist)
    {
        cout << THEATRE_NOT_FOUND << endl;
    }

    else if (playersByTheatre.empty())
    {
        // If no players were found for the specified play, print a relevant error message.
        cout << PLAY_NOT_FOUND << endl;
    }
    else
    {
        // Iterate through the map of players by theater and print the results
        for (const auto &theatrePlayers : playersByTheatre)
        {
            const string &currentTheatre = theatrePlayers.first; // Get the theater name
            const set<string> &currentPlayers = theatrePlayers.second; // Get the set of associated players

            // Iterate through the players in the set and print them with the theater name
            for (const auto &player : currentPlayers)
            {
                cout << currentTheatre << " : " << player << endl;
            }
        }
    }
}


// This function searches for a play by its actual name or alias within the theater data.
// It takes a reference parameter, 'play,'of type string. It first checks if the name
// matches an actual play and returns it. If the name is an alias, it retrieves the
// associated actual play name. If the name is not found, it returns an empty string,
// indicating no match. Useful for identifying actual play names and aliases within the data.
string searchPlay(const string &play)
{
    // Check if the play is a known play or an alias
    const auto &playData = theatreData.find(play);

    // If the 'play' parameter is found within the theater data and it has associated entries,
    // indicating that it is an actual play, the function returns 'play' as it is an actual play name.
    if (playData != theatreData.end() && playData->second.size() > 0)
    {
        return play;
    }
    else
    {
        // If the 'play' parameter is not found as an actual play name, this part of the function
        // searches for 'play' among the aliases for various plays in the theater data. When a match
        // is found, it returns the actual play name associated with the provided alias.
        for (const auto &entry : theatreData)
        {
            const auto &plays = entry.second;
            for (const auto &playEntry : plays)
            {
                if (playEntry.first == play)
                {
                    return playEntry.second.begin()->first; // Returns the first actual play name associated with an alias 'play'.
                }
            }
        }
    }

    return ""; // Return an empty string if the play/alias is not found
}



// This function counts the number of words in a given string. It takes a reference parameter 'str': the input
// string to be analyzed. This function is designed to count individual words separated by spaces.
// If a word is enclosed within double quotes ("), it is treated as a single word. This handling is crucial
// because double-quoted words are often used to include spaces within a single word, and it's important to
// count them as such for accurate word counting.
int getWordCount(const string &str)
{
    // Create a stringstream to split the input string into words.
    istringstream ss(str);
    int count = 0;
    string word;

    while (ss >> word)
    {
        // If the word starts with a double quote, treat it as a quoted word
        if (word.front() == '"')
        {
            // Keep reading until the quoted word ends
            do
            {
                ss >> word;
            } while (word.back() != '"');
        }
        count++;
    }
    return count; // Return the total word count in the input string.
}


// This function manages the main command loop, allowing users to interact with the program.
// It continuously reads user input, splits it into a command and parameters, and executes the
// corresponding function based on the provided command. It handles various commands such as
// 'quit,' 'theatres,' 'plays,' 'plays_in_theatre,' 'theatres_of_play,' 'plays_in_town,' and 'players_in_play.'
// It checks for incorrect parameters and unknown commands, displaying error messages as necessary.
// The function uses a while loop for ongoing interaction and relies on the 'getWordCount' function
// to count the words in a string to help validate command parameters.
void mainLoop()
{
    string userInput; // Initialize a string to store user input.

    while (true)
    {
        cout << PROMPT; // Display a prompt to the user.
        getline(cin, userInput); // Read the user's input as a full line of text.

        // Split user input into command and parameters
        istringstream ss(userInput); // Create a stream to parse the input.
        string command;
        ss >> command; // Extract the first word (the command) from the input.

       // This line uses the `getWordCount` function to count the words in the `userInput` string.
       // The `- 1` is used to subtract 1 from the word count to exclude the command itself.
       // The resulting `paramCount` will represent the count of additional parameters
       // provided by the user, excluding the command.
        int paramCount = getWordCount(userInput) - 1;
        if (command == "quit")
        {
            // If there are any parameters provided with the "quit" command, it's considered incorrect.
            if (paramCount > 0)
            {
                cout << WRONG_PARAMETERS << endl;
            }
            else
            {
                // If no parameters are provided, the user intends to exit the program.
                break;
            }
        }

        // Check if the user has entered the "theatres" command.
        // This block handles the command to display a list of all theaters.
        // If there are no extra parameters, execute the "printTheatres" function
        // to display a list of all theaters.
        else if (command == "theatres")
        {
            // Check for wrong number of parameters for theatres command
            if (paramCount > 0)
            {
                cout << WRONG_PARAMETERS << endl;
            }
            else
            {
                printTheatres();
            }
        }

        // Check if the user has entered the "plays" command.
        // This block handles the command to display a list of unique plays.
        // If there are no extra parameters, execute the "printPlays" function
        // to display a list of unique plays.
        else if (command == "plays")
        {
            // Check for wrong number of parameters for plays command
            if (paramCount > 0)
            {
                cout << WRONG_PARAMETERS << endl;
            }
            else
            {
                printPlays();
            }
        }

        // Check if the user has entered the "plays_in_theatre" command.
        // This block handles the command to display a list of plays in a specific theater.
        // If the user provides the correct number of parameters, extract the theater name
        // and execute the "printPlaysInTheatre" function to display the plays in that theater.
        else if (command == "plays_in_theatre")
        {
            string theatre;

            // Check for wrong number of parameters for plays_in_theatre command
            if (paramCount > 1)
            {
                cout << WRONG_PARAMETERS << endl;
            }
            else
            {

                char quote;

                // Check if the first character is a double quote
                if (ss >> quote && quote == '"')
                {
                    // If there is a double quote, extract content until the end of the double quote
                    getline(ss, theatre, '"');
                }
                else
                {
                    ss.putback(quote); // Put back the non-quote character
                    ss >> theatre;
                }

                // Print theatres corresponding to the extracted play
                printPlaysInTheatre(theatre);
            }
        }

        // Check if the user has entered the "theatres_of_play" command.
        // This block handles the command to display a list of theaters offering a specific play.
        // If the user provides the correct number of parameters, extract the play name
        // and execute the "printTheatresOfPlay" function to display the theaters offering that play.
        else if (command == "theatres_of_play")
        {
            string play;
            // Check for wrong number of parameters for plays_in_town command
            if (paramCount > 1)
            {
                cout << WRONG_PARAMETERS << endl;
            }
            else
            {

                char quote;

                // Check if the user enclosed the play name in double quotes.
                if (ss >> quote && quote == '"')
                {
                    // If the play name is quoted, extract it until the end of the quotes.
                    getline(ss, play, '"');
                }
                else
                {
                    // If the play name is quoted, extract it until the end of the quotes.
                    ss.putback(quote);
                    ss >> play;
                }

                // Print theatres corresponding to the extracted play, including aliases.
                printTheatresOfPlay(play);
            }
        }

        // Check if the user has entered the "plays_in_town" command.
        // This block handles the command to display a list of plays available in a specific town.
        // It checks the number of parameters, and if incorrect, displays a "WRONG_PARAMETERS" error message.
        // If the correct number of parameters is provided, it extracts the town name from the input.
        // It checks if the town name is enclosed in double quotes. If so, it extracts the content between the quotes.
        // Then, it executes the "printPlaysInTown" function to display the list of plays available in the specified town.
        else if (command == "plays_in_town")
        {

            string town;

            // Check for wrong number of parameters for plays_in_town command
            if (paramCount > 1)
            {
                cout << WRONG_PARAMETERS << endl;
            }
            else
            {
                string word; // Temporary variable to hold a word
                char quote; // Variable to check for double quotes

                // Check if the first character is a double quote
                if (ss >> quote && quote == '"')
                {
                    // If there is a double quote, extract content until the end of the double quote
                    getline(ss, town, '"');
                }
                else
                {
                    ss.putback(quote); // Put back the non-quote character
                    ss >> town; // Read the town name directly without quotes
                }

                // Print plays corresponding to the extracted town
                printPlaysInTown(town);
            }
        }

        // Check if the user has entered the "players_in_play" command.
        // This block handles the command to display the players performing in a specific play and theater, if provided.
        // It checks the correctness of the number of parameters and extracts the play and optional theater name.
        // If both parameters are provided, it executes the "printPlayersInPlay" function to display the players
        // performing in the specified play and theater. If only the play is provided, it prints players for that play
        // across all theaters. This block also handles and displays error messages for incorrect parameters.
        else if (command == "players_in_play")
        {
            string play;
            string theatre;
            ss >> play; // Read play directly without checking for quotes

            char quote;
            if (ss >> quote && quote == '"')
            {
                // Check if a theater is provided by looking for double quotes
                getline(ss, theatre, '"');

                // Check for wrong number of parameters for players_in_play command with theatre
                if (paramCount > 2)
                {
                    cout << WRONG_PARAMETERS << endl;
                }
                else
                {
                    printPlayersInPlay(play, theatre); // Execute function to display players for the specified play and theater.
                }
            }
            else
            {
                // No theatre provided, print players for the play from all theatres
                ss.putback(quote); // Put back the non-quote character
                ss >> theatre;
                // Check for wrong number of parameters for players_in_play command without a theatre
                if (paramCount > 2)
                {
                    cout << WRONG_PARAMETERS << endl;
                }
                else
                {
                    printPlayersInPlay(play, theatre); // Execute function to display players for the specified play and all theaters.
                }
            }
        }
        else
        {
            cout << COMMAND_NOT_FOUND << endl; // Display an error message for commands that are not recognized.
        }
    }
}


// This function reads data from the input file and stores it in a structured format.
// Parameters: fileName: Name of the input file to be processed.
// This function validates and organizes the data from the file, which is then
// stored in the theaterData data structure for future access.
// It performs error checking for empty fields, incorrect field count, and file open issues.
void processInputFile(const string &fileName)
{
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) // Check if the file could be opened
    {
        cerr << FILE_ERROR << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    int lineNumber = 0;
    vector<int> linesWithEmptyFields; // Keep track of lines with at least one empty field

    while (getline(inputFile, line))
    {
        lineNumber++;

        // Check for empty fields
        // This code checks input lines for empty or improperly formatted fields by searching for ";;" (empty field),
        // ensuring the correct number of fields, and identifying "; ;" sequences. In case of an issue, it displays
        // an error message and exits the program.
        if (line.find(";;") != string::npos || count(line.begin(), line.end(), ';') < NUMBER_OF_FIELDS - 1 || line.find("; ;") != string::npos)
        {
            cerr << EMPTY_FIELD << lineNumber << endl;
            exit(EXIT_FAILURE);
        }

        // Split the line into fields using stringstream
        istringstream ss(line);
        vector<string> fields;
        string field;
        while (getline(ss, field, ';'))
        {
            fields.push_back(field);
        }

        // Check for the correct number of fields
        if (fields.size() != NUMBER_OF_FIELDS)
        {
            cerr << WRONG_PARAMETERS << endl;
            exit(EXIT_FAILURE);
        }

        // Extract data from fields in each input line. Assign values to variables representing
        // town, theatre, play, player, and the number of free seats. The "numberOfFreeSeats"
        // variable is determined based on whether the input field contains "none" or an integer.
        string town = fields[0];
        string theatre = fields[1];
        string play = fields[2];
        string player = fields[3];
        int numberOfFreeSeats = (fields[4] == "none") ? 0 : stoi(fields[4]);

        // Add data to the 'theatreData' data structure:
        // 'emplace_back' is used to efficiently add an element to the vector associated
        // with 'town' in 'theatreData'. The element is constructed in place and consists
        // of nested data structures for town, theatre, play, player, and numberOfFreeSeats
        theatreData[town].emplace_back(theatre, vector<pair<string, vector<pair<string, int>>>>{{play, {{player, numberOfFreeSeats}}}});
    }

    inputFile.close();
}


// The main function is the entry point of the program.
// It initializes the program, processes the input file, and starts the user interaction loop.

int main()
{
    // Get input file name from the user
    cout << "Input file: ";
    string fileName;
    getline(cin, fileName);

    // Process the input file
    processInputFile(fileName);

    // Start main command loop
    mainLoop();

    return EXIT_SUCCESS;
}
