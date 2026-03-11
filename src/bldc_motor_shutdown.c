/*************************************************************************
PURPOSE: (Shutdown jobs for BLDC motor — print final state)
**************************************************************************/
#include <stdio.h>
#include "../include/bldc.h"

int bldc_motor_shutdown(BLDC_MOTOR* M) {

    printf("=== BLDC Motor Shutdown ===\n");
    printf("  Final time:     %.4f s\n",    M->time);
    printf("  Final speed:    %.4f rad/s (%.1f RPM)\n", M->omega, M->rpm);
    printf("  Final current:  %.4f A\n",    M->current);
    printf("  Final torque:   %.4f N\n",    M->torque);
    printf("  Final power:    %.4f W\n",    M->power);
    if (M->stall)
        printf("  ** Motor stalled at t=%.4f s **\n", M->stallTime);

    return 0;
}