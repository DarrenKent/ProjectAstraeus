/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#ifndef _WEAPON_
#define _WEAPON_

#include "Engine\Geometry.h"
#include "Engine\Node.h"

class Weapon {
	public:
		enum {
			WEAPON_LASER = 0,
			WEAPON_MISSILE
		};

		Weapon( int damage, int type, Node* node = NULL );	// Constructor
		~Weapon();											// Deconstructor

		void	Fire( float dX, float dY, float dZ );		// Fires the weapon
		void	WeaponLogic( float deltaTime );				// Executes one frame of logic
		void	DrawWeapon();								// Draws the weapon to the screen

		void	SetPosition( float x, float y, float z );	// Sets the position of the weapon

		bool	GetDestroy();								// Returns if the weapon should be destroyed
		float	GetXPos();									// Returns the weapon's X position
		float	GetYPos();									// Returns the weapon's Y position
		float	GetZPos();									// Returns the weapon's Z position
		int		GetDamage();								// Returns the weapon's damage
		int		GetType();									// Returns the weapon type
		Node*	GetNode();									// Returns the weapon node
	protected:
	private:
		bool			mActive;							// Weapon active
		bool			mDestroy;							// Weapon should be destroyed
		double			mTimeActive;						// Weapon's active time
		unsigned int	mDamage;							// Weapon damage
		unsigned int	mType;								// Weapon type
		Node			*mNode;								// Weapon node
		Vector3			mPosition, mTail;					// Weapon position, tail position
		Vector3			mVelocity;							// Weapon velocity
};

#endif														// _WEAPON_