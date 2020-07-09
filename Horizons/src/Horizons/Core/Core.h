#pragma once

#define HZ_TICKS_PER_SECOND 60.0f
#define HZ_PHYS_VELOCITY_ITERATIONS 8
#define HZ_PHYS_POSITION_ITERATIONS 3

#define HZ_SLEEP_BETWEEN_TICKS

#ifdef HZ_DEBUG
#	define HZ_PHYSICS_DEBUG_DRAW
#	define HZ_CONSOLE_ENABLED
#endif

#define BIT(x) (1 << x)