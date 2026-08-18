#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t fullness ;
    uint8_t happiness;
    uint8_t wellness ;
} catt_t;

void catt_init( catt_t * catt );

void catt_stat_decrease( catt_t * catt );

void catt_give_milk( catt_t * catt );

void catt_give_heart( catt_t * catt );

void catt_give_meds( catt_t * catt );

bool catt_satisfied( catt_t * catt );

bool catt_alive( catt_t * catt );
