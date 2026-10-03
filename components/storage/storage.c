#include "storage.h"
#include <assert.h>
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#define TAG       "Storage"
#define NAMESPACE "catt"
#define KEY       "stats"
#define KEY_DECAY "decay"

void storage_init( void )
{
    esp_err_t err = nvs_flash_init();
    if( err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND )
    {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
}

bool storage_load( catt_t * catt )
{
    assert( catt != NULL );

    nvs_handle_t handle;
    if( nvs_open( NAMESPACE , NVS_READONLY , &handle ) != ESP_OK ) return false;

    catt_t loaded;
    size_t size = sizeof( loaded );
    esp_err_t err = nvs_get_blob( handle , KEY , &loaded , &size );
    nvs_close( handle );

    if( err != ESP_OK || size != sizeof( loaded ) || !catt_valid( &loaded ) ) return false;

    *catt = loaded;
    return true;
}

bool storage_save( const catt_t * catt )
{
    assert( catt != NULL );

    nvs_handle_t handle;
    esp_err_t err = nvs_open( NAMESPACE , NVS_READWRITE , &handle );
    if( err != ESP_OK )
    {
        ESP_LOGE( TAG , "open failed: %s" , esp_err_to_name( err ) );
        return false;
    }

    err = nvs_set_blob( handle , KEY , catt , sizeof( *catt ) );
    if( err == ESP_OK ) err = nvs_commit( handle );
    nvs_close( handle );

    if( err != ESP_OK )
    {
        ESP_LOGE( TAG , "save failed: %s" , esp_err_to_name( err ) );
        return false;
    }
    return true;
}

bool storage_load_decay( uint32_t * elapsed_ms )
{
    assert( elapsed_ms != NULL );

    nvs_handle_t handle;
    if( nvs_open( NAMESPACE , NVS_READONLY , &handle ) != ESP_OK ) return false;

    uint32_t loaded;
    esp_err_t err = nvs_get_u32( handle , KEY_DECAY , &loaded );
    nvs_close( handle );

    if( err != ESP_OK ) return false;

    *elapsed_ms = loaded;
    return true;
}

bool storage_save_decay( uint32_t elapsed_ms )
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open( NAMESPACE , NVS_READWRITE , &handle );
    if( err != ESP_OK )
    {
        ESP_LOGE( TAG , "open failed: %s" , esp_err_to_name( err ) );
        return false;
    }

    err = nvs_set_u32( handle , KEY_DECAY , elapsed_ms );
    if( err == ESP_OK ) err = nvs_commit( handle );
    nvs_close( handle );

    if( err != ESP_OK )
    {
        ESP_LOGE( TAG , "save decay failed: %s" , esp_err_to_name( err ) );
        return false;
    }
    return true;
}
