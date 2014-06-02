/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include <vector>

#include "Engine\Geometry.h"

#ifndef _STAR_FIELD_
#define _STAR_FIELD_

class StarField {
public:
	StarField(	int xpos, int ypos, int zpos,
				int width, int height, int depth,
				int starCount );							// Constructor

	void Draw( float deltaTime );							// Draw Star Field
protected:
private:
	std::vector< std::pair< Vector3, float > > mStars;		// List of Stars
};

#endif _STAR_FIELD_