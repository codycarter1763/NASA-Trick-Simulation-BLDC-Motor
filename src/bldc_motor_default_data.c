/*************************************************************************
PURPOSE: (Set default data for the BLDC motor model)
**************************************************************************/
#include "../include/bldc.h"

int bldc_motor_default_data(BLDC_MOTOR* M) {
    /* Initial conditions*/
    M->init_voltage = 24; /* V      Nominal bus voltage */
    M->init_current = 0.0; /* A     Start from rest */
    M->init_omega = 0.0; /* rad/s   Start from rest */
    M->init_angle = 0.0; /* rad     Rotor at zero   */
    M->init_tau_load = 0.0; /* N    No startup load */

    /* Motor parameters (small BLDC) */
    M->R = 0.5; /* Ω             Phase resistance */
    M->L = 0.001; /* kg.m/s2     Phase inductance */
    M->Ke = 0.05; /* V.s/rad     Back-EMF constant */
    M->Kt = 0.05; /* N.m/A       Torque constant */
    M->J = 0.0001; /* kg.m2      Rotor inertia */
    M->B; /* N.m.s/rad           Viscous damping */

    return 0;
}