/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

#ifndef _osal_
#define _osal_


#ifdef __cplusplus
extern "C"
{
#endif

#include "osal_defs.h"
#include <stdint.h>
#include <time.h>

#define CFG_TICKS_PER_SECOND 	1000000
#define USECS_PER_SEC   		1000000
#define USECS_PER_TICK  (USECS_PER_SEC / CFG_TICKS_PER_SECOND)


struct gb_timeval {
	time_t		tv_sec;		/* seconds */
	suseconds_t	tv_usec;	/* and microseconds */
};



/* General types */
#define TRUE                1
#define FALSE               0

#define boolean uint8_t

#define uint8 uint8_t
#define uint16 uint16_t
#define uint32 uint32_t
#define uint64 uint64_t
#define int8 int8_t
#define int16 int16_t
#define int32 int32_t
#define int64 int64_t



typedef struct
{
    uint32 sec;     //< Seconds elapsed since the Epoch (Jan 1, 1970) 
    uint32 usec;    //< Microseconds elapsed since last second boundary 
} ec_timet;





typedef struct osal_timer
{
    ec_timet stop_time;
} osal_timert;

void osal_timer_start(osal_timert * self, uint32 timeout_us);
boolean osal_timer_is_expired(osal_timert * self);
int osal_usleep(uint32 usec);
ec_timet osal_current_time(void);
void osal_microsleep(ec_timet *tv);
int osal_gettimeofday(ec_timet *tv);

uint32_t us_ticker_read(void);




#ifdef __cplusplus
}
#endif

#endif

