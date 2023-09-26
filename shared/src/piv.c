/**
 ******************************************************************************
 * @file           :  piv
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include <stdio.h>

struct PIV {
    double Kpp;
    double Kiv;
    double Kpv;
    double Kvff;
    double Kaff;
    double Kalp;
    double Ts;
    double YMin;
    double YMax;
    double Kt;
    double limitDiff;
    double velPrevStep;
    double velCommand;
    double velSmooth;
    int firstRun;
    int posController;
    int bSaturated;
    double posPrevStep;
    double velComputed;
    double posCommand;
};

void PIV_init(struct PIV *piv) {
    piv->Kpp = 0;
    piv->Kiv = 0;
    piv->Kpv = 0;
    piv->Kvff = 0;
    piv->Kaff = 0;
    piv->Kalp = 0;
    piv->Ts = 0;
    piv->YMin = 0;
    piv->YMax = 0;
    piv->Kt = 0;
    piv->limitDiff = 0.0;
    piv->velPrevStep = 0.0;
    piv->firstRun = 1;
    piv->posController = 1;
    piv->bSaturated = 0;
    piv->posPrevStep = 0.0;
    piv->velComputed = 0.0;
    piv->posCommand = 0.0;
}

void PIV_setGains(struct PIV *piv, double Kpp, double Kiv, double Kpv) {
    piv->Kpp = Kpp;
    piv->Kiv = Kiv;
    piv->Kpv = Kpv;
}

void PIV_setSamplingTime(struct PIV *piv, double Ts) {
    piv->Ts = Ts;
    // You may need to initialize any relevant members here.
}

double PIV_saturate_windup(struct PIV *piv, double val) {
    if (piv->YMax == piv->YMin) {
        piv->limitDiff = 0.0;
        return val;
    } else if (val > piv->YMax) {
        piv->limitDiff = piv->YMax - val;
        return piv->YMax;
    } else if (val < piv->YMin) {
        piv->limitDiff = piv->YMin - val;
        return piv->YMin;
    } else {
        piv->limitDiff = 0.0;
        return val;
    }
}

double PIV_saturate(struct PIV *piv, double val) {
    if (piv->YMax == piv->YMin) {
        piv->bSaturated = 0;
        return val;
    } else if (val > piv->YMax) {
        piv->bSaturated = 1;
        return piv->YMax;
    } else if (val < piv->YMin) {
        piv->bSaturated = 1;
        return piv->YMin;
    } else {
        piv->bSaturated = 0;
        return val;
    }
}

double PIV_updateVelLoop(struct PIV *piv, double velMeasured, double velCmd, double posCommand, double accFF) {
    piv->velSmooth = (1 - piv->Kalp) * velMeasured + piv->Kalp * piv->velPrevStep;
    piv->velPrevStep = piv->velSmooth;
    piv->velCommand = (piv->Kvff * velCmd) + posCommand - piv->velSmooth;
    piv->velCommand = PIV_saturate_windup(piv, (piv->Kpv * piv->velCommand));
    piv->velCommand = PIV_saturate(piv, piv->velCommand + (piv->Kaff * accFF));
    return piv->velCommand;
}

double PIV_update(struct PIV *piv, double posMeasured, double posRef, double velFF, double accFF) {
    if (piv->firstRun) {
        piv->posPrevStep = posMeasured;
        piv->velPrevStep = 0.0;
        piv->firstRun = 0;
    }

    piv->velComputed = (posMeasured - piv->posPrevStep) / piv->Ts;
//    if (piv->posController)
//        piv->posCommand = PIV_updatePosLoop(piv, posRef - posMeasured);
//    else
//        piv->posCommand = 0.0;

    piv->posPrevStep = posMeasured;
    return PIV_updateVelLoop(piv, piv->velComputed, velFF, piv->posCommand, accFF);
}

void PIV_reset(struct PIV *piv) {
    piv->limitDiff = 0.0;
    piv->velPrevStep = 0.0;
    // Initialize other members here as needed.
    piv->firstRun = 1;
    piv->bSaturated = 0;
}

void PIV_printCoefficients(struct PIV *piv) {
    printf("PIV Coefficients :\n");
    printf(" Kpp : %lf\n", piv->Kpp);
    printf(" Kiv : %lf\n", piv->Kiv);
    printf(" Kpv : %lf\n", piv->Kpv);
    printf(" Kvff : %lf\n", piv->Kvff);
    printf(" Kaff : %lf\n", piv->Kaff);
    printf(" Kalp : %lf\n", piv->Kalp);
    printf(" Kt : %lf\n", piv->Kt);
    printf(" Ts : %lf\n", piv->Ts);
    printf(" YMax : %lf\n", piv->YMax);
    printf(" YMin : %lf\n", piv->YMin);
}

//int main() {
//    struct PIV piv;
//    PIV_init(&piv);
//
//    // Example usage:
//    PIV_setGains(&piv, 1.0, 2.0, 3.0);
//    PIV_setSamplingTime(&piv, 0.1);
//    PIV_printCoefficients(&piv);
//
//    return 0;
//}
