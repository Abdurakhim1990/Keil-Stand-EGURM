#ifndef INCL_DEBUG_H
#define INCL_DEBUG_H

#include "main.h"
#include "debug_init.h"

typedef enum {
	DEBUG_TX_SIZE			= 35,
	DEBUG_RX_SIZE			= 35,
} debug_size;

void DebugUart(char* ch);

#endif
