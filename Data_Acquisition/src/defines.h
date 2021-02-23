#pragma once
#ifndef __DEFINES_H
#define __DEFINES_H

/// @{

/* _____STANDARD INCLUDES____________________________________________________ */

/* _____PROJECT INCLUDES_____________________________________________________ */

/* _____DEFINITIONS _________________________________________________________ */
#ifndef NULL
/// NULL pointer
#define NULL 0
#endif

/* _____TYPE DEFINITIONS_____________________________________________________ */
// Provide standard types if absent from standard C library
typedef unsigned char   uint8_t;
typedef signed char     int8_t;
typedef unsigned short  uint16_t;
typedef signed short    int16_t;
typedef unsigned long   uint32_t;
typedef signed long     int32_t;

#define _Bool uint8_t

typedef uint8_t  u8_t;     ///< unsigned 8-bit value (0 to 255)
typedef int8_t   s8_t;     ///< signed 8-bit value (-128 to +127)
typedef uint16_t u16_t;    ///< unsigned 16-bit value (0 to 65535)
typedef int16_t  s16_t;    ///< signed 16-bit value (-32768 to 32767)
typedef uint32_t u32_t;    ///< unsigned 32-bit value (0 to 4294967296)
typedef int32_t  s32_t;    ///< signed 32-bit value (-2147483648 to +2147483647)
typedef float    float_t;  ///< 32 bits precision floating point number
typedef double   double_t; ///< 64 bits precision floating point number

#ifdef __cplusplus
	#define bool_t bool
	#ifndef FALSE
		#define FALSE	false
	#endif
	#ifndef TRUE
		#define TRUE	true
	#endif
#else
	#define bool_t _Bool
	#ifndef FALSE
		#define FALSE	0
	#endif
	#ifndef TRUE
		#define TRUE	(!FALSE)
	#endif
#endif

/* _____GLOBAL VARIABLES_____________________________________________________ */

/* _____GLOBAL FUNCTION DECLARATIONS_________________________________________ */

/* _____MACROS_______________________________________________________________ */
/// Set a bit (1)
#define BIT_SET_HI(var, bit)             do { (var) |= (1 << (bit)); } while (0)

/// Clear a bit (0)
#define BIT_SET_LO(var, bit)             do { (var) &= ~(1 << (bit)); } while (0)

/// Toggle a bit
#define BIT_TOGGLE(var, bit)             do { (var) ^= (1 << (bit)); } while (0)

/// Test if a bit is set (1?)
#define BIT_IS_HI(var, bit)              (((var)&(1 << (bit))) != 0)

/// Test if a bit is cleared (0?)
#define BIT_IS_LO(var, bit)              (((var)&(1 << (bit))) == 0)

/// Wait until a bit is set
#define LOOP_UNTIL_BIT_IS_HI(var, bit)   do { ; } while (BIT_IS_LO((var), (bit)))

/// Wait until a bit is cleared
#define LOOP_UNTIL_BIT_IS_LO(var, bit)   do { ; } while (BIT_IS_HI((var), (bit)))

/// Increments "var" by 1 and mask it using "mask"
#define INC_MASKED(var, mask)	((var) = ((var) + 1) & (mask))

/// @}


/// @name Byte extraction macros
//@{
/// Extract the high 8 bits of a 16-bit value (Most Significant Byte)
#define U16_HI8(data) ((u8_t)(((data) >> 8) & 0xff))

/// Extract the low 8 bits of a 16-bit value (Least Significant Byte)
#define U16_LO8(data) ((u8_t)((data) & 0xff))

/// Extract the high 8 bits (bits 31..24) of a 32-bit value
#define U32_HI8(data) ((u8_t)(((data) >> 24) & 0xff))

/// Extract the medium high 8 bits (bits 23..16) of a 32-bit value
#define U32_MH8(data) ((u8_t)(((data) >> 16) & 0xff))

/// Extract the medium low 8 bits (bits 15..8) of a 32-bit value
#define U32_ML8(data) ((u8_t)(((data) >> 8) & 0xff))

/// Extract the low 8 bits (bits 7..0) of a 32-bit value
#define U32_LO8(data) ((u8_t)((data) & 0xff))
//@}

#endif //__DEFINES_H
