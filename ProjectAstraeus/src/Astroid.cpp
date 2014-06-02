/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include "Astroid.h"

Astroid::Astroid( Node* node ) {
	mActive						= false;
	mAstraeusCollisionPlaying	= false;
	mOre						= false;
	mPlayerCollisionPlaying		= false;
	mDYaw						= 0.0f;
	mDPitch						= 0.0f;
	mDRoll						= 0.0f;
	mDX							= 0.0f;
	mDY							= 0.0f;
	mDZ							= 0.0f;
	mYaw						= node->GetYaw();
	mPitch						= node->GetPitch();
	mRoll						= node->GetRoll();
	mXPos						= node->GetXPos();
	mYPos						= node->GetYPos();
	mZPos						= node->GetZPos();
	mHealth						= 100;
	mNode						= node;
}

bool Astroid::AstraeusCollisionPlaying() {
	return mAstraeusCollisionPlaying;
}

void Astroid::AstroidLogic( float deltaTime ) {
	mYaw	+= mDYaw * deltaTime;
	mPitch	+= mDPitch * deltaTime;
	mRoll	+= mDRoll * deltaTime;
	mXPos	+= mDX * deltaTime;
	mYPos	+= mDY * deltaTime;
	mZPos	+= mDZ * deltaTime;

	mNode->SetPosition( mXPos, mYPos, mZPos );
	mNode->SetRotation( mYaw, mPitch, mRoll );
}

void Astroid::ChangeHealth( int amount ) {
	mHealth += amount;
	if ( mHealth < 0 )
		mHealth = 0;
}

bool Astroid::CheckCollision( float x, float y, float z, float radius ) {
	float tDistance = std::sqrt( std::pow( x - mXPos, 2 ) + 
								 std::pow( y - mYPos, 2 ) + 
								 std::pow( z - mZPos, 2 ) );
	if ( tDistance < radius + mNode->GetScale() * 50.0f )
		return true;
	return false;
}

bool Astroid::PlayerCollisionPlaying() {
	return mPlayerCollisionPlaying;
}

bool Astroid::GetOre() {
	return mOre;
}

float Astroid::GetDX() {
	return mDX;
}

float Astroid::GetDY() {
	return mDY;
}

float Astroid::GetDZ() {
	return mDZ;
}

float Astroid::GetRadius() {
	return mNode->GetScale();
}

float Astroid::GetXPos() {
	return mXPos;
}

float Astroid::GetYPos() {
	return mYPos;
}

float Astroid::GetZPos() {
	return mZPos;
}

int Astroid::GetHealth() {
	return mHealth;
}

std::string Astroid::GetId() {
	if ( mNode )
		return mNode->GetId();
	return "";
}

void Astroid::SetActive( bool active ) {
	mActive = active;
}

void Astroid::SetAstraeusCollisionPlaying( bool playing ) {
	mAstraeusCollisionPlaying = playing;
}

void Astroid::SetHealth( int amount ) {
	mHealth = amount;
}

void Astroid::SetOre( bool ore ) {
	mOre = ore;
}

void Astroid::SetPlayerCollisionPlaying( bool playing ) {
	mPlayerCollisionPlaying = playing;
}

void Astroid::SetRotationVeloctiy( float yaw, float pitch, float roll ) {
	mDYaw	= yaw;
	mDPitch = pitch;
	mDRoll	= roll;
}

void Astroid::SetVelocity( float dx, float dy, float dz ) {
	mDX = dx;
	mDY = dy;
	mDZ = dz;
}