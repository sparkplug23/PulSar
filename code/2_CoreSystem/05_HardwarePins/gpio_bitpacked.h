#ifndef _GPIO_BITPACKED_H_
#define _GPIO_BITPACKED_H_

#include <stdint.h>
#include <pgmspace.h>

#include "../mSystemConstants.h"
#include "gpio_enums.h"

/*********************************************************************************************\
 * GPIO selectable-function bit packing
 *
 * gpio_enums.h
 *   Static base GPIO IDs only, for example GPIO_REL, GPIO_REL_INV, GPIO_MODEM_RX.
 *
 * gpio_bitpacked.cpp
 *   Build-specific selectable GPIO groups.
 *
 * Stored/selected GPIO function IDs use:
 *
 *   packed_id = PGPIO(base_id) + index
 *
 * Examples:
 *
 *   PGPIO(GPIO_MODEM_RX) + 0  -> Modem RX
 *   PGPIO(GPIO_MODEM_RX) + 1  -> Modem RX2
 *   PGPIO(GPIO_MODEM_RX) + 2  -> Modem RX3
 *
 * Module-facing code must not manually pack IDs. Use:
 *
 *   tkr_pins->Pin(GPIO_MODEM_RX, 0);
 *   tkr_pins->Pin(GPIO_MODEM_RX, 1);
 *
 * SelectablePins_BitPacked[] stores selectable groups:
 *
 *   PGPIO(GPIO_MODEM_RX) + MGPIO(count)
 *
 * In group entries, the lower 5 bits store count-1.
 * In stored selected IDs, the lower 5 bits store the selected index.
 *
 * The count values must come from mSystemConstants.h where they are configurable/capacity
 * limits. Fixed hardware/bus widths may be written directly in gpio_bitpacked.cpp.
\*********************************************************************************************/

#define GPIO_INDEX_BITS  5
#define GPIO_INDEX_MASK  0x001F
#define GPIO_BASE_MASK   0xFFE0

#define PGPIO(x)         ((uint16_t)((x) << GPIO_INDEX_BITS))
#define UGPIO(x)         ((uint16_t)((x) >> GPIO_INDEX_BITS))
#define MGPIO(x)         ((uint16_t)((x) ? ((x) - 1) : 0))
#define PIGPIO(x,i)      ((uint16_t)(PGPIO(x) + ((uint16_t)(i) & GPIO_INDEX_MASK)))
#define PIGPIO_N(x,n)    PIGPIO(x, ((n) ? ((n) - 1u) : 0u))

/*********************************************************************************************\
 * Selectable packed GPIO list
 *
 * Defined once in gpio_bitpacked.cpp.
\*********************************************************************************************/

extern const uint16_t SelectablePins_BitPacked[] PROGMEM;
extern const uint16_t SelectablePins_BitPacked_Count;

#endif // _GPIO_BITPACKED_H_
