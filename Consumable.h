/// Description: Header file for Consumable class, inherits from Item class.
///
/// Authors: Martin Pettersson, Christoffer Wiss
///	Version: 2013-11-17
#pragma once
#include "Item.h"
#include "Character.h"

namespace GameLogic
{
	class Character; // Forward declaration solves circular dependancies

	class Consumable : public Item
	{
	public:
		// Consumable constructor.
		Consumable(std::string name, std::string type, int price, double weight, int nrUses, bool consumedOnPickUp) : Item(name, type, price, weight), nrUses_(nrUses), consumedOnPickUp_(consumedOnPickUp){}

		// Applies the effect a consumable has on the character.
		virtual void applyEffect(Character& character) = 0;

		// Returns a detailed string representation of this item.
		virtual std::string printItem() const;

		// Returns true if the consumable was consumed.
		virtual bool isConsumed() const;

		// Returns the number of charges/uses of this consumable item.
		int getNrUses() const;

		// Sets the number of charges/uses of this consumable item.
		void setNrUses(int uses);

		// Returns true if this item is consumed and destroyed when picked up.
		bool isConsumedOnPickup() const;

		// Assigns left-hand Consumable to right-hand Consumable.
		virtual Consumable& operator=(const Consumable& env);

	protected:
		int nrUses_;
		bool consumedOnPickUp_;
	};
}

