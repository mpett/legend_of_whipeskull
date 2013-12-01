/// Description: Header file for Potion class, inherits from Consumable.
///
/// Authors: Martin Pettersson, Christoffer Wiss
///	Version: 2013-11-17
#pragma once
#include "Consumable.h"
#include "Character.h"


namespace GameLogic
{
	class Character; // Forward declaration solves circular dependancies

	class Potion : public Consumable
	{
	public:
		// Potion constructor.
		Potion(std::string name, int price, double weight, int nrUses, bool consumedOnPickUp, int healthAffected) : Consumable(name, "Potion", price, weight, nrUses, consumedOnPickUp), healthAffected_(healthAffected){}

		// Applies the effect a potion has on the character.
		void applyEffect(Character& character);

	private:
		int healthAffected_;

	};

}

