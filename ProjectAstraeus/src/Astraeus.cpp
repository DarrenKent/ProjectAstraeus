/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include "Astraeus.h"

Astraeus::Astraeus( Node *node ) {
	mAlive			= true;
	mChargeTick		= false;
	mTimeActive		= 0.0;
	mYaw			= node->GetYaw();
	mPitch			= node->GetPitch();
	mRoll			= node->GetRoll();
	mXPos			= node->GetXPos();
	mYPos			= node->GetYPos();
	mZPos			= node->GetZPos();
	mMaxHealth		= 10000;
	mMaxShields		= 10000;
	mHealth			= mMaxHealth;
	mShields		= mMaxShields;
	mAstraeusNode	= node;
}

void Astraeus::AstraeusLogic( float deltaTime ) {
	mTimeActive += deltaTime;

	if ( mAlive ) {
		if ( !mChargeTick && (int)( mTimeActive * 10 ) % 10 == 0 ) {
			ChangeShields( 1 );
			mChargeTick = true;
		} else if ( (int)( mTimeActive * 10 ) % 10 != 0 )  {
			mChargeTick = false;
		}
	}
}

void Astraeus::ChangeHealth( int amount ) {
	mHealth += amount;
	if ( mHealth > mMaxHealth )
		mHealth = mMaxHealth;
	if ( mHealth < 0 )
		mHealth = 0;
}

void Astraeus::ChangeShields( int amount ) {
	mShields += amount;
	if ( mShields > mMaxShields )
		mShields = mMaxShields;
	if ( mShields < 0 )
		mShields = 0;
}

void Astraeus::DealDamage( int amount ) {
	mShields -= amount;
	if ( mShields < 0 ) {
		mHealth += mShields;
		mShields = 0;
	}
	if ( mHealth < 0 )
		mHealth = 0;
}

int Astraeus::GetHealth() {
	return mHealth;
}

int Astraeus::GetMaxHealth() {
	return mMaxHealth;
}

int Astraeus::GetMaxShields() {
	return mMaxShields;
}

int Astraeus::GetShields() {
	return mShields;
}

float Astraeus::GetXPos() {
	return mXPos;
}

float Astraeus::GetYPos() {
	return mYPos;
}



