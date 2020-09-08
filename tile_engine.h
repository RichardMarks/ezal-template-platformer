#ifndef TILE_ENGINE_H

#define EZAL_MAX_USER_DATA_PTRS 32
#include <ezal.h>

typedef unsigned char tile_data_type;

struct TileInfo {
  int tileset_x;
  int tileset_y;
};

struct Tileset {
  ALLEGRO_BITMAP* shared_bitmap;
  struct TileInfo* tile_infos;
  int num_tiles_across;
  int num_tile_infos;
  int tile_width;
  int tile_height;
};

struct TilemapLayer {
  tile_data_type* tile_data;
  int tileset_index;
  int scroll_x;
  int scroll_y;
};

struct Tilemap {
  struct TilemapLayer* layers;
  int num_layers;
  int num_columns;
  int num_rows;
};

extern void tilemap_render(
  struct Tileset* tilesets,
  struct Tilemap* tm,
  int offset_x,
  int offset_y);

extern void tileset_init(
  struct Tileset* ts,
  int tile_width,
  int tile_height,
  ALLEGRO_BITMAP* bitmap);

extern void tileset_destroy(struct Tileset* ts);

extern void tilemap_init(
  struct Tilemap* tm,
  int num_columns,
  int num_rows,
  int num_layers);

extern void tilemap_layer_init(
  struct Tilemap* tm,
  int layer_index,
  tile_data_type* layer_data,
  int tileset_index,
  int scroll_x,
  int scroll_y);

extern void tilemap_destroy(struct Tilemap* tm);

// void api_concept () {
//   struct Tileset tileset;
//   tileset_init(&tileset, 5, 5, atlas_bitmap);
//   // uses bitmap size and tile size to calculate
//   // and allocate tile infos and init the tileset properties
//   tileset_destroy(&tileset);
//   // frees tile infos and sets tileset properties to zero

//   struct Tilemap tilemap;
//   tilemap_init(&tilemap, 64, 40, 2);
//   // uses map size and layer count to allocate layers and layer tiles

//   int* layer_data_array;
//   int layer_index = 0;
//   int tileset_index = 0;
//   int scroll_x = 0;
//   int scroll_y = 0;
//   tilemap_layer_init(
//     &tilemap,
//     layer_index,
//     layer_data_array,
//     tileset_index,
//     scroll_x,
//     scroll_y);
//   // init tilemap layer properties and copies layer data

//   tilemap_destroy(&tilemap);
//   // frees tilemap layers and layer tiles
// }


#define TILE_ENGINE_H
#endif
