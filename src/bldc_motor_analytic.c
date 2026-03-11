/*************************************************************************
PURPOSE: (BLDC motor analytic update - called each scheduled timestep)
**************************************************************************/
#include "bldc/include/bldc.h"

int bldc_motor_analytic(BLDC_MOTOR* M) {

    double dt = 0.001;  /* s - must match scheduled rate in S_define */

    /* Back-EMF */
    M->back_emf = M->Ke * M->omega;

    /* Derivatives */
    double di_dt     = (M->voltage - M->R * M->current - M->back_emf) / M->L;
    double domega_dt = (M->Kt * M->current - M->B * M->omega - M->tau_load) / M->J;
    double dtheta_dt = M->omega;

    /* Euler integration */
    M->current += di_dt     * dt;
    M->omega   += domega_dt * dt;
    M->theta   += dtheta_dt * dt;

    /* Derived quantities */
    M->torque = M->Kt * M->current;
    M->power  = M->voltage * M->current;
    M->time  += dt;
    M->rpm = M->omega * (60.0 / (2.0 * 3.14));

    /* Stall detection */
    if (!M->stall && M->time > 0.1 && M->omega < 0.01 && M->tau_load > 0.0) {
        M->stall     = 1;
        M->stallTime = M->time;
    }

    return 0;
}
