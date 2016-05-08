#ifndef _LEVEL_H_
#define _LEVLE_H_

#include "audio.h"
#include "types.h"
#include "dict.h"
#include "Tile.h"
#include "entity.h"
#include "player.h"

#include <string.h>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <time.h>
#include <SDL.h>
#include <glib.h>

/**
* @brief loads the level from a config file
*/
void Level_Load();

void Level_Save();

#endif 