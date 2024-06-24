
/**
 ******************************************************************************
 * @file           :  plc.c
 * @brief          :  functions for plc standard library
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "plc_stdlib.h"
#include "plc_core.h"
#include "std_defs_and_macros.h"
#include "log.h"
#include "ethercatsetget.h"
#include "map.h"
#include <math.h>

/**
 * @brief calculates the cartesian change in position in a cycle (aka velocity)
 * @param drive_num_x
 * @param drive_num_y
 * @param drive_num_z
 * @param num_axis - number of axes that participate in the cartesian move (either 1, 2 or 3)
 * @param cycle_time_ms - the period in ms that this function will be called at
 * @param grc gbc error code
 * @return float velocity
 *
 * the units of the velocity are "steps / ms" or steps being whatever the base unit of actpos for the drive is
 *
 */
float plcsl_calc_cartesian_velocity(uint8_t drive_num_x, uint8_t drive_num_y, uint8_t drive_num_z, uint8_t num_axis,
                                    uint8_t cycle_time_ms, gberror_t *grc) {
    static int32_t last_pos_x = 0;
    static int32_t last_pos_y = 0;
    static int32_t last_pos_z = 0;

    int32_t current_pos_x = 0;
    int32_t current_pos_y = 0;
    int32_t current_pos_z = 0;

    if (num_axis != 1 && num_axis != 2 && num_axis != 3) {
        *grc = E_INVALID_PARAMETER;
        return 0;
    }

    if (map_drive_get_actpos_wrd_function_ptr[drive_num_x] != NULL) {
        current_pos_x = map_drive_get_actpos_wrd_function_ptr[drive_num_x](drive_num_x);
    } else {
        LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                 "GBEM: Missing function pointer for map_drive_get_actpos_wrd on drive [%u]", drive_num_x);
    }


    if (num_axis == 2 || num_axis == 3) {
        if (map_drive_get_actpos_wrd_function_ptr[drive_num_y] != NULL) {
            current_pos_y = map_drive_get_actpos_wrd_function_ptr[drive_num_y](drive_num_y);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                     "GBEM: Missing function pointer for map_drive_get_actpos_wrd on drive [%u]", drive_num_y);
        }
    }
    if (num_axis == 3) {
        if (map_drive_get_actpos_wrd_function_ptr[drive_num_z] != NULL) {
            current_pos_z = map_drive_get_actpos_wrd_function_ptr[drive_num_z](drive_num_z);
        } else {
            LL_ERROR(GBEM_MISSING_FUN_LOG_EN,
                     "GBEM: Missing function pointer for map_drive_get_actpos_wrd on drive [%u]", drive_num_z);
        }
    }

    float cart_velo;
    cart_velo = ((float) sqrt(pow(((float) current_pos_x - (float) last_pos_x), 2) +
                              pow(((float) current_pos_y - (float) last_pos_y), 2) +
                              +pow(((float) current_pos_z - (float) last_pos_z), 2)));

    //here we scale the velocity by the cycle time
    cart_velo *= (float) cycle_time_ms;

    //store current pos as last pos in a static
    last_pos_x = current_pos_x;
    last_pos_y = current_pos_y;
    last_pos_z = current_pos_z;

    *grc = E_SUCCESS;

    return cart_velo;
}



/* PULSE GENERATORS */

/**
 * @brief Generates a clock with programmable period time.
 * @param c plcsl_clock_pulse struct
 * @param n if n is 0 clock is continuous and for n pulses otherwise
 * @param r reset
 */
void plcsl_clock_pulse(clock_pulse_t *c, int n, bool r) {

    c->private.tx = plc_ms_counter;
    c->q = false;
    c->run = c->cnt < n;

    if (!c->private.init || r) {
        c->private.init = true;
        c->cnt = 0;
        c->private.tn = c->private.tx - ((uint64_t) c->pt * 1);
        c->run = false;

    } else if ((c->cnt < n || n == 0) && (c->private.tx - c->private.tn >= ((uint64_t) c->pt * 1))) {
        c->cnt++;
        c->q = true;
        c->private.tn = c->private.tn + ((uint64_t) c->pt * 1);
    }
}

/**
 * @brief divides a clock
 * @param c
 * @param r
 * @param clk
 */
void plcsl_clock_div(clock_div_t *c, bool r, bool clk) {

    if (r) {
        c->private.cnt = 0;
        c->q0 = 0;
        c->q1 = 0;
        c->q2 = 0;
        c->q4 = 0;
        c->q5 = 0;
        c->q6 = 0;
        c->q7 = 0;
    } else if (clk) {
        c->private.cnt++;
        c->q0 = BIT_CHECK(c->private.cnt, 0);
        c->q1 = BIT_CHECK(c->private.cnt, 1);
        c->q2 = BIT_CHECK(c->private.cnt, 2);
        c->q3 = BIT_CHECK(c->private.cnt, 3);
        c->q4 = BIT_CHECK(c->private.cnt, 4);
        c->q5 = BIT_CHECK(c->private.cnt, 5);
        c->q6 = BIT_CHECK(c->private.cnt, 6);
        c->q7 = BIT_CHECK(c->private.cnt, 7);

    }

}

/* COUNTERS */

/**
 * @brief Up counter - sets output q after count has reached pv
 * @param c plcsl_ctu struct
 * @param c rising edge increments count
 * @param r reset
 *
 * in ctu_t struct bool q (output), uint32_t pv (pre-load value) uint32_t cv (counter value)
 */
void plcsl_ctu(ctu_t *c, bool cu, bool r) {
    if (r) {
        c->cv = 0;
    } else if ((cu && !c->private.prev_cu) && (c->cv < c->pv)) {
        c->cv++;
    }
    c->q = (c->cv >= c->pv);
    c->private.prev_cu = cu;
}

/**
 * @brief Down counter
 * @param c plcsl_ctd struct
 * @param cd rising edge decrements count
 * @param ld load value
 *
 * in ctd_t struct bool q (output), uint32_t pv (pre-load value) uint32_t cv (counter value)
 */
void plcsl_ctd(ctd_t *c, bool cd, bool ld) {
    if (ld) {
        c->cv = c->pv;
    } else if ((cd && c->private.prev_cd) && (c->cv > 0)) {
        c->cv--;
    }
    c->q = (c->cv <= 0);
}

/**
 * @brief count up and down
 * @param c ctud_t struct
 * @param cu rising edge increments counter
 * @param cd rising edge decrements counter
 * @param r reset
 * @param ld load value
 *
 * in ctud_t struct bool qu, qd (output), uint32_t pv (pre-load value) uint32_t cv (counter value)
 */
void plcsl_ctud(ctud_t *c, bool cu, bool cd, bool r, bool ld) {

    if (r) {
        c->cv = 0;
    } else if (ld) {
        c->cv = c->pv;
    } else {

        if (!((cu && c->private.prev_cu) && (cd && c->private.prev_cd))) {

            if ((cu && c->private.prev_cu) && (c->cv < c->pv)) {
                c->cv++;
            } else if ((cd && c->private.prev_cd) && (c->cv > 0)) {
                c->cv--;
            }
        }
    }
    c->qu = c->cv >= c->pv;
    c->qd = c->cv <= 0;

    c->private.prev_cu = cu;
    c->private.prev_cd = cd;
}

//@formatter:off
/****************************************************************

             TON - On-delay timing - state machine


            +--------+        +---+   +--------+
       in   |        |        |   |   |        |
          --+        +--------+   +---+        +-------------
            t0       t1       t2  t3  t4       t5
                 +---+                     +---+
       q         |   |                     |   |
          -------+   +---------------------+   +-------------
              t0+pt  t1                t4+pt   t5
         PT      +---+                     +---+
          :     /    |            +       /    |
       et :    /     |           /|      /     |
          :   /      |          / |     /      |
          :  /       |         /  |    /       |
          0-+        +--------+   +---+        +-------------
            t0       t1       t2  t3  t4       t5


        2        +---+                     +---+
state   1   +----+   |        +---+   +----+   |
        0 --+        +--------+   +---+        +------


****************************************************************/
//@formatter:on

void plcsl_ton(ton_t *t, bool in) {
    if (t->private.state == 0 && !t->private.prev_in && in) { //rising edge
        //start timer
        t->private.state = 1;
        t->q = false;
        t->private.start_count = plc_ms_counter;
    } else {
        //state is 1 or 2
        if (!in) {
            t->et = 0;
            t->q = false;
            t->private.state = 0;
        } else if (t->private.state == 1) {
            if (t->private.start_count + t->pt <= plc_ms_counter) {
                t->private.state = 2;
                t->q = true;
                t->et = t->pt;
            } else {
                t->et = plc_ms_counter - t->private.start_count;
            }
        }
    }

    t->private.prev_in = in;
}


//@formatter:off
/****************************************************************

             TOF - Off-delay timing - state machine


              +--------+        +---+   +--------+
       in     |        |        |   |   |        |
           ---+        +--------+   +---+        +-----------
              t0       t1       t2  t3  t4       t5
              +-------------+   +---------------------+
       q      |             |   |                     |
           ---+             +---+                     +------
              t0          t1+pt t2                  t5+pt
         pt                 +---+                     +------
          :                /    |       +            /
       et :               /     |      /|           /
          :              /      |     / |          /
          :             /       |    /  |         /
          0------------+        +---+   +--------+
                       t1           t3           t5


        2                   +---+                     +------
state   1              +----+   |   +---+        +----+
        0 -------------+        +---+   +--------+


****************************************************************/
//@formatter:on

void plcsl_tof(tof_t *t, bool in) {

    if (t->private.state == 0 && t->private.prev_in && !in) { //falling edge
        //start timer
        t->private.state = 1;
        t->private.start_count = plc_ms_counter;
    } else {
        //state is 1 or 2
        if (in) {
            t->et = 0;
            t->private.state = 0;
        } else if (t->private.state == 1) {

            if (t->private.start_count + t->pt <= plc_ms_counter) {

                t->private.state = 2;
                t->et = t->pt;
            } else {
                t->et = plc_ms_counter = t->private.start_count;
            }
        }
    }
    t->q = in || (t->private.state == 1);

    t->private.prev_in = in;
}


//@formatter:off
/****************************************************************
                  TP - pulse timing - state machine

            +--------+     ++ ++   +--------+
       in   |        |     || ||   |        |
          --+        +-----++-++---+        +---------
            t0       t1    t2 t3    t4       t5
            +----+         +----+  +----+
       q    |    |         |    |  |    |
          --+    +---------+    +--+    +-------------
            t0   t0+pt    t2 t2+pt t4  t4+pt
         pt      +---+          +       +---+
          :     /    |         /|      /    |
       et :    /     |        / |     /     |
          :   /      |       /  |    /      |
          :  /       |      /   |   /       |
          0-+        +-----+    +--+        +---------
            t0       t1    t2      t4       t5


        2        +---+          +       +---+
state   1   +----+   |     +----|  +----+   |
        0 --+        +-----+    +--+        +---------

****************************************************************/
//@formatter:on

void plcsl_tp(tp_t *t, bool in) {

    if (t->private.state == 0 && !t->private.prev_in && in) { //rising edge
//start timer
        t->private.state = 1;
        t->q = true;
        t->private.start_count = plc_ms_counter;
    } else if (t->private.state == 1) {

        if (t->private.start_count + (uint64_t) t->pt <= plc_ms_counter) {
            t->private.state = 2;
            t->q = false;
            t->et = t->pt;
        } else {
            t->et = plc_ms_counter - t->private.start_count;
        }
    }
    if (t->private.state == 2 && !in) {
        t->et = 0;
        t->private.state = 0;
    }

    t->private.prev_in = in;
}

/* EDGE DETECTION */

/**
 * @brief rising edge trigger
 * @param r plcsl_r_trig struct
 * @param clk signal
 *
 * in plcsl_r_trig bool q
 */
void plcsl_r_trig(r_trig_t *r, bool clk) {
    r->q = clk && !r->private.m;
    r->private.m = clk;
}

/**
 * @brief falling edge trigger
 * @param f plcsl_f_trig struct
 * @param clk signal
 */
void plcsl_f_trig(f_trig_t *f, bool clk) {
    f->q = !clk && !f->private.m;
    f->private.m = !clk;
}

/* SIGNAL PROCESSING - INTEGRATE, DIFF, PID */

/**
 * @brief
 * @param i pointer to integrate struct
 * @param en enable the integrator (starts accumulating samples)
 * @param val value to integrate
 * @param reset clear the integrator
 * @attention uses seconds
 */
void plcsl_integrate(integrate_t *i, bool en, double val, bool reset) {

    if (!i->private.init) {
        i->private.init = true;
        i->private.prev_val = val;
    } else if (en) {
        //trapezoidal
        i->private.a = i->private.a +
                       (i->k * 0.5E-3 * (val + i->private.prev_val) * (double) (plc_ms_counter - i->private.prev_t));

        if (i->private.a >= i->out_max) {
            i->integral = i->out_max;
            i->lim = true;
        } else if (i->private.a <= i->out_min) {
            i->integral = i->out_min;
            i->lim = true;
        } else {
            i->integral = i->private.a;
            i->lim = false;
        }
    }
    if (reset) {
        i->integral = 0;
        i->private.a = 0;
        i->lim = false;
        return;
    }

    i->private.prev_val = val;
    i->private.prev_t = plc_ms_counter;
}

/**
 * @brief differentiates a signal
 * @param d
 * @param en
 * @param val
 */
void plcsl_differentiate(differentiate_t *d, bool en, double val) {
    double tc = (double) (plc_ms_counter - d->private.prev_t);

    if (!d->private.init) {
        d->private.init = true;
        d->private.prev_val = val;
    } else if (en && tc > 0.0) {
        d->differential = (val - d->private.prev_val) / tc * 1000 * d->k;
        d->private.prev_val = val;
    } else {
        d->differential = 0.0;
    }
    d->private.prev_t = plc_ms_counter;
}



/* BISTABLE */

//@formatter:off
/* SR has ever been thus:

                  +-----+
S1----------------| >=1 |---Q1
         +---+    |     |
 R------O| & |----|     |
 Q1------|   |    |     |
         +---+    +-----+
*/
//@formatter:on

void plcsl_sr(sr_t sr, bool s, bool r) {
    sr.q = s || (!r && sr.q);
}

//@formatter:off
/* RS is:
                   +---+
R1----------------O| & |---Q1
        +-----+    |   |
S-------| >=1 |----|   |
Q1------|     |    |   |
        +-----+    +---+
*/
//@formatter:on

void plcsl_rs(rs_t rs, bool s, bool r) {
    rs.q = (!r) && (s || rs.q);
}

/* MISC. */

/**
 * @brief hysteresis function
 * @param hysteresis struct
 * @param in signal
 *
 * In struct     bool q (output), bool win (in is inside window), double on (on threshold), double off (off threshold)
 *
 * This Hysteresis function has two modes:
 * 1. if on > off then Q will be switched high when in > on and switched low when in < off.
 * 2. if on < off then Q will be switched high when in < on and switched low when in > off.
 * The output win will be high when in is between low and high.
 *
 */
void plcsl_hysteresis(hysteresis_t *h, double in) {

    if (h->on >= h->off) {
        if (in < h->off) {
            h->q = false;
            h->win = false;
        } else if (in > h->on) {
            h->q = true;
            h->win = false;
        } else {
            h->win = true;
        }
    } else {
        if (in > h->off) {
            h->q = false;
            h->win = false;
        } else if (in < h->on) {
            h->q = true;
            h->win = false;
        } else {
            h->win = true;
        }
    }

}
