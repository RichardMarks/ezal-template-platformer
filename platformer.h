#ifndef PLATFORMER_H

#include "tile_engine.h"

#define PLATFORMER_SCREEN_WIDTH 320
#define PLATFORMER_SCREEN_HEIGHT 200

#define PLATFORMER_TILE_WIDTH 5
#define PLATFORMER_TILE_HEIGHT 5

#define PLATFORMER_TILEMAP_COLUMNS 64
#define PLATFORMER_TILEMAP_ROWS 40
#define PLATFORMER_TILEMAP_LAYERS 2

#define PLATFORMER_USER_DATA_ID 0

struct Platformer {
  struct EZALConfig cfg;
  struct EZALRuntimeAdapter* rta;
  ALLEGRO_BITMAP* atlas_bitmap;
  struct Tileset* tilesets;
  struct Tilemap* tilemap;
  int num_tilesets;
};

extern void platformer_initialize(struct Platformer* platformer);

#define PLATFORMER_H
#endif
