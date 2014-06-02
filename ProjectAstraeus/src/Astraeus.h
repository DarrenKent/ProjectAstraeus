/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#ifndef _ASTRAEUS_
#define _ASTRAEUS_

#include "Engine\Node.h"

class Astraeus {
	public:
		Astraeus( Node *node );

		void AstraeusLogic( float deltaTime );				// One frame of logic
		void ChangeHealth( int amount );					// Adjust the astraeus' health
		void ChangeShields( int amount );					// Adjust the astraeus' shields
		void DealDamage( int amount );						// Deal damage to the astraeus

		int GetHealth();									// Returns the astraeus' health
		int GetMaxHealth();									// Returns the astraeus' max health
		int GetMaxShields();								// Returns the astraeus' max shields
		int GetShields();									// Returns the astraeus' shields
		float GetXPos();									// Returns the astraeus x position
		float GetYPos();									// Returns the astraeus y position
	protected:
	private:
		bool	mAlive;										// Alive Flag
		bool	mChargeTick;								// Charge Flag
		double	mTimeActive;								// Time Astraeus is active
		float	mYaw, mPitch, mRoll;						// Astraeus Rotation
		float	mXPos, mYPos, mZPos;						// Astraeus Position
		int		mHealth, mShields;							// Astraeus Health and Shields
		int		mMaxHealth, mMaxShields;					// AStraeus Max Health and Shields
		Node*	mAstraeusNode;								// Astraeus Node
};

#endif // _ASTRAEUS_