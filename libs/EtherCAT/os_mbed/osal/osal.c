/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */


#include <osal.h>


#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

extern uint64_t global_tick;
extern uint32_t global_last_tick;
extern volatile unsigned int *DWT_CYCCNT;



#define  timercmp(a, b, CMP)                                \
  (((a)->sec == (b)->sec) ?                           \
   ((a)->usec CMP (b)->usec) :                        \
   ((a)->sec CMP (b)->sec))


#define  timeradd(a, b, result)                             \
  do{                                                       \
    (result)->sec = (a)->sec + (b)->sec;           \
    (result)->usec = (a)->usec + (b)->usec;        \
    if ((result)->usec >= 1000000)                       \
    {                                                       \
       ++(result)->sec;                                  \
       (result)->usec -= 1000000;                        \
    }                                                       \
  } while (0)




uint32_t us_ticker_read(void){
	return (*DWT_CYCCNT / (SystemCoreClock / 1000000));
}

#define NOP() asm volatile ("nop")

void wait_us (uint32_t delay){
//	const uint32_t start = us_ticker_read();
//	while ((us_ticker_read() - start) < (uint32_t) delay);
uint32_t tick_now;

	 uint32_t start = us_ticker_read();
	    if(delay){
	        uint32_t expiry = (start + delay);
	        if(start > expiry){ //overflow
	            while(us_ticker_read() > start || us_ticker_read() < expiry){
	                NOP();
	            }
	        } else{
	        while(1){
	        	tick_now = us_ticker_read();
	        	if (tick_now < expiry){
	        		break;
	        	}
//	        	volatile uint32_t dg_ticker_read =us_ticker_read();
//	        	volatile uint32_t dg_expiry = expiry;
//	        	volatile uint32_t dg_delay= delay;
	            NOP();
	        }
	        }
	    }


//		 uint32_t start = us_ticker_read();
//		    if(delay){
//		        uint32_t expiry = (start + delay);
//		        if(start > expiry){ //overflow
//		            while(us_ticker_read() > expiry){
//		                NOP();
//		            }
//		            return;
//		        }
//		        while(us_ticker_read() < expiry){
//		            NOP();
//		        }
//		    }
//



/*
 * us_ticker_read() - start) < (uint32_t) delay)
 * 999 - 998 = 2 < 5
 * 0 - 999 = -999
 */

}


int gettimeofday_(ec_timet *tp)
{

		//take a copy of the global ticks in a critical section
		taskENTER_CRITICAL();
		uint64_t copy_of_global_tick = global_tick;
		uint32_t copy_of_global_last_tick = global_last_tick;
		taskEXIT_CRITICAL();

		uint64_t updated_global_tick;
	    uint32_t actual_tick = us_ticker_read();
	    if (actual_tick>copy_of_global_last_tick){
	    	updated_global_tick = copy_of_global_tick + (actual_tick - copy_of_global_last_tick);
	    }
	    else{
	    	updated_global_tick = copy_of_global_tick + ((0xFFFFFFFF-copy_of_global_last_tick)+actual_tick);
	    }


	    uint64_t ticks_remainder;

	    tp->sec = updated_global_tick / CFG_TICKS_PER_SECOND;
	    ticks_remainder = updated_global_tick % CFG_TICKS_PER_SECOND;
	    tp->usec = ticks_remainder * USECS_PER_TICK;

	    return 0;
}

int osal_usleep (uint32 usec)
{
    wait_us(usec); 
    return 0;
}
//struct timezone *tz

int osal_gettimeofday(ec_timet *tv)
{
    return gettimeofday_(tv);
}

void osal_microsleep(ec_timet *tv){

	ec_timet now;
	uint64_t usec, sec;

	osal_gettimeofday(&now);


	/* Compute the difference  */
	usec = tv->usec - now.usec;
	sec = tv->sec - now.sec - (usec <0);

	if (sec < 0){
		 /* The time has already elapsed.  */
		return;
	}

//	now.sec = sec;
//	now.tv_nsec = usec + (usec < 0 ? 1000000 : 0);
//
//	tv = &now;
//
//	osal_gettimeofday(tv, tz)
//
//	uint64_t sleep_time = (tv->sec * 1000000 + tv->usec);
//
//	wait_us(sleep_time);
}


ec_timet osal_current_time (void)
{
	ec_timet current_time;
    ec_timet return_value;

    gettimeofday_ (&current_time);
    return_value.sec = current_time.sec;
    return_value.usec = current_time.usec;
    return return_value;
}



void osal_timer_start (osal_timert * self, uint32 timeout_usec)
{
    ec_timet start_time;
    ec_timet timeout;
    ec_timet stop_time;

    gettimeofday_ (&start_time);
    timeout.sec = timeout_usec / USECS_PER_SEC;
    timeout.usec = timeout_usec % USECS_PER_SEC;
    timeradd (&start_time, &timeout, &stop_time);

    self->stop_time.sec = stop_time.sec;
    self->stop_time.usec = stop_time.usec;
}


char* ullx(uint64_t val)
{
    static char buf[34] = { [0 ... 33] = 0 };
    char* out = &buf[33];
    uint64_t hval = val;
    unsigned int hbase = 16;

    do {
        *out = "0123456789abcdef"[hval % hbase];
        --out;
        hval /= hbase;
    } while(hval);

    *out-- = 'x', *out = '0';

    return out;
}


boolean osal_timer_is_expired (osal_timert * self)
{
	ec_timet current_time;
	ec_timet stop_time;
    uint64_t is_not_yet_expired;

    gettimeofday_ (&current_time);
    stop_time.sec = self->stop_time.sec;
    stop_time.usec = self->stop_time.usec;


    // timercmp returns true (nonzero) or false (0) depending on the result of the comparison.
    is_not_yet_expired = timercmp (&current_time, &stop_time, <);


volatile uint64_t inye = is_not_yet_expired;
volatile bool retval = (is_not_yet_expired == false);

//if (is_not_yet_expired == false){
//	printf("timer has expired\n");
//}



    return is_not_yet_expired == false;
}
