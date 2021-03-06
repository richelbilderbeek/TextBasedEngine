#include "Engine.h"

void Engine::run(Character* player, Room* currentRoom , vector<Room> allRooms, vector<int> roomIdVisited)
{
#pragma region Story

	cout << "Now you begin your story." << endl;

#pragma endregion		

#pragma region Game

	int check = 0;
	string verb, noun;

	while (check != 1)
	{
		player = Engine::getPlayer(*currentRoom);
		cout << "Enter your command: ";
		cin >> verb;
		cin >> noun;


		verb = Engine::toLowerCase(verb);
		noun = Engine::toLowerCase(noun);
		if (verb == "go")
		{
			if (Engine::checkIfDirection(noun))
			{

				DirectionType dir = Engine::getDirection(noun);

				//check if more than one room in direction
				unsigned int numOfRooms = 0, destinationId;
				vector<ConnectedRoom*> connectedRoomIds;
				for (ConnectedRoom cR : currentRoom->getConnectedRooms())
				{
					if (cR.getDirection() == dir)
					{
						numOfRooms++;
						connectedRoomIds.push_back(&cR);
					}
				}
				if (numOfRooms == 0)
					cout << "No rooms due " << noun << "." << endl;
				else {



					if (numOfRooms == 1)
					{
						for (ConnectedRoom cR : currentRoom->getConnectedRooms())
						{
							if (cR.getDirection() == dir)
							{
								Engine::changeRoom(allRooms, currentRoom->getId(), cR.getRoomId(), dir);
								currentRoom = &allRooms[cR.getRoomId()];
								cout << "You are now in " << currentRoom->getName() << "." << endl;
							}
						}
					}
					else
					{
						cout << "multiple rooms in this direction" << endl;
						size_t vecSize = connectedRoomIds.size();
						unsigned int i = 0, selectedRoomNumber;
						for (Room r : allRooms)
						{
							if (r.getId() == connectedRoomIds[i]->getRoomId())
							{
								cout << "[" << i << "] - " << r.getName() << endl;
								i++;
							}
						}
						cout << "enter number of room:";
						cin >> selectedRoomNumber;
						destinationId = connectedRoomIds[selectedRoomNumber]->getRoomId();
						Engine::changeRoom(allRooms, currentRoom->getId(), destinationId, dir);
					}
					bool roomVisited = false;
					for (int id : roomIdVisited)
					{
						if (id == currentRoom->getId())
							roomVisited = true;
					}
					if (roomVisited == false)
						roomIdVisited.push_back(currentRoom->getId());
				}
			}
			else
				cout << "verb go can only be used with direction" << endl;

		}
		else if (verb == "pickup")
		{
			vector<string> itemRoomList;
			for (DynamicItem* dItem : currentRoom->getItems())
				itemRoomList.push_back(Engine::toLowerCase(dItem->getName()));


			for (size_t i = 0; i < itemRoomList.size(); i++)
			{
				if (noun == itemRoomList[i])
				{
					cout << "The " << noun << " was added to your inventory" << endl;
					//need to push item to character item pounc
					for (DynamicItem* dItem : currentRoom->getItems())
					{
						if (Engine::toLowerCase(dItem->getName()) == itemRoomList[i])
						{
							player->pickUp(dItem);
							currentRoom->removeItem(dItem->getId());
						}
					}
					break;
				}
				else
				{
					cout << "There is no such item here" << endl;
					break;
				}
			}
		}
		else if (verb == "lookat")
		{

			if (noun == "inventory")
			{
				player->printItemPouch();
			}
			else if (noun == "room")
			{
				currentRoom->softPrint();
			}
			else if (noun == "scenary")
			{
				string viewScenary;
				currentRoom->printScenaryNames();
				cout << "Do you want to investigate any piece of Scenary? [y/n]" << endl;
				cin >> viewScenary;
				if (viewScenary == "y")
				{
					bool findScenary = false;
					cout << "Enter name of scenary:";
					cin >> viewScenary;
					for (Scenary* sc : currentRoom->getScenary())
					{
						if (sc->getName() == viewScenary)
						{
							sc->print();
							findScenary = true;
						}
					}
					if (findScenary == false)
						cout << "No piece of scenary with that name was found." << endl;
				}
			}
			else if (noun == "characters")
			{
				string viewCharacters;
				currentRoom->printCharacterNames();
				cout << "Do you want to inspect any character? [y/n]" << endl;
				cin >> viewCharacters;
				if (viewCharacters == "y")
				{
					bool findCharacter = false;
					cout << "Enter name of a character:";
					cin >> viewCharacters;
					for (Character* character : currentRoom->getCharacters())
					{
						if (character->getName() == viewCharacters)
						{
							character->print();
							viewCharacters = true;
						}
					}
					if (findCharacter == false)
						cout << "No character with that name was found." << endl;
				}
			}
			else if (noun == "items")
			{
				string viewItems;
				currentRoom->printItemNames();
				cout << "Do you want to inspect any item? [y/n]" << endl;
				cin >> viewItems;
				if (viewItems == "y")
				{
					bool findItem = false;
					cout << "Enter name of item:";
					cin >> viewItems;
					viewItems = (Engine::toLowerCase(viewItems));
					for (DynamicItem* dItem : currentRoom->getItems())
					{
						if (Engine::toLowerCase(dItem->getName()) == viewItems)
						{
							dItem->print();
							viewItems = true;
						}
					}
					if (findItem == false)
						cout << "No item with that name was found." << endl;
				}
			}
			else
			{
				vector<string> itemRoomList;
				vector<string> itemInventoryList;
				for (DynamicItem* dItem : currentRoom->getItems())
				{
					if (Engine::toLowerCase(dItem->getName()) == noun)
						itemRoomList.push_back(Engine::toLowerCase(dItem->getName()));
				}
				for (DynamicItem* dItem : player->getItemPouch())
				{
					if (Engine::toLowerCase(dItem->getName()) == noun)
						itemInventoryList.push_back(Engine::toLowerCase(dItem->getName()));
				}

				vector<string> characterRoomList;
				for (Character* character : currentRoom->getCharacters())
				{
					if (Engine::toLowerCase(character->getName()) == noun)
						characterRoomList.push_back(Engine::toLowerCase(character->getName()));
				}

				vector<string> sceneryRoomList;
				for (Scenary* scene : currentRoom->getScenary())
				{
					if (Engine::toLowerCase(scene->getName()) == noun)
						sceneryRoomList.push_back(scene->getName());
				}

				if (itemRoomList.size() > 0)
				{
					for (size_t i = 0; i < itemRoomList.size(); i++)
					{
						if (noun == itemRoomList[i])
						{
							cout << "Found in Room" << endl;
							for (DynamicItem* dItem : currentRoom->getItems())
							{
								if (Engine::toLowerCase(dItem->getName()) == itemRoomList[i])
									dItem->print();
							}
							break;
						}
						else {
							cout << "There is no such item here" << endl;
							break;
						}
					}
				}
				else if (itemInventoryList.size() > 0)
				{
					for (size_t i = 0; i < itemInventoryList.size(); i++)
					{
						if (noun == itemInventoryList[i])
						{
							cout << "Found in Inventory" << endl;
							for (DynamicItem* dItem : player->getItemPouch())
							{
								if (Engine::toLowerCase(dItem->getName()) == itemInventoryList[i])
									dItem->print();
							}
							break;
						}
						else {
							cout << "There is no such item here" << endl;
							break;
						}
					}
				}
				else if (characterRoomList.size() > 0)
				{
					for (size_t i = 0; i < characterRoomList.size(); i++)
					{
						if (noun == characterRoomList[i])
						{
							for (Character* character : currentRoom->getCharacters())
							{
								if (character->getName() == characterRoomList[i])
									character->print();
							}
							break;
						}
						else {
							cout << "There is no such character here" << endl;
							break;
						}
					}
				}
				else if (sceneryRoomList.size() > 0)
				{
					for (size_t i = 0; i < sceneryRoomList.size(); i++)
					{
						if (noun == sceneryRoomList[i])
						{
							for (Scenary* scene : currentRoom->getScenary())
							{
								if (scene->getName() == sceneryRoomList[i])
									scene->print();
							}
							break;
						}
						else {
							cout << "There is no scenery with that name here" << endl;
							break;
						}
					}
				}
				else
				{
					cout << "There is nothing with that name here" << endl;
				}
			}
		}
		else if (verb == "use")
		{
			if (noun == "item")
			{
				string viewItems;
				player->softPrintItemPouch();
				cout << "Do you want to inspect any item? [y/n]" << endl;
				cin >> viewItems;
				if (viewItems == "y")
				{
					bool findItem = false;
					cout << "Enter name of item:";
					cin >> viewItems;
					viewItems = (Engine::toLowerCase(viewItems));
					for (DynamicItem* dItem : player->getItemPouch())
					{
						if (Engine::toLowerCase(dItem->getName()) == viewItems)
						{
							dItem->print();
							viewItems = true;
						}
					}
					if (findItem == false)
						cout << "No item with that name was found in inventory." << endl;
				}
			}
			else
			{
				vector<string> itemInventoryList;
				for (DynamicItem* dItem : player->getItemPouch())
				{
					if (Engine::toLowerCase(dItem->getName()) == noun)
						itemInventoryList.push_back(Engine::toLowerCase(dItem->getName()));
				}

				if (itemInventoryList.size() > 0)
				{
					for (size_t i = 0; i < itemInventoryList.size(); i++)
					{
						if (noun == itemInventoryList[i])
						{
							for (DynamicItem* dItem : player->getItemPouch())
							{
								if (Engine::toLowerCase(dItem->getName()) == itemInventoryList[i])
									dItem->useItem();
							}
							break;
						}
						else {
							cout << "There is no such item in inventory" << endl;
							break;
						}
					}
				}
				else
					cout << "There is no such item in inventory" << endl;
			}
		}
		else if (verb == "view")
		{
			if (noun == "stats")
			{
				int wealthValue = 0;
				for (DynamicItem* dItem : player->getItemPouch())
					wealthValue += dItem->getValue();
				cout << "Number of unique rooms visited :\t" << roomIdVisited.size() << endl;
				cout << "Number of items in inventory :\t" << player->getItemPouch().size() << endl;
				cout << "Number of times items have been used :\t" << "uses" << endl;
				cout << "Total value of items in inventory :\t" << wealthValue << " coins" << endl;
				cout << "Current Health :\t" << player->getHealth() << "/100" << endl;
				cout << "Current Wealth :\t" << player->getWallet() << " coins" << endl;
			}
			else if (noun == "health")
			{
				cout << "Current Health :\t" << player->getHealth() << "/100" << endl;
			}
			else if (noun == "wallet")
			{
				cout << "Current Wealth :\t" << player->getWallet() << " coins" << endl;
			}
		}
		else if (verb == "help")
		{
			if (noun == "controls")
			{
				cout << "Keywords:" << endl;
				cout << "Go ...\nPickup ...\nLookat ...\nUse ...\nView ..." << endl;
				cout << "Directions:" << endl;
				cout << "North\nWest\nEast\nSouth\nNorthWest\nNorthEast\nSouthWest\nSouthEast" << endl;
			}
			else if (noun == "rooms")
			{
				//write info about rooms and what kind of interactions can be made
				cout << "rooms..." << endl;
			}
			else if (noun == "items")
			{
				//write info about items and what kind of interactions can be made
				cout << "items..." << endl;
			}
			else if (noun == "characters")
			{
				//write info about characters and what kind of interactions can be made
				cout << "characters..." << endl;
			}
			else if (noun == "scenary")
			{
				//write info about scenary and what kind of interactions can be made
				cout << "scenary..." << endl;
			}
			else
			{
				cout << "After the word help type one of the following for more information.\ncontrols\nrooms\nitems\ncharacters\nscenary" << endl;
			}
		}
		else if (verb == "settings")
		{
			if (noun == "quit")
			{
				string quit;
				cout << "Progress will not be saved. IF you wish to save type 'settings save'. Are you sure you wish to quit? [y/n]" << endl;
				cin >> quit;
				if (quit == "y")
				{
					cout << "Game Has Ended" << endl;
					check = 1;
				}
				else
					cout << "canceled quiting" << endl;
			}
			else if (noun == "save")
			{
				//implement save to file
			}
			else if (noun == "load")
			{
				//implement load from file
			}
			else
			{
				cout << "Enter one of the following settings commands:-\nquit\nsave\nload" << endl;
			}
		}
		else
		{
			cout << "No valid verb. Type 'help controls' for a list of all commands" << endl;
		}
	}

#pragma endregion

}

#pragma region Character/Room

bool Engine::checkIfItem(Room& currentRoom, string noun)
{
	return true;
}

bool  Engine::checkIfCharacter(Room& currentRoom, string noun)
{
	return true;
}

bool  Engine::checkIfScenary(Room& currentRoom, string noun)
{
	return true;
}

bool  Engine::checkIfRoom(vector<Room> allRooms, string noun)
{
	string nameLowerCase;
	for (Room r : allRooms)
	{
		nameLowerCase = toLowerCase(r.getName());
		if (nameLowerCase == noun)
			return true;
	}
	return false;
}

bool  Engine::checkIfDirection(string s)
{
	if (s == "north")
		return true;
	else if (s == "northeast")
		return NorthEast;
	else if (s == "east")
		return true;
	else if (s == "southeast")
		return true;
	else if (s == "south")
		return true;
	else if (s == "southwest")
		return true;
	else if (s == "west")
		return true;
	else if (s == "northwest")
		return true;
	else
		return false;
}

size_t  Engine::getRoomIdWithPlayer(vector<Room>& allRooms)
{
	size_t size = allRooms.size();
	for (size_t i = 0; i<size; i++)
	{
		Character* c;
		c = getPlayer(allRooms[i]);
		if (&c != NULL)
			return i;
	}
	return NULL;
}

Character*  Engine::getPlayer(Room& currentRoom)
{
	vector<Character*> chars = currentRoom.getCharacters();
	for (Character* c : chars)
	{
		if (c->getId() == 1)
			return c;
	}
	return NULL;
}

void  Engine::changeRoom(vector<Room>& allRooms, unsigned int currentRoomId, unsigned int destinationRoomId, DirectionType direction)
{
	vector<ConnectedRoom> cRooms = allRooms[currentRoomId].getConnectedRooms();
	for (ConnectedRoom cR : cRooms)
	{
		if (cR.getRoomId() == destinationRoomId)
		{
			if (cR.getDirection() == direction)
			{
				Character* player = getPlayer(allRooms[currentRoomId]);
				allRooms[destinationRoomId].addCharacter(player);
				allRooms[currentRoomId].removeCharacter(player->getId());
				cout << player->getName() << " left " << allRooms[currentRoomId].getName()
					<< " and went to " << allRooms[destinationRoomId].getName() << endl;
			}
			else
			{
				cout << "No valid room in that direction" << endl;
			}
		}
	}
}


DirectionType  Engine::getDirection(int x)
{
	switch (x) {
	case 0:
		return North;
	case 1:
		return NorthEast;
	case 2:
		return East;
	case 3:
		return SouthEast;
	case 4:
		return South;
	case 5:
		return SouthWest;
	case 6:
		return West;
	case 7:
		return NorthWest;
	default:
		return North;
	}
}

DirectionType  Engine::getDirection(string s)
{
	s = toLowerCase(s);

	if (s == "north")
		return North;
	else if (s == "northeast")
		return NorthEast;
	else if (s == "east")
		return East;
	else if (s == "southeast")
		return SouthEast;
	else if (s == "south")
		return South;
	else if (s == "southwest")
		return SouthWest;
	else if (s == "west")
		return West;
	else if (s == "northwest")
		return NorthWest;
	else
		return North;
}

string  Engine::toLowerCase(string s)
{
	unsigned int length = s.length();
	for (unsigned int i = 0; i<length; i++)
		s[i] = tolower(s[i]);
	return s;
}

#pragma endregion

#pragma region Populate

void  Engine::poulateConnectedRoomVector(vector<ConnectedRoom>& cRoomVector, vector<string> cRooms, string delimiter)
{
	stringstream ss;
	int dir;
	unsigned int roomId;
	DirectionType direction;
	bool isLocked;

	for (string s : cRooms)
	{
		vector<string> connectedRoom = split(s, delimiter);
		ss << connectedRoom[0];
		ss >> roomId;
		ss.clear();
		ss << connectedRoom[1];
		ss >> dir;
		ss.clear();
		direction = getDirection(dir);
		isLocked = to_bool(connectedRoom[2]);
		cRoomVector.push_back(ConnectedRoom(roomId, direction, isLocked));
	}
}

void  Engine::populateScenaryVector(vector<Scenary*>& scenaryVector, vector<string> scenaryPieces, string delimiter)
{
	/*
	1 | table | a varnished table | it is covered in old cutlery | true
	*/

	stringstream ss;
	unsigned int sceneId;
	string name, desc, additionalDialogue;
	bool state;

	for (string s : scenaryPieces)
	{
		vector<string> scenaryPiece = split(s, delimiter);
		ss << scenaryPiece[0];
		ss >> sceneId;
		ss.clear();
		name = scenaryPiece[1];
		desc = scenaryPiece[2];
		additionalDialogue = scenaryPiece[3];
		state = to_bool(scenaryPiece[4]);
		scenaryVector.push_back(createScenary(sceneId, name, desc, additionalDialogue, state));
	}
}

void  Engine::populateCharacterVector(vector<Character*>& characterVector, vector<string> characters, string delimiter)
{
	/*
	***Characters***
	char~char~char~char
	plr|id|name|des|health|items|wallet|state|leftHand|rightHand~
	or
	enemy|id|name|des|health|items|wallet|state|leftHand|rightHand~
	or
	npc|id|name|des|health|items|wallet|state|leftHand|rightHand~
	*/


	stringstream ss;
	int health, wallet;
	unsigned int charId;
	string type, name, desc, leftHand, rightHand;
	bool state, onlyOne = true;
	vector<DynamicItem*> dynamicItems;

	for (string s : characters)
	{

		vector<string> character = split(s, delimiter);
		character[0] = type;
		ss << character[1];
		ss >> charId;
		ss.clear();
		name = character[2];
		desc = character[3];
		ss << character[4];
		ss >> health;
		ss.clear();
		vector<DynamicItem*> dynamicItems;
		vector<string> items = split(character[5], "#");
		populateItemVector(dynamicItems, items, ",");
		ss << character[6];
		ss >> wallet;
		ss.clear();
		state = to_bool(character[7]);
		leftHand = character[8];
		rightHand = character[9];
		if (type == "plr" && onlyOne == true)
		{
			//create Player
			characterVector.push_back(createPlayer(charId, name, desc, health, dynamicItems, wallet, state, populateItem(leftHand, ","), populateItem(rightHand, ",")));
			onlyOne = false;
		}
		else if (type == "enemy")
		{
			//create Enemy
			string additionalDialogue = character[8];
			characterVector.push_back(createEnemy(charId, name, desc, health, dynamicItems, wallet, state, populateItem(leftHand, ","), populateItem(rightHand, ","), additionalDialogue));
		}
		else
		{
			//create NPC
			characterVector.push_back(createNPC(charId, name, desc, health, dynamicItems, wallet, state, populateItem(leftHand, ","), populateItem(rightHand, ",")));
		}
	}
}

void  Engine::populateItemVector(vector<DynamicItem*>& dynamicItems, vector<string>items, string delimiter)
{
	for (string s : items)
		dynamicItems.push_back(populateItem(s, delimiter));
}

DynamicItem*  Engine::populateItem(string itemAsString, string delimiter)
{
	stringstream ss;
	int atk, def, spd, effectVal;
	unsigned int itemId, lockId, val, uses;
	string name, desc;
	bool state;
	ConsumableType consType;

	vector<string> item = split(itemAsString, delimiter);

	ss << item[0];
	ss >> itemId;
	ss.clear();
	name = item[1];
	desc = item[2];
	ss << item[3];
	ss >> val;
	ss.clear();
	state = to_bool(item[4]);
	ss << item[5];
	ss >> uses;
	ss.clear();


	if (item.size() == 7)
	{
		//create Key
		lockId = to_bool(item[6]);
		return createKey(itemId, name, desc, val, state, uses, lockId);
	}
	else if (item.size() == 8)
	{
		//create Consumable
		/*ss << item[6];
		ss >> consType;*/
		consType = healthPotion;
		ss << item[7];
		ss >> effectVal;
		ss.clear();
		return createConsumable(itemId, name, desc, val, state, uses, consType, effectVal);
	}
	else if (item.size() == 9)
	{
		//create Weapon
		ss << item[6];
		ss >> atk;
		ss.clear();
		ss << item[7];
		ss >> def;
		ss.clear();
		ss << item[8];
		ss >> spd;
		ss.clear();
		return createWeapon(itemId, name, desc, val, state, uses, atk, def, spd);
	}
	else
	{
		return new DynamicItem(itemId, name, desc, val, state, uses);
	}
}

#pragma endregion

#pragma region Create Classes

Consumable*  Engine::createConsumable(unsigned int id, string name, string desc, int val, bool state, int uses, ConsumableType consType, int effectVal)
{
	return new Consumable(id, name, desc, val, state, uses, consType, effectVal);
}

Weapon*  Engine::createWeapon(unsigned int id, string name, string desc, int val, bool state, int uses, int atk, int def, int spd)
{
	return new Weapon(id, name, desc, val, state, uses, atk, def, spd);
}

Key*  Engine::createKey(unsigned int id, string name, string desc, int val, bool state, int uses, unsigned int lockId)
{
	return new Key(id, name, desc, val, state, uses, lockId);
}

Character* Engine::createNPC(unsigned int id, string name, std::string description, unsigned int health, vector<DynamicItem*> itemPouch, unsigned int wallet, bool state, DynamicItem* equippedLeft, DynamicItem* equippedRight)
{
	return new Character(id, name, description, health, itemPouch, wallet, state, equippedLeft, equippedRight);
}

EnemyCharacter* Engine::createEnemy(unsigned int id, string name, std::string description, unsigned int health, vector<DynamicItem*> itemPouch, unsigned int wallet, bool state, DynamicItem* equippedLeft, DynamicItem* equippedRight, string additionalDialogue)
{
	return new EnemyCharacter(id, name, description, health, itemPouch, wallet, state, equippedLeft, equippedRight, additionalDialogue);
}

PlayerCharacter*  Engine::createPlayer(unsigned int id, string name, std::string description, unsigned int health, vector<DynamicItem*> itemPouch, unsigned int wallet, bool state, DynamicItem* equippedLeft, DynamicItem* equippedRight)
{
	return new PlayerCharacter(id, name, description, health, itemPouch, wallet, state, equippedLeft, equippedRight);
}

Scenary*  Engine::createScenary(unsigned int id, std::string name, std::string description, std::string additionalDialogue, bool state)
{
	return new Scenary(id, name, description, additionalDialogue, state);
}

#pragma endregion

#pragma region loadGame

void  Engine::loadGameFromFile(vector<Room>& allRooms)
{
	stringstream ss;
	unsigned int roomId;
	string name;
	string description;
	string itemsString;
	string charString;
	string sceneString;
	string cRoomString;


	/******************************Code by Maik Beckmann from http://stackoverflow.com/questions/2912520/read-file-contents-into-a-string-in-c *********************************/

	ifstream inFile("c:/temp/game.txt");
	string gameFile((std::istreambuf_iterator<char>(inFile)),
		(std::istreambuf_iterator<char>()));

	vector<string> rooms = split(gameFile, "/");


	for (string s : rooms)
	{
		vector<string> roomVar = split(s, "-");
		//Break down Room

		ss << roomVar[0];
		ss >> roomId;
		ss.clear();
		name = roomVar[1];
		description = roomVar[2];
		itemsString = roomVar[3];
		charString = roomVar[4];
		sceneString = roomVar[5];
		cRoomString = roomVar[6];

		//Break down items
		vector<DynamicItem*> dynamicItems;
		vector<string> items = split(itemsString, "~");
		populateItemVector(dynamicItems, items, "|");

		vector<Character*> characterVector;
		vector<string> characters = split(charString, "~");
		populateCharacterVector(characterVector, characters, "|");

		vector<Scenary*> scenaryVector;
		vector<string> scenary = split(sceneString, "~");
		populateScenaryVector(scenaryVector, scenary, "|");

		vector<ConnectedRoom> cRoomVector;
		vector<string> cRoom = split(cRoomString, "~");
		poulateConnectedRoomVector(cRoomVector, cRoom, "|");


		allRooms.push_back(Room(roomId, name, description,
			dynamicItems,
			characterVector,
			scenaryVector,
			cRoomVector));
	}

	inFile.close();
}

#pragma endregion

#pragma region Sourced Code

/******************************Code by Georg Fritzsche from http://stackoverflow.com/questions/3613284/c-stdstring-to-boolean *********************************/
bool  Engine::to_bool(string str)
{
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	istringstream is(str);
	bool b;
	is >> std::boolalpha >> b;
	is.clear();
	return b;
}

/******************************Code by Niall McGuinness from moodle*********************************/
vector<string>  Engine::split(string data, string delimiter)
{
	vector<string> outVector;
	string strElement;
	size_t oldPos = -1;
	size_t pos = data.find(delimiter, oldPos + 1);
	while (pos != string::npos)
	{
		strElement = data.substr(oldPos + 1, pos - oldPos - 1);
		outVector.push_back(strElement);
		oldPos = pos;
		pos = data.find(delimiter, oldPos + 1);
	}
	return outVector;
}

#pragma endregion