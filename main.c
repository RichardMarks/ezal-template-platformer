#include <ezal.h>

EZAL_FN(my_create_fn);
EZAL_FN(my_destroy_fn);
EZAL_FN(my_update_fn);
EZAL_FN(my_render_fn);

int main(int argc, char** argv)
{
  struct EZALConfig cfg;

  ezal_use_config_defaults(&cfg);

  return ezal_start(
    "EZAL Empty Template",
    &my_create_fn,
    &my_destroy_fn,
    &my_update_fn,
    &my_render_fn,
    &cfg);
}

EZAL_FN(my_create_fn)
{
  // create things
}

EZAL_FN(my_destroy_fn)
{
  // destroy things
}

EZAL_FN(my_update_fn)
{
  // update things
  if (EZAL_KEY(ALLEGRO_KEY_ESCAPE))
  {
    ezal_stop(ctx);
  }
}

EZAL_FN(my_render_fn)
{
  // draw things
  al_draw_text(
    ctx->al_ctx->font,
    al_map_rgb(255, 255, 255),
    0,
    0,
    0,
    "Press ESC to Exit");
}
