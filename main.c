#include "platformer.h"

int main(int argc, char** argv)
{
  struct Platformer platformer;

  platformer_initialize(&platformer);

  return platformer.rta->start(platformer.rta);
}
