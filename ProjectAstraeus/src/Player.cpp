/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include <iostream>

#include "Player.h"

Player::Player( Node *node ) {
	mAlive			= true;
	mChargeTick		= false;
	mTimeActive		= 0.0;
	mDX				= 0.0f;
	mDY				= 0.0f;
	mDZ				= 0.0f;
	mMaxVelocity	= 400.0f;
	mRotation		= node->GetYaw();
	mXPos			= node->GetXPos();
	mYPos			= node->GetYPos();
	mZPos			= node->GetZPos();
	mHealth			= 1000;
	mMaxHealth		= 1000;
	mMaxMissiles	= 10;
	mMaxShields		= 1000;
	mMissiles		= 10;
	mShields		= 1000;
	mNode			= node;
}

void Player::AddMissile() {
	mMissiles ++;
	if ( mMissiles > mMaxMissiles ) {
		mMissiles = mMaxMissiles;
	}
}

void Player::ChangeHealth( int amount ) {
	mHealth += amount;
	if ( mHealth < 0 )
		mHealth = 0;
	if ( mHealth > mMaxHealth )
		mHealth = mMaxHealth;
}

void Player::ChangeShields( int amount ) {
	mShields += amount;
	if ( mShields < 0 )
		mShields = 0;
	if ( mShields > mMaxShields )
		mShields = mMaxShields;
}

void Player::DealDamage( int amount ) {
	mShields -= amount;
	if ( mShields < 0 ) {
		mHealth += mShields;
		mShields = 0;
	}
	if ( mHealth < 0 )
		mHealth  = 0;
}

void Player::FireMissile() {
	mMissiles --;
}

void Player::KillPlayer() {
	mNode->SetVisible( false );
	mHealth = 0;
	mAlive = false;
}

void Player::MoveForward( float amount, float deltaTime ) {
	mDX += cos( mRotation * 3.14159f / 180.0f ) * amount * deltaTime;
	mDY += sin( mRotation * 3.14159f / 180.0f ) * amount * deltaTime;
	if ( mDX > mMaxVelocity )
		mDX = mMaxVelocity;
	if ( mDX < -mMaxVelocity )
		mDX = -mMaxVelocity;
	if ( mDY > mMaxVelocity )
		mDY = mMaxVelocity;
	if ( mDY < -mMaxVelocity )
		mDY = -mMaxVelocity;
}

void Player::PlayerLogic( float deltaTime ) {
	mTimeActive += deltaTime;

	if ( mAlive ) {
		mXPos += mDX * deltaTime;
		mYPos += mDY * deltaTime;
		mNode->SetPosition( mXPos, mYPos, mZPos );
		mNode->SetRotation( mRotation, mNode->GetPitch(), mNode->GetRoll() );

		if ( !mChargeTick && (int)( mTimeActive * 10 ) % 10 == 0 ) {
			ChangeShields( 1 );
			mChargeTick = true;
		} else if ( (int)( mTimeActive * 10 ) % 10 != 0 )  {
			mChargeTick = false;
		}
	}
}

bool Player::GetAlive() {
	return mAlive;
}

float Player::GetDX() {
	return mDX;
}

float Player::GetDY() {
	return mDY;
}

float Player::GetDZ() {
	return mDZ;
}

int Player::GetHealth() {
	return mHealth;
}

int Player::GetMaxHealth() {
	return mMaxHealth;
}

int Player::GetMaxMissiles() {
	return mMaxMissiles;
}

int Player::GetMaxShields() {
	return mMaxShields;
}

int Player::GetMissiles() {
	return mMissiles;
}

float Player::GetRotation() {
	return mRotation;
}

int Player::GetShields() {
	return mShields;
}

float Player::GetXPos() {
	return mXPos;
}

float Player::GetYPos() {
	return mYPos;
}

float Player::GetZPos() {
	return mZPos;
}

void Player::SetPosition( float x, float y, float z ) {
	mXPos = x;
	mYPos = y;
	mZPos = z;
}

void Player::SetRotation( float rotation ) {
	mRotation = rotation;
}

void Player::SetVelocity( float dx, float dy, float dz ) {
	mDX = dx;
	mDY = dy;
	mDZ = dz;
}

void Player::SetXPos( float x ) {
	mXPos = x;
}

void Player::SetYPos( float y ) {
	mYPos = y;
}

void Player::SetZPos( float z ) {
	mZPos = z;
}