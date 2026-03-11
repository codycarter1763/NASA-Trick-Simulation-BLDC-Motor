/*************************************************************************
PURPOSE: (Initialize BLDC motor state from initial conditions)
**************************************************************************/
#include "../include/bldc.h"

int bldc_motor_init(BLDC_MOTOR* M) {
    /* Initial conditions copied into state variables */
    M->voltage = M->init_voltage;
    M->current = M->init_current; 
    M->omega = M->init_omega;
    M->theta = M->init_angle; 
    M->tau_load = M->init_tau_load;

    /* Compute derived quantites at t = 0 */
    M->back_emf = M->Ke * M->omega;
    M->torque = M->Kt * M->current;
    M->power = M->voltage * M->current;

    /* Reset event flags */
    M->stall = 0;
    M->stallTime = 0.0;
    M->time = 0.0;

    return 0;
}