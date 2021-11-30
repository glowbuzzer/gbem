/**
 ******************************************************************************
 * @file           :  plc.h
 * @brief          :  functions for plc standard library
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_PLC_STDLIB_H
#define GB_MONOREPO_PLC_STDLIB_H

#include "stdbool.h"
#include "inttypes.h"

/* EDGE DETECTION */
typedef struct {
    bool m;
} r_trig_private_t;

typedef struct {
    bool q;
    r_trig_private_t private;
} r_trig_t;

typedef struct {
    bool m;
} f_trig_private_t;

typedef struct {
    bool q;
    f_trig_private_t private;
} f_trig_t;

/*TIMERS*/
typedef struct {
    uint64_t start_count;
    int state; //0=reset, 1=counting, 2=set
    bool prev_in;
} tp_private_t;

typedef struct {
    uint32_t pt;
    bool q;
    uint32_t et;
    tp_private_t private;
} tp_t;


typedef struct {
    uint64_t start_count;
    int state; // 0=reset, 1=counting, 2=set
    bool prev_in;
} ton_private_t;

typedef struct {
    uint32_t pt;
    bool q;
    uint32_t et;
    ton_private_t private;
} ton_t;

typedef struct {
    uint64_t start_count;
    int state; // 0=reset, 1=counting, 2=set
    bool prev_in;
} tof_private_t;

typedef struct {
    uint32_t pt;
    bool q;
    uint32_t et;
    tof_private_t private;
} tof_t;


/* COUNTERS */

typedef struct {
    bool prev_cu;
} ctu_private_t;

typedef struct {
    bool q;
    uint32_t cv;
    uint32_t pv;
    ctu_private_t private;
} ctu_t;


typedef struct {
    bool prev_cd;
} ctd_private_t;

typedef struct {
    uint32_t pv;
    bool q;
    uint32_t cv;
    ctd_private_t private;
} ctd_t;


typedef struct {
    bool prev_cu;
    bool prev_cd;
} ctud_private_t;

typedef struct {
    uint32_t pv;
    bool qu;
    bool qd;
    uint32_t cv;
    ctud_private_t private;
} ctud_t;


/* PULSE GENERATORS */

typedef struct {
    bool init;
    uint64_t tx;
    uint64_t tn;
} clock_pulse_private_t;

typedef struct {
    int cnt;
    int pt;
    bool q;
    bool run;
    clock_pulse_private_t private;
} clock_pulse_t;



typedef struct {
    uint64_t cnt;
} clock_div_private_t;

typedef struct {
    bool q0;
    bool q1;
    bool q2;
    bool q3;
    bool q4;
    bool q5;
    bool q6;
    bool q7;
    clock_div_private_t private;
} clock_div_t;

/* INTEGRATE, DIFF, PID */

typedef struct {
    bool init;
    double a;
    double prev_val;
    uint64_t prev_t;
} integrate_private_t;

typedef struct {
    double k;
    double out_min;
    double out_max;
    double integral;
    bool lim;
    integrate_private_t private;
} integrate_t;


typedef struct {
    bool init;
    double prev_val;
    uint64_t prev_t;
} differentiate_private_t;

typedef struct {
    double k;
    double differential;
    differentiate_private_t private;
} differentiate_t;

typedef struct {

} sr_private_t;

typedef struct {
    bool q;
    sr_private_t private;
} sr_t;

typedef struct {

} rs_private_t;

typedef struct {
    bool q;
    rs_private_t private;
} rs_t;

/* MISC. */


typedef struct {

} hysteresis_private_t;

typedef struct {
    bool q;
    bool win;
    double on;
    double off;
    rs_private_t private;
} hysteresis_t ;


/* EDGE DETECTION */
void plcsl_f_trig(f_trig_t *f, bool clk);
void plcsl_r_trig(r_trig_t *r, bool clk);

/* COUNTERS */
void plcsl_ctu(ctu_t *c, bool cu, bool r);
void plcsl_ctd(ctd_t *c, bool cd, bool ld);
void plcsl_ctud(ctud_t *c, bool cu, bool cd, bool r, bool ld);

/* INTEGRATE, DIFF, PID */
void plcsl_integrate(integrate_t *i, bool en, double val, bool reset);
void plcsl_differentiate(differentiate_t *d, bool en, double val);

/* BISTABLE */
void plcsl_sr(sr_t sr, bool s, bool r);
void plcsl_rs(rs_t rs, bool s, bool r);

/* TIMERS */
void plcsl_tp(tp_t *t, bool in);
void plcsl_ton(ton_t *t, bool in);
void plcsl_tof(tof_t *t, bool in);

/* PULSE GENERATORS */
void plcsl_clock_pulse(clock_pulse_t *c, int n, bool r);
void plcsl_clock_div(clock_div_t *c, bool r, bool clk);


/*MISC */
void plcsl_hysteresis(hysteresis_t *h, double in);

#endif //GB_MONOREPO_PLC_STDLIB_H
