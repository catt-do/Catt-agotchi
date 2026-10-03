#pragma once
#include <stdbool.h>

// True only on the sample where a button goes from released to down.
// `was_down` carries the previous sample between calls.
bool button_press_edge( bool is_down , bool * was_down );
