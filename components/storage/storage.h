#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "catt.h"

void storage_init( void );

// False if nothing usable is stored (first boot, missing, wrong size or out-of-range stats)
bool storage_load( catt_t * catt );

bool storage_save( const catt_t * catt );

// How far into the current decay interval the Catt was, so a reboot doesn't restart it
bool storage_load_decay( uint32_t * elapsed_ms );

bool storage_save_decay( uint32_t elapsed_ms );
