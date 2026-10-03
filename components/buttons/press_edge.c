#include "press_edge.h"

bool button_press_edge( bool is_down , bool * was_down )
{
    bool pressed = is_down && !*was_down;
    *was_down = is_down;
    return pressed;
}
