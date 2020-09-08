#include "tile_engine.h"

/*
  ALLEGRO_BITMAP* shared_bitmap;
  struct TileInfo* tile_infos;
  int num_tiles_across;
  int num_tile_infos;
  int tile_width;
  int tile_height;
*/
void tileset_init(
  struct Tileset* ts,
  int tile_width,
  int tile_height,
  ALLEGRO_BITMAP* bitmap)
{
  if (!ts)
  {
    fprintf(stderr, "tileset_init error: ts is not a valid pointer\n");
    return;
  }

  if (!bitmap)
  {
    fprintf(stderr, "tileset_init error: bitmap is not a valid pointer\n");
    return;
  }

  if (tile_width <= 0)
  {
    fprintf(stderr, "tileset_init error: tile width must be greater than zero\n");
    return;
  }

  if (tile_height <= 0)
  {
    fprintf(stderr, "tileset_init error: tile height must be greater than zero\n");
    return;
  }

  int bitmap_width = al_get_bitmap_width(bitmap);
  int bitmap_height = al_get_bitmap_height(bitmap);

  if (tile_width > bitmap_width)
  {
    tile_width = bitmap_width;
  }

  if (tile_height > bitmap_height)
  {
    tile_height = bitmap_height;
  }

  ts->tile_width = tile_width;
  ts->tile_height = tile_height;
  ts->shared_bitmap = bitmap;

  int num_tiles_down = bitmap_height / tile_height;

  ts->num_tiles_across = bitmap_width / tile_width;
  ts->num_tile_infos = num_tiles_down * ts->num_tiles_across;

  fprintf(
    stdout,
    "tileset_init\n"
    "  bitmap size %dx%d\n"
    "  tile size %dx%d\n"
    "  tiles down %d\n"
    "  tiles across %d\n"
    "  tile infos %d\n",
    bitmap_width,
    bitmap_height,
    tile_width,
    tile_height,
    num_tiles_down,
    ts->num_tiles_across,
    ts->num_tile_infos);

  int tile_info_size = sizeof(struct TileInfo);
  ts->tile_infos = (struct TileInfo*)malloc(
    ts->num_tile_infos * tile_info_size);

  printf("\n  infos:\n");
  for (int i = 0; i < ts->num_tile_infos; i++)
  {
    struct TileInfo* tile_info = &ts->tile_infos[i];
    tile_info->tileset_x = (i % ts->num_tiles_across) * tile_width;
    tile_info->tileset_y = (i / ts->num_tiles_across) * tile_height;
    printf("%8d: (%04d, %04d)\n", i, tile_info->tileset_x, tile_info->tileset_y);
  }
  printf("\n");
}

void tileset_destroy(struct Tileset* ts)
{
  if (!ts)
  {
    fprintf(stderr, "tileset_destroy error: ts is not a valid pointer\n");
    return;
  }

  if (!ts->tile_infos)
  {
    return;
  }

  free(ts->tile_infos);
  ts->tile_infos = 0;

  ts->tile_width = 0;
  ts->tile_height = 0;
  ts->num_tile_infos = 0;
  ts->shared_bitmap = 0;
}

void tilemap_init(
  struct Tilemap* tm,
  int num_columns,
  int num_rows,
  int num_layers)
{
  if (!tm)
  {
    fprintf(stderr, "tilemap_init error: tm is not a valid pointer\n");
    return;
  }

  if (num_columns <= 0)
  {
    fprintf(stderr, "tilemap_init error: columns must be greater than zero\n");
    return;
  }

  if (num_rows <= 0)
  {
    fprintf(stderr, "tilemap_init error: rows must be greater than zero\n");
    return;
  }

  if (num_layers <= 0)
  {
    fprintf(stderr, "tilemap_init error: layers must be greater than zero\n");
    return;
  }

  tm->num_columns = num_columns;
  tm->num_rows = num_rows;
  tm->num_layers = num_layers;

  int tilemap_layer_size = sizeof(struct TilemapLayer);

  tm->layers = (struct TilemapLayer*)malloc(num_layers * tilemap_layer_size);

  int num_tiles_per_layer = num_columns * num_rows;

  int tile_data_size = sizeof(tile_data_type);

  for (int i = 0; i < num_layers; i++)
  {
    struct TilemapLayer* layer = &tm->layers[i];

    layer->tile_data = (tile_data_type*)malloc(num_tiles_per_layer * tile_data_size);

    memset(layer->tile_data, 1, tile_data_size * num_tiles_per_layer);

    layer->scroll_x = 0;
    layer->scroll_y = 0;
    layer->tileset_index = 0;
  }
}

void tilemap_layer_init(
  struct Tilemap* tm,
  int layer_index,
  tile_data_type* layer_data,
  int tileset_index,
  int scroll_x,
  int scroll_y)
{
  if (!tm)
  {
    fprintf(stderr, "tilemap_layer_init error: tm is not a valid pointer\n");
    return;
  }

  if (!layer_data)
  {
    fprintf(stderr, "tilemap_layer_init error: layer data is not a valid pointer\n");
    return;
  }

  if (layer_index < 0 || layer_index >= tm->num_layers)
  {
    layer_index = 0;
  }

  struct TilemapLayer* layer = &tm->layers[layer_index];

  layer->scroll_x = scroll_x;
  layer->scroll_y = scroll_y;
  layer->tileset_index = tileset_index;

  int tile_data_size = sizeof(tile_data_type);
  int num_tiles_per_layer = tm->num_columns * tm->num_rows;
  memcpy(layer->tile_data, layer_data, tile_data_size * num_tiles_per_layer);
}

void tilemap_destroy(struct Tilemap* tm)
{
  if (!tm)
  {
    fprintf(stderr, "tilemap_destroy error: tm is not a valid pointer\n");
    return;
  }

  if (!tm->layers)
  {
    return;
  }

  for (int i = 0; i < tm->num_layers; i++)
  {
    struct TilemapLayer* layer = &tm->layers[i];
    if (layer->tile_data)
    {
      free(layer->tile_data);
    }
  }

  free(tm->layers);
  tm->layers = 0;
  tm->num_layers = 0;
  tm->num_columns = 0;
  tm->num_rows = 0;
}

void tilemap_render(
  struct Tileset* tilesets,
  struct Tilemap* tm,
  int offset_x,
  int offset_y)
{
  int num_tiles = tm->num_columns * tm->num_rows;
  for (int layer_index = 0; layer_index < tm->num_layers; layer_index++)
  {
    struct TilemapLayer* layer = &tm->layers[layer_index];
    struct Tileset* ts = &tilesets[layer->tileset_index];
    ALLEGRO_BITMAP* tile_bitmap = ts->shared_bitmap;

    for (int tile_index = 0; tile_index < num_tiles; tile_index++)
    {
      int column = tile_index % tm->num_columns;
      int row = tile_index / tm->num_columns;

      tile_data_type tile = layer->tile_data[tile_index];

      if (!tile)
      {
        continue;
      }

      struct TileInfo* tile_info = &ts->tile_infos[tile - 1];

      int screen_x = layer->scroll_x + (column * ts->tile_width);
      int screen_y = layer->scroll_y + (row * ts->tile_height);

      int tile_x = tile_info->tileset_x;
      int tile_y = tile_info->tileset_y;

      al_draw_bitmap_region(
        tile_bitmap,
        tile_x,
        tile_y,
        ts->tile_width,
        ts->tile_height,
        screen_x + offset_x,
        screen_y + offset_y,
        0
      );
    }
  }
}
