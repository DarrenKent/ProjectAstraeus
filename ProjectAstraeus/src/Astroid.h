/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#ifndef _ASTROID_
#define _ASTROID_

#include "Engine\Node.h"

class Astroid {
public:
	Astroid( Node* node );

	bool		AstraeusCollisionPlaying();					// Returns astraeus collision SFX playing
	void		AstroidLogic( float deltaTime );			// Executes one frame of logic
	void		ChangeHealth( int amount );					// Changes the astroid's health
	bool		CheckCollision( float x, float y, 
								float z, float radius );	// Checks to see if the object collided with astroid
	bool		PlayerCollisionPlaying();					// Returns player collision SFX playing
		
	bool		GetOre();									// Returns if this astroid has ore
	float		GetDX();									// Returns astroid's x velocity
	float		GetDY();									// Returns astroid's y velocity
	float		GetDZ();									// Returns astroid's z velocity
	float		GetRadius();								// Returns astroid's radius
	float		GetXPos();									// Returns astroid's x position
	float		GetYPos();									// Returns astroid's y position
	float		GetZPos();									// Returns astroid's z position
	int			GetHealth();								// Returns astroid's health
	std::string GetId();									// Returns astroid's node id

	void		SetActive( bool active );					// Sets the astroid active
	void		SetAstraeusCollisionPlaying( bool playing );// Sets the astraeus collision SFX playing
	void		SetHealth( int amount );					// Sets the astroid's health
	void		SetOre( bool ore );							// Sets the astroid's ore status
	void		SetPlayerCollisionPlaying( bool playing );	// Sets the player collision SFX playing
	void		SetRotationVeloctiy( float yaw, float pitch, 
									 float roll );			// Sets the astroid's rotational velocity
	void		SetVelocity( float dx, float dy, float dz );// Sets the astroid' velocity
protected:
private:
	bool  mActive;											// Astroid Active Flag
	bool  mAstraeusCollisionPlaying;						// Astroid Astraeus Collision Playing
	bool  mOre;												// Astroid Contains Ore
	bool  mPlayerCollisionPlaying;							// Astroid Player Collision Playing
	float mDYaw, mDRoll, mDPitch;							// Astroid Rotation Velocity
	float mDX, mDY, mDZ;									// Astroid Velocity	
	float mXPos, mYPos, mZPos;								// Astroid Position
	float mYaw, mPitch, mRoll;								// Astroid Rotation
	int mHealth;											// Astroid Health
	Node* mNode;											// Astroid Node
};

#endif														//_ASTROID_