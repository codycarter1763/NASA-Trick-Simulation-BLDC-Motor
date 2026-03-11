/*************************************************************************
PURPOSE: (Compute BLDC motor derivatives for Trick integration)
**************************************************************************/
#include "../include/bldc.h"
#include "/home/cody/trick/include/trick/integrator_c_intf.h"

int bldc_motor_deriv(BLDC_MOTOR* M) {
    /* Back EMF from motor speed */
    M->back_emf = M->Ke * M->omega;

    /* V = Ri + L(di/dt) + e -> di/dt = (V - Ri - e) / L */
    double di_dt = (M->voltage - M->R * M->current - M->back_emf) / M->L;

    /* Electromagnetic torque */
    M->torque = M->Kt * M->current;

    /* Mechanical: J(dω/dt) = τ_em - Bω - τ_load */
    double domega_dt = (M->torque - M->B * M->omega - M->tau_load) / M->J;

    /* Rotor kinematics */
    double dtheta_dt = M->omega;

    /* Electrical Power */
    M->power = M->voltage * M->current;

    /* Load derivatives into Trick intergrator */
    load_deriv(&di_dt, &domega_dt, &dtheta_dt, NULL);

    return 0;
}