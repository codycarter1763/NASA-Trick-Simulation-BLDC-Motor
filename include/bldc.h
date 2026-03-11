/*************************************************************************
PURPOSE: (Represent the state and initial conditions of a DC brushless motor)
**************************************************************************/
#ifndef BLDC_H
#define BLDC_H

typedef struct {
    /* Initial conditions*/
    double init_voltage; /* *i V    Initial applied voltage */
    double init_current; /* *i A    Initial current */
    double init_omega; /* *i rad/s  Initial angular velocity */
    double init_angle; /* *i rad    Initial rotor angle */
    double init_tau_load; /* *i N   Init load torque */

    /* Motor parameters */
    double R; /* *i Ω           Phase resistance */
    double L; /* *i kg.m/s2     Phase inductance */
    double Ke; /* *i V.s/rad    Back-EMF constant */
    double Kt; /* *i N.m/A      Torque constant */
    double J; /* *i kg.m2       Rotor inertia */
    double B; /* *i N.m.s/rad   Viscous damping */

    /* State variables */
    double current; /* A    Phase current */
    double omega; /* rad/s  Angular velocity */
    double theta; /* rad    Rotor angle */
    double torque; /* N     Electromagnetic torque */
    double back_emf; /* V   Back-EMF voltage */
    double voltage; /* V    Applied voltage */
    double tau_load; /* N   Load torque */
    double power; /* W      Electrical input power */
    double rpm; /* rpm      Revolutions per minute */

    double time; /* s       Model time */

    int stall; /* --        Has motor stalled? */
    double stallTime; /* s  Time of stall event */
} BLDC_MOTOR;

#ifdef __cplusplus
extern "C" {
#endif
    int bldc_motor_default_data(BLDC_MOTOR*);
    int bldc_motor_init(BLDC_MOTOR*);
    int bldc_motor_analytic(BLDC_MOTOR*);
    int bldc_motor_shutdown(BLDC_MOTOR*);
#ifdef __cplusplus
}
#endif

#endif
