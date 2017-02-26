#include "VGA.h"

void ScreenSetup()
{
  VGA.begin(VGAWISHBONESLOT(9),CHARMAPWISHBONESLOT(10));
  VGA.clear();
  VGA.setBackgroundColor(BLACK);
}