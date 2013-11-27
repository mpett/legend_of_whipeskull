/// Description: The GameEngine maintains the current game, allowing user to provide input, 
///              diplays text to screen and updates world accordingly.
///
/// Authors: Martin Pettersson, Christoffer Wiss
///	Version: 2013-11-27

#include "GameEngine.h"
#include "DungeonRoom.h"
#include "Human.h"
#include "Goblin.h"
#include "NormalSword.h"
#include "NormalArmor.h"
#include <iostream>
#include <algorithm>
using std::string;
using std::cout;
using std::endl;
using std::cin;

namespace GameLogic
{
	// Destructor - Free memory here
	GameEngine::~GameEngine()
	{
		cleanUpGame();
	}

	// Constructor 
	GameEngine::GameEngine()
	{
		actions_.insert(std::pair<string, ACT_PTR>("GO", &Character::go));
		actions_.insert(std::pair<string, ACT_PTR>("MOVE", &Character::go));
		actions_.insert(std::pair<string, ACT_PTR>("TAKE", &Character::pickup));
		actions_.insert(std::pair<string, ACT_PTR>("PICKUP", &Character::pickup));
		actions_.insert(std::pair<string, ACT_PTR>("HELP", &Character::help));
		actions_.insert(std::pair<string, ACT_PTR>("LOOK", &Character::look));
		actions_.insert(std::pair<string, ACT_PTR>("SEE", &Character::look));
		actions_.insert(std::pair<string, ACT_PTR>("DROP", &Character::drop));
		actions_.insert(std::pair<string, ACT_PTR>("PUT", &Character::drop));
		actions_.insert(std::pair<string, ACT_PTR>("INVENTORY", &Character::showInventory));
		actions_.insert(std::pair<string, ACT_PTR>("INV", &Character::showInventory));
		actions_.insert(std::pair<string, ACT_PTR>("SHOW", &Character::showInventory));
		actions_.insert(std::pair<string, ACT_PTR>("STATS", &Character::showStats));
		actions_.insert(std::pair<string, ACT_PTR>("ME", &Character::showStats));
		actions_.insert(std::pair<string, ACT_PTR>("EQUIP", &Character::equip));
		actions_.insert(std::pair<string, ACT_PTR>("UNEQUIP", &Character::unequip));
		actions_.insert(std::pair<string, ACT_PTR>("ATTACK", &Character::attack));
		actions_.insert(std::pair<string, ACT_PTR>("HIT", &Character::attack));
	}

	// Parses input string from player console.
	void GameEngine::processPlayerTurn(Character& character)
	{
		string input;
		bool turnIsOver = false;
		while(!turnIsOver)
		{
			cout << ">";
			cin >> input; // Take first verb
			std::transform(input.begin(), input.end(),input.begin(), ::toupper);

			auto it = actions_.find(input);

			// Could not find action
			if(it == actions_.end())
			{
				cout << "Action is not available." << endl;
				
				// Flush input stream for next command line
				cin.sync();
			}
			// Found action
			else
			{
				std::getline(cin, input);
				cout << endl << startPlayerEventsLine;
				turnIsOver = (character.*(it->second))(input); // Turn is only over if action returns true (i.e. turn is consumed) 
				cout << endSeperatorLine;
			}
		}
	}

	// Returns true if this game has reached an end.
	bool GameEngine::gameOver() const
	{
		// TODO: GameOver
		return false;
	}

	// Loads a saved game state from file.
	bool GameEngine::loadGame(std::string file)
	{
		// TODO : Load
		return false;
	}

	// Saves a game state to file.
	bool GameEngine::saveGame(std::string file) const
	{
		// TODO : Save
		return false;
	}
		
	// Start a new game, create all items and inhabit environments.
	void GameEngine::startNewGame(std::string file)
	{
		// TODO : Start new game
		cout << "This is not a game!" << endl;

		Environment *env1 = new DungeonRoom("A very (very) dark room.");
		Environment *env2 = new DungeonRoom("Another very dark room.");
		
		Character *character = new Human(true,"Joe", "Human", 5, 1, 1, 1, 1, 0, 100, env2);
		character = new Goblin(false,"Goe1", "Goblin", 5, 1, 1, 1, 1, 0, 10, env1);

		env1->addNeigbor(env2, "UP");
		env2->addNeigbor(env1, "DOWN");
		Item *miscItem1 = new Item("Golden Key", "Key", 100, 1);
		env1->addMiscItem(miscItem1);
		Item *miscItem2 = new Item("Silver Key", "Key", 100, 1);
		env2->addMiscItem(miscItem2);
		Equipable *eqItem1 = new NormalSword("Bastard Sword", 50, 5.25, 2, 5, 1);
		Equipable *eqItem2 = new NormalSword("Heavy Sword", 50, 5.25, 2, 5, 10);
		Equipable *eqItem3 = new NormalArmor("Good Amror", 75, 6.25, 25, 1);
		Equipable *eqItem4 = new NormalArmor("Armor of Pain", 1, 10.25, -10, 1);
		env1->addEquipable(eqItem1);
		env1->addEquipable(eqItem2);
		env2->addEquipable(eqItem3);
		env2->addEquipable(eqItem4);
		environments_.push_back(env1);
		environments_.push_back(env2);
	}

	// Deletes all objects associated with current game instance.
	void GameEngine::cleanUpGame()
	{
		for (Environment* room : environments_)
		{
			if(room != nullptr)
			{
				delete room;
			}
		}
		environments_.clear();
	}

	// Updates current state of the game. This is the main method.
	void GameEngine::mainLoop()
	{
		bool gameOver = false;
		string input;
		int menuChoice;
		while(!gameOver) 
		{
			cout << endl << getMainMenu() << endl;
			
			std::getline(cin, input);
			menuChoice = atoi(input.c_str());

			switch(menuChoice)
			{
				case(1):
					startNewGame("file");
					gameLoop();
					break;
				case(2):
					cout << "Good Bye! :)" << endl;
					gameOver = true;
					break;
				default:
					cout << "This is not a menu option." << endl;
					break;
			}
		}
	}

	// Always call a new turn until game is over.
	// The game is played out here.
	void GameEngine::gameLoop()
	{
		bool doGameLoop = true;

		while(doGameLoop)
		{
			doGameLoop = newTurn();
		}
		cleanUpGame();
	}

	// Run each time a new turn has occurred.
	bool GameEngine::newTurn()
	{
		for(Environment * room : environments_)
		{
			room->enableCharacterActions();
		}
		return updateRooms();
	}

	// Goes through each room, runs action for each Character in room.
	bool GameEngine::updateRooms()
	{
		bool thereIsAPlayer = false;
		//cout << endl << startEventsLine;
		for(Environment * room : environments_)
		{
			auto characters = room->getCharacters(); // Not the most effective but it works...
			for(auto it = characters.begin(); it != characters.end(); it++)
			{
				bool controllable = it->second->isControllable();
				// Check if character is alive
				if(it->second->isAlive())
				{
					room->turnEvent(*it->second);
					if(controllable) thereIsAPlayer = true;

					if(it->second->getCanPerformAction())
					{
						bool inNewRoom = it->second->getInNewRoom();
						it->second->setInNewRoom(false);

						// Controllable by user - parse input time
						if(controllable)
						{
							cout << endSeperatorLine;
							cout << "\tIt is now " << it->second->getName() << "'s turn." << endl;
							cout << endSeperatorLine;
							// If at new room for the first time write 
							if(inNewRoom)
							{
								cout << endl;
								it->second->look(""); // Output room information
							}
							processPlayerTurn(*it->second);
						}
						// NPC - Generate random action
						else
						{
							it->second->action();
						}
						it->second->setCanPerformAction(false);
					}
				}
				// Generate loot and remove character
				else
				{
					if(controllable)
					{
						cout << it->second->getName() << " was killed! " << endl;
						
						cout << "\nPress enter to continue . . . ";
						cin.sync();
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						cout << endSeperatorLine;
					}
					it->second->killCharacter(true);
				}
			}
		}

		if(!thereIsAPlayer)
		{
			cout << "All of your character were killed.\n\tGAME OVER" << endl;
			cout << "\nPress enter to continue . . . ";
			cin.sync();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << endSeperatorLine;
		}

		return thereIsAPlayer;
	}

	// Returns a string containing menu options, such as ability to save, load, start a new game or to exit.
	string GameEngine::getMainMenu() const
	{
		// TODO : Menu
		return "1. Play Game \n2. Exit game";
	}
}