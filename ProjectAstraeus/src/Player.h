/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#ifndef _PLAYER_
#define _PLAYER_

#include "Engine\Node.h"

class Player {
	public:
		Player( Node *node );								// Constructor
		~Player();											// Deconstructor

		void	AddMissile();								// Adds a Missile
		void	ChangeHealth( int amount );					// Changes the Player's Health by the Amount
		void	ChangeShields( int amount );				// Changes the Player's Shields by the Amount
		void	DealDamage( int amount );					// Deals Damage to the Player
		void	FireMissile();								// Fires a missle
		void	KillPlayer();								// Kills the player
		void	MoveForward( float amount, float deltaTime );// Moves the Player forward
		void	PlayerLogic( float deltaTime );				// One Frame of Player Logic

		bool	GetAlive();									// Returns the Player's Alive status
		float	GetDX();									// Returns the Player's X Velocity
		float	GetDY();									// Returns the Player's Y Velocity
		float	GetDZ();									// Returns the Player's Z Velocity
		int		GetHealth();								// Returns the Player's Health
		int		GetMaxHealth();								// Returns the Player's Max Health
		int		GetMaxMissiles();							// Returns the Maximum Number of Missiles
		int		GetMaxShields();							// Returns the Player's Max Shields
		int		GetMissiles();								// Returns the Player's Missile Count
		float	GetRotation();								// Returns the Player's Rotation
		int		GetShields();								// Returns the Player's Shields
		float	GetXPos();									// Returns the Player's X Pos
		float	GetYPos();									// Returns the Player's Y Pos
		float	GetZPos();									// Returns the Player's Z Pos

		void	SetPosition( float x, float y, float z );	// Set Player Position
		void	SetRotation( float rotation );				// Set Player Rotation
		void	SetVelocity( float dx, float dy, float dz );// Set Player Velocity
		void	SetXPos( float x );							// Set Player X Position
		void	SetYPos( float y );							// Set Player Y Position
		void	SetZPos( float z );							// Set Player Z Position
	protected:
	private:
		bool	mAlive;										// Player is Alive
		bool	mChargeTick;								// Charge Flag
		double	mTimeActive;								// Player Active Time
		float	mDX, mDY, mDZ;								// Player Velocity
		float	mRotation;									// Player Rotation
		float	mMaxVelocity;								// Player Maximum Velocity
		float	mXPos, mYPos, mZPos;						// Player Position
		int		mHealth, mShields;							// Player Health and Shields
		int		mMaxHealth, mMaxShields;					// Player Max Health and Shields
		int		mMissiles, mMaxMissiles;					// Player Missiles
		Node	*mNode;										// Player Node

};

#endif														// _PLAYER_