/*Kieran Hoey & Cormac White
Games Development Year 3
D00163930 & D00163463*/

#include "EnemyCharacter.h"

EnemyCharacter::EnemyCharacter(unsigned int id, std::string name, std::string description, unsigned int health,
	std::vector<DynamicItem*> itemPouch, unsigned int wallet, bool state, DynamicItem* equippedLeft,
	DynamicItem* equippedRight, std::string additionalDialogue)
	: Character(id, name, description, health, itemPouch, wallet, state,equippedLeft,equippedRight)
{
	setAdditionalDialogue(additionalDialogue);
}

EnemyCharacter::EnemyCharacter()
{

}

EnemyCharacter::~EnemyCharacter()
{

}

void EnemyCharacter::setAdditionalDialogue(std::string additionalDialogue)
{
	EnemyCharacter::additionalDialogue = (additionalDialogue.length() > 8) ? additionalDialogue : "default dialogue";
}
