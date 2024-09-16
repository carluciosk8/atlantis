#include "input.h"

#include "city.h"
#include "torpedo.h"


uint8_t last_trigger = 0;


void handle_input()
{
    uint8_t controller = ubox_read_ctl(UBOX_MSX_CTL_CURSOR);
    uint8_t trigger = controller & UBOX_MSX_CTL_FIRE1;
    if (trigger && !last_trigger && !torpedos[torpedo_current].is_active)
    {
        if      (controller & UBOX_MSX_CTL_LEFT)  torpedo_fire(TORPEDO_LEFT_PATT, TORPEDO_LEFT_X, TORPEDO_LEFT_Y, TORPEDO_LEFT_DX, TORPEDO_LEFT_DY);
        else if (controller & UBOX_MSX_CTL_RIGHT) torpedo_fire(TORPEDO_RIGHT_PATT, TORPEDO_RIGHT_X, TORPEDO_RIGHT_Y, TORPEDO_RIGHT_DX, TORPEDO_RIGHT_DY);
        else if (map_have_installation[0])        torpedo_fire(TORPEDO_CENTER_PATT, TORPEDO_CENTER_X, TORPEDO_CENTER_Y, TORPEDO_CENTER_DX, TORPEDO_CENTER_DY);
    }
    last_trigger = trigger;
}
