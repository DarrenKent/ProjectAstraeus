/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "StarField.h"

StarField::StarField( int xpos, int ypos, int zpos, int width, int height, int depth, int starCount ) {
	srand( (unsigned int)time(NULL) );
	for ( int star = 0; star < starCount; ++star ) {
		int		xMin	= xpos - width;
		int		xMax	= xpos + width;
		int		yMin	= ypos - depth;
		int		yMax	= ypos + depth;
		int		zMin	= zpos - height;
		int		zMax	= zpos + height;
		float	x		= (float)( rand() % ( xMax - xMin) + xMin );
		float	y		= (float)( rand() % ( yMax - yMin) + yMin );
		float	z		= (float)( rand() % ( zMax - zMin) + zMin );

		Vector3 tStarPos;
		tStarPos.x		= x;
		tStarPos.y		= y;
		tStarPos.z		= z;

		float tColor	= ( rand() % 100 ) / 100.0f;

		std::pair< Vector3, float > tStar;
		tStar.first		= tStarPos;
		tStar.second	= tColor;

		mStars.push_back( tStar );
	}
}

void StarField::Draw( float deltaTime ) {
	glPushMatrix();
		glDisable( GL_LIGHTING );
		glEnable( GL_COLOR_MATERIAL );
		glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
		glBegin( GL_POINTS );
			for ( unsigned int star = 0; star < mStars.size(); ++star ) {
				int		tRandDir = rand() % 2;
				float	tDir	 = 1.0f;
				if ( tRandDir == 0 )
					tDir		 = -1.0f;
				float tNewColor  = (float)( ( rand() % 15 ) / 2.0f ) * deltaTime;
				float tColor	 = tDir * tNewColor + mStars[star].second;
				if ( tColor > 1.0f )
					tColor		 = 1.0f;
				if ( tColor < 0.0f )
					tColor		 = 0.0f;
				glColor4f( tColor, tColor, tColor, 1.0f );
				glVertex3f( (GLfloat)mStars[star].first.x, (GLfloat)mStars[star].first.y, (GLfloat)mStars[star].first.z );
				mStars[star].second = tColor;
			}
		glEnd();
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		glDisable( GL_COLOR_MATERIAL );
	glPopMatrix();
}