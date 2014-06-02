/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include "Engine\Debug.h"

#include "Weapon.h"

Weapon::Weapon( int damage, int type, Node *node ) {
	DebugMessage( "Initializing Weapon...", 4 );
	mActive		= false;
	mDestroy	= false;
	mTimeActive = 0.0f;
	mDamage		= damage;
	mType		= type;
	mNode		= node;
	mPosition	= Vector3();
	mTail		= Vector3();
	mVelocity	= Vector3();
}

Weapon::~Weapon() {
	DebugMessage( "Deleting Weapon...", 4 );
	if ( mNode ) {
		delete mNode;
		mNode = NULL;
	}
}

void Weapon::Fire( float dX, float dY, float dZ ) {
	mActive		= true;
	mVelocity.x = dX;
	mVelocity.y = dY;
	mVelocity.z = dZ;
	mTail.x		= cos( atan( dY / dX ) ) * 2.0f + mPosition.x;
	mTail.y		= sin( atan( dY / dX ) ) * 2.0f + mPosition.y;
}

void Weapon::WeaponLogic( float deltaTime ) {
	if ( mActive ) {
		mTimeActive += (double)deltaTime;
		mPosition.x += mVelocity.x * deltaTime;
		mPosition.y += mVelocity.y * deltaTime;
		mPosition.z += mVelocity.z * deltaTime;
		mTail.x		+= mVelocity.x * deltaTime;
		mTail.y		+= mVelocity.y * deltaTime;
		mTail.z		+= mVelocity.z * deltaTime;

		if ( mNode )
			mNode->SetPosition( mPosition.x, mPosition.y, mPosition.z );

		if ( mTimeActive > 4.0f )
			mDestroy = true;
	}
}

void Weapon::DrawWeapon() {
	glColor4f( 0.0f, 1.0f, 0.0f, 1.0f );
	glLineWidth( 3.0f );
	glBegin( GL_LINES );
		glVertex3f( mPosition.x, mPosition.y, mPosition.z );
		glVertex3f( mTail.x, mTail.y, mTail.z );
	glEnd();
}

void Weapon::SetPosition( float x, float y, float z ){
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
}

bool Weapon::GetDestroy() {
	return mDestroy;
}

float Weapon::GetXPos() {
	return mPosition.x;
}

float Weapon::GetYPos() {
	return mPosition.y;
}

float Weapon::GetZPos() {
	return mPosition.z;
}

int Weapon::GetDamage() {
	return mDamage;
}

int Weapon::GetType() {
	return mType;
}

Node* Weapon::GetNode() {
	return mNode;
}

