/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#ifndef _PARSERS_
#define _PARSERS_

#include <vector>

#include "Engine\StateManager.h"
#include "IK\irrKlang.h"

#include "Game.h"
#include "StateDemo.h"

void CreateAstroidField( DemoState *state, int x, int y, int z, 
						 int width, int height, int maxSize, 
						 int minSize, int count,
						 std::vector< std::string > files,
						 std::vector< std::string > textures ); // Create an Astroid Field
void ParseSettings( Game *game );							    // Parse Settings
void ParseLevel( DemoState *state, irrklang::ISoundEngine *aMgr,
	             std::string filename );					    // Parse a Level File
void SaveSettings( Game *game );							    // Save Settings


#endif															// _PARSERS_