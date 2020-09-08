#include "platformer.h"
#include "level_1.h"

#define platformer_ctx(ctx) ctx->user[PLATFORMER_USER_DATA_ID]
#define set_platformer_on_ctx(ctx, platformer) \
  platformer_ctx(ctx) = (void*)(platformer)
#define get_platformer_from_ctx(ctx) \
  struct Platformer* platformer = ((struct Platformer*)platformer_ctx(ctx))

void private_platformer_configure(struct EZALConfig* cfg);

void private_platformer_load_resources(struct Platformer* platformer);
void private_platformer_unload_resources(struct Platformer* platformer);

void private_platformer_initialize_tilesets(struct Platformer* platformer);
void private_platformer_destroy_tilesets(struct Platformer* platformer);

void private_platformer_initialize_tilemap(struct Platformer* platformer);
void private_platformer_destroy_tilemap(struct Platformer* platformer);

EZAL_FN(platformer_create_fn);
EZAL_FN(platformer_destroy_fn);
EZAL_FN(platformer_update_fn);
EZAL_FN(platformer_render_fn);

void platformer_initialize(struct Platformer* platformer)
{
  if (!platformer)
  {
    fprintf(stderr, "platformer is not a valid pointer\n");
    exit(EXIT_FAILURE);
  }

  private_platformer_configure(&platformer->cfg);

  platformer->rta = ezal_init(
    "EZAL Platformer Template",
    &platformer_create_fn,
    &platformer_destroy_fn,
    &platformer_update_fn,
    &platformer_render_fn,
    &platformer->cfg);

  set_platformer_on_ctx(platformer->rta->rt_ctx, platformer);

  platformer->rta->rt_ctx->al_ctx->screen_color = al_map_rgb(96, 96, 96);

  private_platformer_load_resources(platformer);
  private_platformer_initialize_tilesets(platformer);
  private_platformer_initialize_tilemap(platformer);
}

void private_platformer_configure(struct EZALConfig* cfg)
{
  ezal_use_config_defaults(cfg);

  cfg->logical_width = PLATFORMER_SCREEN_WIDTH;
  cfg->logical_height = PLATFORMER_SCREEN_HEIGHT;
  cfg->auto_scale = true;
  cfg->stretch_scale = false;
  // cfg->fullscreen = true;
  // cfg->debug = true;
}

void private_platformer_load_resources(struct Platformer* platformer)
{
  ALLEGRO_BITMAP* atlas_bitmap = al_load_bitmap("res/atlas.png");

  if (!atlas_bitmap)
  {
    fprintf(stderr, "unable to load res/atlas.png\n");
    exit(EXIT_FAILURE);
  }
  platformer->atlas_bitmap = atlas_bitmap;
}

void private_platformer_unload_resources(struct Platformer* platformer)
{
  if (platformer->atlas_bitmap)
  {
    al_destroy_bitmap(platformer->atlas_bitmap);
    platformer->atlas_bitmap = 0;
  }
}

void private_platformer_initialize_tilesets(struct Platformer* platformer)
{
  int tileset_size = sizeof(struct Tileset);

  platformer->num_tilesets = 1;

  platformer->tilesets = (struct Tileset*)malloc(
    platformer->num_tilesets * tileset_size);

  for (int i = 0; i < platformer->num_tilesets; i++)
  {
    tileset_init(
      &platformer->tilesets[i],
      PLATFORMER_TILE_WIDTH,
      PLATFORMER_TILE_HEIGHT,
      platformer->atlas_bitmap);
  }
}

void private_platformer_destroy_tilesets(struct Platformer* platformer)
{
  if (!platformer->tilesets)
  {
    return;
  }

  for (int i = 0; i < platformer->num_tilesets; i++)
  {
    tileset_destroy(&platformer->tilesets[0]);
  }

  free(platformer->tilesets);
  platformer->tilesets = 0;
}

void private_platformer_initialize_tilemap(struct Platformer* platformer)
{
  int tilemap_size = sizeof(struct Tilemap);

  platformer->tilemap = (struct Tilemap*)malloc(tilemap_size);

  tilemap_init(
    platformer->tilemap,
    PLATFORMER_TILEMAP_COLUMNS,
    PLATFORMER_TILEMAP_ROWS,
    PLATFORMER_TILEMAP_LAYERS);
}

void private_platformer_destroy_tilemap(struct Platformer* platformer)
{
  if (!platformer->tilemap)
  {
    return;
  }

  tilemap_destroy(platformer->tilemap);

  free(platformer->tilemap);
  platformer->tilemap = 0;
}

EZAL_FN(platformer_create_fn)
{
  get_platformer_from_ctx(ctx);

  tilemap_layer_init(platformer->tilemap, 0, level_1_layer_0, 0, 0, 0);
  tilemap_layer_init(platformer->tilemap, 1, level_1_layer_1, 0, 0, 0);
}

EZAL_FN(platformer_destroy_fn)
{
  get_platformer_from_ctx(ctx);

  private_platformer_destroy_tilemap(platformer);
  private_platformer_destroy_tilesets(platformer);
  private_platformer_unload_resources(platformer);
}

EZAL_FN(platformer_update_fn)
{
  get_platformer_from_ctx(ctx);

  if (EZAL_KEY(ALLEGRO_KEY_ESCAPE))
  {
    ezal_stop(ctx);
  }
}

EZAL_FN(platformer_render_fn)
{
  get_platformer_from_ctx(ctx);

  tilemap_render(platformer->tilesets, platformer->tilemap, 0, 0);

  al_draw_text(
    ctx->al_ctx->font,
    al_map_rgb(255, 255, 255),
    0,
    0,
    0,
    "Press ESC to Exit");
}
