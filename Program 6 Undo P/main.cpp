/* prog6Wumpus.cpp
 
 Description: Navigate a maze to kill the Wumpus.
 Implementation of a 1972 game by Gregory Yob
 
 Results of a partial run are shown below. This version differs from what
 was required in the program description in these ways:
 1. It displays the cave rooms each time
 2. When an arrow ricochets, it only ricochets the total distance originally specified
 and not always 3 rooms
 3. It handles 'D' to display board
 3. It does not implement undo
 
 
 Author: Tejaswi Reddy                 ______18______
 CS 141 Program #6: Wumpus        /      |       \
 TA: Elliott Ness, T 6:00 AM     /      _9__      \
 April 3, 2016                  /      /    \      \
 System: XCode on Mac          /      /      \      \
                             17     8        10     19
                             |  \   / \      /  \   / |
 Hunt the Wumpus:            |   \ /   \    /    \ /  |
 The Wumpus lives in a       |    7     1---2     11  |
 completely dark cave of     |    |    /     \    |   |
 20 rooms.  Each room has    |    6----5     3---12   |
 3 tunnels leading to other  |    |     \   /     |   |
 rooms as shown:             |    \       4      /    |
                             |     \      |     /     |
                              \     15---14---13     /
                               \   /            \   /
                                \ /              \ /
                                 16---------------20
 Hazards:
 1. Two rooms have bottomless pits in them.  If you go there you fall and die.
 2. Two other rooms have super-bats.  If you go there, a bats grab you and
 fly you to some random room, which could be troublesome.  Then those bats
 go to some random room.
 3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and
 is too heavy for a bat to lift.  Usually he is asleep.  Two things wake
 him up: Anytime you shoot an arrow, or you entering his room.  When he
 wakes he moves one room 75% of the time, staying still the other 25% of
 the time. After that, if he is in your room, he eats you and you die!
 
 Moves:
 On each move you can:
 1. Move into an adjacent room.  To move enter 'M' followed by a space and
 then a room number.
 2. Shoot your guided arrow through a list of three adjacent rooms, which
 you specify.  Your arrow ends up in the final room. If an arrow
 can't go a direction because there is no connecting tunnel, it ricochets
 and moves to a randomly selected adjacent room and continues
 doing this until it has traveled 3 rooms in total.  If the arrow
 hits the Wumpus, you win! If the arrow hits you, you lose. You
 automatically pick up an arrow if you go through a room with an arrow in
 it. To shoot enter 'S' followed by a list of up to 3 adjacent room numbers
 separated by spaces.
 3. Undo your last move by entering 'U'
 5. Enter 'D' to display the cave map (not required in assignment)
 4. Enter 'x' to exit the game (not required in assignment)
 
 Good luck!
 
 
 
 ______18______
 /      |       \
 /      _9__      \
 /      /    \      \
 /      /      \      \
 17     8        10     19
 | \   / \      /  \   / |
 |  \ /   \    /    \ /  |
 |   7     1---2     11  |
 |   |    /     \    |   |
 |   6----5     3---12   |
 |   |     \   /     |   |
 |   \       4      /    |
 |    \      |     /     |
 \     15---14---13     /
 \   /            \   /
 \ /              \ /
 16---------------20
 
 
 You are in room 5.
 You feel a draft.
 
 1. Enter your move: m 3
 Invalid move.  Please retry.
 1. Enter your move: m 4
 
 ______18______
 /      |       \
 /      _9__      \
 /      /    \      \
 /      /      \      \
 17     8        10     19
 | \   / \      /  \   / |
 |  \ /   \    /    \ /  |
 |   7     1---2     11  |
 |   |    /     \    |   |
 |   6----5     3---12   |
 |   |     \   /     |   |
 |   \       4      /    |
 |    \      |     /     |
 \     15---14---13     /
 \   /            \   /
 \ /              \ /
 16---------------20
 
 
 You are in room 4.
 
 
 2. Enter your move: m 3
 
 ______18______
 /      |       \
 /      _9__      \
 /      /    \      \
 /      /      \      \
 17     8        10     19
 | \   / \      /  \   / |
 |  \ /   \    /    \ /  |
 |   7     1---2     11  |
 |   |    /     \    |   |
 |   6----5     3---12   |
 |   |     \   /     |   |
 |   \       4      /    |
 |    \      |     /     |
 \     15---14---13     /
 \   /            \   /
 \ /              \ /
 16---------------20
 
 
 You are in room 3.
 You hear rustling of bat wings.
 
 3. Enter your move: m 2
 Woah... you're flying!
 You've just been transported by bats to room 18
 
 ______18______
 /      |       \
 /      _9__      \
 /      /    \      \
 /      /      \      \
 17     8        10     19
 | \   / \      /  \   / |
 |  \ /   \    /    \ /  |
 |   7     1---2     11  |
 |   |    /     \    |   |
 |   6----5     3---12   |
 |   |     \   /     |   |
 |   \       4      /    |
 |    \      |     /     |
 \     15---14---13     /
 \   /            \   /
 \ /              \ /
 16---------------20
 
 
 You are in room 18.
 
 
 4. Enter your move:
 ...
 */


#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <cassert>
#include <cctype>
#include <ctime>   // to seed random number generator with time(0)
using namespace std;

// global constants
#define TRUE 1
#define FALSE 0
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20


// Used to more conveniently pass all game information between functions
struct GameInfoStruct {
    int moveNumber;  // Counts up from 1, incrementing for each move
    int personRoom;  // Room 1..20 the person currently is in
    int wumpusRoom;  // Room 1..20 the Wumpus is in
    int pitRoom1;     // Room 1..20 the first pit is in
    int pitRoom2;    // Room 1..20 the second pit is in
    int batsRoom1;     // Room 1..20 the first set of bats are in
    int batsRoom2;     // Room 1..20 the second set of bats are in
    int arrowRoom;   // -1 if arrow is with person, else room number 1..20
};


// For linked list used to implement a stack to implement undo
struct StackStruct {
    GameInfoStruct storedGameInfo;  // All the game information
    StackStruct *pNext;             // Next node pointer
};


// Function prototype needed to allow mutual recursion between
//   functions checkForHazards() and inRoomWithBats()
void checkForHazards(GameInfoStruct &, int &, int[][3]);

//--------------------------------------------------------------------------------
void displayIdentifyingInformationAndInstructions()
{
    cout << "                                                        \n"
    << "Author: Tejaswi Reddy                 ______18______             \n"
    << "CS 141 Program #6: Wumpus        /      |       \\           \n"
    << "TA: Elliott Ness, T 6:00 AM     /      _9__      \\          \n"
    << "April 3, 2016                  /      /    \\      \\        \n"
    << "System: XCode on Mac          /      /      \\      \\       \n"
    << "                             17     8        10     19       \n"
    << "                            |  \\   / \\      /  \\   / |    \n"
    << "Hunt the Wumpus:            |   \\ /   \\    /    \\ /  |    \n"
    << "The Wumpus lives in a       |    7     1---2     11  |       \n"
    << "completely dark cave of     |    |    /     \\    |   |      \n"
    << "20 rooms.  Each room has    |    6----5     3---12   |       \n"
    << "3 tunnels leading to other  |    |     \\   /     |   |      \n"
    << "rooms as shown:             |    \\       4      /    |      \n"
    << "                            |     \\      |     /     |      \n"
    << "                             \\     15---14---13     /       \n"
    << "                              \\   /            \\   /       \n"
    << "                               \\ /              \\ /        \n"
    << "                               16---------------20           \n"
    << "Hazards:                                                     \n"
    << "1. Two rooms have bottomless pits in them.  If you go there you fall and die.  \n"
    << "2. Two other rooms have super-bats.  If you go there, a bats grab you and      \n"
    << "   fly you to some random room, which could be troublesome.  Then those bats   \n"
    << "   go to some random room.                                                     \n"
    << "3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and   \n"
    << "   is too heavy for a bat to lift.  Usually he is asleep.  Two things wake     \n"
    << "   him up: Anytime you shoot an arrow, or you entering his room.  When he      \n"
    << "   wakes he moves one room 75% of the time, staying still the other 25% of     \n"
    << "   the time. After that, if he is in your room, he eats you and you die!       \n"
    << "                                                                               \n"
    << "Moves:                                                                         \n"
    << "On each move you can:                                                          \n"
    << "1. Move into an adjacent room.  To move enter 'M' followed by a space and      \n"
    << "   then a room number.                                                         \n"
    << "2. Shoot your guided arrow through a list of three adjacent rooms, which       \n"
    << "   you specify.  Your arrow ends up in the final room. If an arrow             \n"
    << "   can't go a direction because there is no connecting tunnel, it ricochets    \n"
    << "   and moves to a randomly selected adjacent room and continues                \n"
    << "   doing this until it has traveled 3 rooms in total.  If the arrow            \n"
    << "   hits the Wumpus, you win! If the arrow hits you, you lose. You              \n"
    << "   automatically pick up an arrow if you go through a room with an arrow in    \n"
    << "   it. To shoot enter 'S' followed by a list of up to 3 adjacent room numbers  \n"
    << "   separated by spaces.                                                        \n"
    << "3. Undo your last move by entering 'U'                                         \n"
    << "5. Enter 'D' to display the cave map (not required in assignment)              \n"
    << "4. Enter 'x' to exit the game (not required in assignment)                     \n"
    << "                                                                               \n"
    << "Good luck!                                                                     \n"
    << " \n"
    << endl;
}//end displayIdentifyingInformationAndInstructions()


//--------------------------------------------------------------------------------
void displayCave()
{
    cout << "                             \n"
    << "       ______18______             \n"
    << "      /      |       \\           \n"
    << "     /      _9__      \\          \n"
    << "    /      /    \\      \\        \n"
    << "   /      /      \\      \\       \n"
    << "  17     8        10     19       \n"
    << "  | \\   / \\      /  \\   / |    \n"
    << "  |  \\ /   \\    /    \\ /  |    \n"
    << "  |   7     1---2     11  |       \n"
    << "  |   |    /     \\    |   |      \n"
    << "  |   6----5     3---12   |       \n"
    << "  |   |     \\   /     |   |      \n"
    << "  |   \\       4      /    |      \n"
    << "  |    \\      |     /     |      \n"
    << "  \\     15---14---13     /       \n"
    << "   \\   /            \\   /       \n"
    << "    \\ /              \\ /        \n"
    << "    16---------------20           \n"
    << "                                  \n"
    << endl;
}//end displayCave()


//--------------------------------------------------------------------------------
// Return true if randomValue is already in array
int alreadyFound(int randomValue,      // New number we're checking
                 int randomNumbers[],  // Set of numbers previously found
                 int limit)            // How many numbers previously found
{
    int returnValue = false;
    
    // compare random value against all previously found values
    for (int i = 0; i<limit; i++) {
        if (randomValue == randomNumbers[i]) {
            returnValue = true;   // It is already there
            break;
        }
    }
    
    return returnValue;
}//end alreadyFound()


//--------------------------------------------------------------------------------
// Fill this array with unique random integers 1..20
void setUniqueValues(int randomNumbers[],   // Array of random numbers
                     int size)                 // Size of random numbers array
{
    int randomValue = -1;
    
    for (int i = 0; i<size; i++) {
        do {
            randomValue = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20
        } while (alreadyFound(randomValue, randomNumbers, i));
        randomNumbers[i] = randomValue;
    }
}//end setUniqueValues(...)


//--------------------------------------------------------------------------------
// Initialize adjacent room values for the room being worked on
void setRoom(
             int tunnels[],            // The room being set
             int adjacentRoom1,        // Adjacent room numbers
             int adjacentRoom2,
             int adjacentRoom3)
{
    // Set each adjacent room number in turn for this room
    tunnels[0] = adjacentRoom1;
    tunnels[1] = adjacentRoom2;
    tunnels[2] = adjacentRoom3;
}// end setRoom()


//--------------------------------------------------------------------------------
// Set the Wumpus, player, bats and pits in distinct random rooms
void initializeGame(int rooms[][3],            // Array of room connectinos
                    GameInfoStruct &gameInfo)    // All other game settings variables
{
    // Array of 6 unique values, to be used in initializing cave hazards locations
    int randomNumbers[6];
    
    // Initialize cave room connections
    //       ______18______
    //      /      |       \
    //     /      _9__      \
    //    /      /    \      \
    //   /      /      \      \
    //  17     8        10    19
    // |  \   / \      /  \   / |
    // |   \ /   \    /    \ /  |
    // |    7     1---2     11  |
    // |    |    /     \    |   |
    // |    6----5     3---12   |
    // |    |     \   /     |   |
    // |    \       4      /    |
    // |     \      |     /     |
    //  \     15---14---13     /
    //   \   /            \   /
    //    \ /              \ /
    //    16---------------20
    // First room number is room being set.  Other three numbers are adjacent rooms that are connected
    setRoom(rooms[1], 2, 5, 8);
    setRoom(rooms[2], 1, 3, 10);    setRoom(rooms[3], 2, 4, 12);
    setRoom(rooms[4], 3, 5, 14);    setRoom(rooms[5], 1, 4, 6);     setRoom(rooms[6], 5, 7, 15);
    setRoom(rooms[7], 6, 8, 17);    setRoom(rooms[8], 1, 7, 9);     setRoom(rooms[9], 8, 10, 18);
    setRoom(rooms[10], 2, 9, 11);   setRoom(rooms[11], 10, 12, 19); setRoom(rooms[12], 3, 11, 13);
    setRoom(rooms[13], 12, 14, 20); setRoom(rooms[14], 4, 13, 15);  setRoom(rooms[15], 6, 14, 16);
    setRoom(rooms[16], 15, 17, 20); setRoom(rooms[17], 7, 16, 18);  setRoom(rooms[18], 9, 17, 19);
    setRoom(rooms[19], 11, 18, 20); setRoom(rooms[20], 13, 16, 19);
    
    // Select some unique random values 1..20 to be used for 2 bats rooms, 2
    // pits rooms, Wumpus room, and initial player room
    setUniqueValues(randomNumbers, 6);
    // Use the unique random numbers to set initial locations of hazards, which
    //    should be non-overlapping
    gameInfo.arrowRoom = -1;    // -1 value indicates player has the arrow
    gameInfo.batsRoom1 = randomNumbers[0];  gameInfo.batsRoom2 = randomNumbers[1];
    gameInfo.pitRoom1 = randomNumbers[2];  gameInfo.pitRoom2 = randomNumbers[3];
    gameInfo.wumpusRoom = randomNumbers[4];
    gameInfo.personRoom = randomNumbers[5];
    
    gameInfo.moveNumber = 1;
}// end initializeBoard()


//--------------------------------------------------------------------------------
// Returns true if nextRoom is adjacent to current room, else returns false.
int roomIsAdjacent( int tunnels[3],     // Array of adjacent tunnels
                   int nextRoom)        // Desired room to move to
{
    return(tunnels[0] == nextRoom ||
           tunnels[1] == nextRoom ||
           tunnels[2] == nextRoom
           );
}//end roomIsAdjacent


//--------------------------------------------------------------------------------
// Display where everything is on the board.
void displayCheatInfo(GameInfoStruct gameInfo)
{
    cout << "Game elements are in rooms: \n"
    << "Player: " << gameInfo.personRoom << ";  "
    << "Wumpus: " << gameInfo.wumpusRoom << ";  "
    << "Bats: " << gameInfo.batsRoom1 << " and " << gameInfo.batsRoom2 << ";  "
    << "Pits: " << gameInfo.pitRoom1 << " and " << gameInfo.pitRoom2 << ";  ";
    if (gameInfo.arrowRoom == -1) {
        cout << "Arrow is with player." << "\n";
    }
    else {
        cout << "Arrow is in room " << gameInfo.arrowRoom << "\n";
    }
    cout << endl;
}// end displayCheatInfo()


//--------------------------------------------------------------------------------
// Display room number and hazards detected
void displayRoomInfo(int rooms[][3], GameInfoStruct gameInfo)
{
    // Retrieve player's current room number and display it
    int currentRoom = gameInfo.personRoom;
    cout << "You are in room " << currentRoom << ". " << endl;
    
    // Retrieve index values of all 3 adjacent rooms
    int room1 = rooms[currentRoom][0];
    int room2 = rooms[currentRoom][1];
    int room3 = rooms[currentRoom][2];
    
    // Display hazard detection message if Wumpus is in an adjacent room
    int wumpusRoom = gameInfo.wumpusRoom;
    if (room1 == wumpusRoom || room2 == wumpusRoom || room3 == wumpusRoom) {
        cout << "You smell a stench. ";
    }
    
    // Display hazard detection message if a pit is in an adjacent room
    int pit1Room = gameInfo.pitRoom1;
    int pit2Room = gameInfo.pitRoom2;
    if (room1 == pit1Room || room1 == pit2Room ||
        room2 == pit1Room || room2 == pit2Room ||
        room3 == pit1Room || room3 == pit2Room
        ) {
        cout << "You feel a draft. ";
    }
    
    // Display hazard detection message if bats are in an adjacent room
    int batsRoom1 = gameInfo.batsRoom1;
    int batsRoom2 = gameInfo.batsRoom2;
    if (room1 == batsRoom1 || room1 == batsRoom2 ||
        room2 == batsRoom1 || room2 == batsRoom2 ||
        room3 == batsRoom1 || room3 == batsRoom2
        ) {
        cout << "You hear rustling of bat wings. ";
    }
    
    cout << "\n\n";
}//end displayRoomInfo()


//--------------------------------------------------------------------------------
// Check for bats.
// If the person just moved into a room with bats, the bats transport the person to
//   a random room that is not where they just were, and is not a room that already
//   has bats in it (but it could be a room with the Wumpus or a pit).
void inRoomWithBats(
                    GameInfoStruct &gameInfo,    // Hazards location and game info
                    int &personIsAlive,            // T/F
                    int rooms[][3])                // Gets passed on to check for death by pit or Wumpus
//   which 75% of the time just moves the Wumpus
{
    int randomNewRoom = -1;        // Used to transport person to a new room
    
    // Person is transported by bats
    // Find new random room location that is not where we started
    do {
        randomNewRoom = rand() % NUMBER_OF_ROOMS;
    } while (randomNewRoom == gameInfo.personRoom);
    
    // Move the person to the new room.
    gameInfo.personRoom = randomNewRoom;
    cout << "Woah... you're flying! \n";
    cout << "You've just been transported by bats to room " << gameInfo.personRoom << endl;
    
    // Put the bats into some random room that is not where the person is and is not
    //    where the other bats are.
    // First find out what room the other bats are in
    int otherBatsRoom;
    int *pBatsToBeMoved = NULL;
    if (gameInfo.personRoom == gameInfo.batsRoom1) {
        // Person is in room with batsRoom1, so other bats are in room batsRoom2
        otherBatsRoom = gameInfo.batsRoom2;
        pBatsToBeMoved = &gameInfo.batsRoom1;  // batsRoom1 are the ones to be changed
    }
    else {
        // Person is in room with batsRoom2, so other bats are in room batsRoom1
        otherBatsRoom = gameInfo.batsRoom1;
        pBatsToBeMoved = &gameInfo.batsRoom2;  // batsRoom2 are the ones to be changed
    }
    
    // Find new random room location that is not where the person is now and is not
    //   where the other bats are
    do {
        randomNewRoom = rand() % NUMBER_OF_ROOMS;
    } while (randomNewRoom == gameInfo.personRoom || randomNewRoom == otherBatsRoom);
    
    // Move these bats to this new random room
    *pBatsToBeMoved = randomNewRoom;
    
    // Handle pit or Wumpus in new room if they're there
    int personRoom = gameInfo.personRoom;
    if (personRoom == gameInfo.pitRoom1 || personRoom == gameInfo.pitRoom2 ||
        personRoom == gameInfo.wumpusRoom) {
        checkForHazards(gameInfo, personIsAlive, rooms);
    }
}//end inRoomWithBats()


//--------------------------------------------------------------------------------
// If the player just moved into a room with a pit, the person dies.
// If the person just moved into the wumpus room, then 75% of the time the Wumpus
//   moves to an adjacent room, but 25% of the time the Wumpus kills the person
void checkForHazards(
                     GameInfoStruct &gameInfo,    // Hazards location and game info
                     int &personIsAlive,            // T/F
                     int rooms[][ 3])
{
    // retrieve the room the person is in
    int personRoom = gameInfo.personRoom;
    
    // Check whether there is a pit
    if (personRoom == gameInfo.pitRoom1 || personRoom == gameInfo.pitRoom2) {
        // Person falls into pit
        cout << "Aaaaaaaaahhhhhh....   " << endl;
        cout << "    You fall into a pit and die. \n";
        personIsAlive = false;
        return;
    }
    
    // Check for the Wumpus
    if (personRoom == gameInfo.wumpusRoom) {
        // Wumpus is there. 75% change of Wumpus moving, 25% chance of it killing you
        // Generate a random number 1..100
        if ( (rand() % 100) < 75) {
            // You got lucky and the Wumpus moves away
            cout << "You hear a slithering sound, as the Wumpus slips away. \n"
            << "Whew, that was close! " << endl;
            // Choose a random adjacent room for the Wumpus to go into
            gameInfo.wumpusRoom = rooms[personRoom][rand() % 3];
        }
        else {
            // Wumpus kills you
            cout << "You briefly feel a slimy tentacled arm as your neck is snapped. \n"
            << "It is over." << endl;
            personIsAlive = false;
            return;
        }
    }
    
    // Check for bats, which if present move you to a new room
    if (gameInfo.personRoom == gameInfo.batsRoom1 || gameInfo.personRoom == gameInfo.batsRoom2) {
        inRoomWithBats(gameInfo, personIsAlive, rooms);
    }
}//end checkForHazards()


//--------------------------------------------------------------------------------
// Return the next room number on the input line, or -1 if there are no more numbers
int parseInputForNextRoom(char *&pInput)
{
    int theNumber = -1;        // stores number read, if any
    
    // skip leading space, if any
    while (isspace(*pInput)) {
        pInput++;
    }
    
    // Read the number
    if (isdigit(*pInput)) {
        sscanf(pInput, "%d", &theNumber);
        
        // Advance input pointer to space after number or NULL
        while (isdigit(*pInput)) {
            pInput++;
        }
    }
    
    return theNumber;
}//end parseNextRoom()


//--------------------------------------------------------------------------------
// User shot arrow, providing between 1 and 3 rooms for the arrow to fly through
void shootArrow(int rooms[][3],                // Array of connected rooms
                GameInfoStruct &gameInfo,    // Locations of hazards, user, Wumpus and arrow
                char inputString[],            // User input with 1 to 3 numbers
                int &personIsAlive,            // Whether or not person is alive
                int &wumpusIsAlive)            // Whether or not Wumpus is alive
{
    char *pInput = inputString;            // pointer to use in parsing input
    int roomsForArrowTravel[3];            // Array of up to 3 rooms for arrow to travel through
    int howManyRoomsForArrow = 0;        // How many rooms the user supplies for arrow to travel through
    
    // Successively scan the user input string, storing room numbers into an array
    int nextRoom;    // Will be the next room number, or -1 if there is no more input
    while ( (nextRoom = parseInputForNextRoom( pInput)) != -1) {
        roomsForArrowTravel[howManyRoomsForArrow] = nextRoom;
        howManyRoomsForArrow++;
    }//end while( sscanf...
    
    // Move the arrow through those rooms, ensuring they are adjacent
    gameInfo.arrowRoom = gameInfo.personRoom;  // arrow starts in person room
    for (int i = 0; i < howManyRoomsForArrow; i++) {
        // Ensure room is adjacent before traveling through it
        if (roomIsAdjacent( rooms[gameInfo.arrowRoom], roomsForArrowTravel[i])) {
            // Room was adjacent, so move arrow there
            gameInfo.arrowRoom = roomsForArrowTravel[i];
            // Kill Wumpus if it is there
            if (gameInfo.arrowRoom == gameInfo.wumpusRoom) {
                wumpusIsAlive = false;
                cout << "Wumpus has just been pierced by your deadly arrow! \n"
                << "Congratulations on your victory, you awesome hunter you.\n";
                return;
            }
            // Kill person if path went to a second room and then came right back
            if (gameInfo.arrowRoom == gameInfo.personRoom) {
                personIsAlive = false;
                cout << "You just shot yourself.  \n"
                << "Maybe Darwin was right.  You're dead.\n";
                return;
            }
        }
        else {
            // Error in user input, as indicated room is not adjacent.
            cout << "Room " << roomsForArrowTravel[i] << " is not adjacent.  Arrow ricochets..." << endl;
            // Move arrow to random adjacent rooms to finish its travel.
            for (int j = 0; j < howManyRoomsForArrow - i; j++) {
                // Move arrow to random adjacent room
                gameInfo.arrowRoom = rooms[gameInfo.arrowRoom][rand() % 3];
                
                // If it richochets into room where user is, user dies
                if (gameInfo.arrowRoom == gameInfo.personRoom) {
                    personIsAlive = false;
                    cout << "You just shot yourself, and are dying.\n"
                    << "It didn't take long, you're dead." << endl;
                    return;
                }
                
                // If it ricochets into room where Wumpus is, Wumpus dies
                if (gameInfo.arrowRoom == gameInfo.wumpusRoom) {
                    wumpusIsAlive = false;
                    cout << "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                    << "Accidental victory, but still you win!" << endl;
                    return;
                }
            }//end for( int j...
        }//end else
        
    }//end for (int i = 0; i < howManyRoomsForArrow
    
    // Since arrow was shot, Wumpus hears it.  Choose a random adjacent room for the Wumpus to go into
    gameInfo.wumpusRoom = rooms[ gameInfo.wumpusRoom][ rand() % 3];
}//end shootArrow


//--------------------------------------------------------------------------------
// push a Node onto the stack, to implement undoing moves
void push( StackStruct *&pTop,                   // Top of the stack
          GameInfoStruct existingGameInfo)     // All the information that needs to be stored
{
    // Create a new Node
    StackStruct *pTemp = new StackStruct;
    
    // Copy the existing game information into it
    pTemp->storedGameInfo = existingGameInfo;
    // Set its pointer to point to the current top of the stack
    pTemp->pNext = pTop;
    
    // Reset the top of stack pointer
    pTop = pTemp;
}//end push()


//--------------------------------------------------------------------------------
// pop a Node off the stack, to implement undoing moves.  Return a GameInfoStruct
//   containing all the game information in it.  There should always be at least
//   the initial node on the stack.
GameInfoStruct pop( StackStruct *&pTop)   // pointer to the top of the stack
{
    GameInfoStruct returnValue;
    
    // There should always be at least the initial node on the stack.  If not,
    // Give a warning message and don't pop anything off the stack.
    if( pTop->pNext != NULL) {
        // There is more than just the initial node on the stack
        StackStruct *pTemp = pTop;              // Keep track of the top node
        pTop = pTop->pNext;                     // Advance stack top pointer
        delete pTemp;                           // Delete the top node
        returnValue = pTop->storedGameInfo;     // Retrieve the game info from the top node
    }
    else {
        // Sanity check.  There is nothing on the stack, so we can't pop it
        cout << "*** Error, you can't undo a move past the beginning of the game *** \n";
    }
    
    return returnValue;
}//end pop()


//--------------------------------------------------------------------------------
//  Undoing moves is implemented within main(), using the push() and pop() functions
//  for the game moves stack.
int main()
{
    int rooms[21][3];           // 3 tunnel index values for the 20 rooms. Indexed 0..20, but
    //    we will ignore 0 and only use 1..20
    GameInfoStruct gameInfo;    // Used to more easily pass game info variables around
    int personIsAlive = TRUE;   // Used in checking for end of game
    int wumpusIsAlive = TRUE;   // Used in checking for end of game
    char typeOfMove;            // Used to check for user input
    int nextRoom;               // User input of destination room used on a 'M' type move
    char inputString[MAX_LINE_LENGTH];  // Stores variable size user input for shooting arrow
    StackStruct *pTop = NULL;  // Pointer to top of stack, used to store info for undoing moves
    
    // Seed the random number generator.  Change seed to time(0) to change output each time.
    // srand(time(0));
    srand(1);
    
    // Display author identifying information and game instructions
    displayIdentifyingInformationAndInstructions();
    
    // Set random initial values for person, Wumpus, bats and pits
    initializeGame(rooms, gameInfo);
    
    // Push initial game information onto the stack.  There should always be
    //   at least this one node on the stack, representing the game initial value.
    push( pTop, gameInfo);
    
    // Main playing loop.  Break when player dies, or player kills Wumpus
    while (personIsAlive && wumpusIsAlive) {
        // To assist with play, display the cave. (Not required in assignment)
        displayCave();
        
        // Handle user input inside a loop to allow retrying move after invalid user input
        while ( true) {
            
            // Display current room information: Room number, hazards detected
            displayRoomInfo(rooms, gameInfo);
            
            // Prompt for and handle move
            cout << gameInfo.moveNumber << ". Enter your move: ";
            cin >> typeOfMove;
            typeOfMove = toupper(typeOfMove);  // Make uppercase to facilitate checking
            
            // Check all types of user input and handle them.  This uses if-else-if code
            //   rather than switch-case, so that we can take advantage of break and continue.
            if (typeOfMove == 'M') {
                // Move to an adjacent room
                cin >> nextRoom;
                if (roomIsAdjacent(rooms[gameInfo.personRoom], nextRoom)) {
                    gameInfo.personRoom = nextRoom;        // move to a new room
                    // Check for pit, Wumpus or bats present in this new room
                    checkForHazards(gameInfo, personIsAlive, rooms);
                    // Pick up arrow if it is in this new room
                    if (gameInfo.arrowRoom == gameInfo.personRoom) {
                        cout << "Congratulations, you found the arrow and can once again shoot." << endl;
                        gameInfo.arrowRoom = -1;   // -1 indicates player has arrow
                    }
                    break;   // break out of the move validation loop
                }
                else {
                    cout << "Invalid move.  Please retry. \n";
                    continue;
                }
            }
            else if (typeOfMove == 'S') {
                // Read the 1 to 3 room numbers which are to follow, to shoot arrow
                fgets(inputString, MAX_LINE_LENGTH, stdin);
                if (gameInfo.arrowRoom == -1) {
                    // arrowRoom of -1 means arrow is with the person
                    shootArrow(rooms, gameInfo, inputString, personIsAlive, wumpusIsAlive);
                }
                else {
                    cout << "Sorry, you can't shoot an arrow you don't have.  Go find it.\n";
                }
                break;   // break out of the move validation loop
            }
            else if (typeOfMove == 'C') {
                // Display Cheat information
                displayCheatInfo(gameInfo);
                continue;                       // doesn't count as a move, so retry same move
            }
            else if (typeOfMove == 'X') {
                // Exit program
                personIsAlive = false;           // Indicate person is dead as a way to exit playing loop
                break;
            }
            else if (typeOfMove == 'U') {
                // Undo move.  Ensure we are not at the beginning of the game, with only the initial
                //    node on the stack
                if( pTop->pNext != NULL) {
                    gameInfo = pop( pTop);
                }
                else {
                    // Test this section ***
                    
                    cout << "Sorry, you can't undo past the beginning of the game.  Please retry.\n";
                }
                continue;
            }//end else if...
            
        }//end while( stillValidatingMove)
        
        // Increment the move number and store the game on the stack
        gameInfo.moveNumber++;
        push( pTop, gameInfo);
        
    }//end while( true)
    
    cout << "\nExiting Program ..." << endl;
}//end main()

