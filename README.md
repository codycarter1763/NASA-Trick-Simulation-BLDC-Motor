# NASA Trick Simulation of BLDC Motor
<img width="1848" height="940" alt="RPM Ctasle Motor" src="https://github.com/user-attachments/assets/0f41a4d6-699f-4cef-ac3c-ed4590e83f26" />

# About
This repository shows my learning journey of NASA's Trick Simulation Environment by modeling a brushless DC motor - specifically a Castle Creations 2200kV brushless motor from my RC car. Mainly used a way for me to learn how to use Trick to model hardware, I chose that motor to be able to have a baseline for how close the simulation could match the real motor.

Using core electrical and mechanical equations of a brushless DC motor, including parameters of phase, back-EMF, torque contants, rotor inertia, and viscous damping, allows for a simulated physics model to be realized and analyzed.

This repository will show:
- Setup process of the simulation
- Implementation of electrical and mechanical motor equations
- File format and setup
- Examples of plots including single variables and Monte Carlo to show motor performance with different LiPO batteries

# What is Trick
<img width="1899" height="621" alt="image" src="https://github.com/user-attachments/assets/ab2f4180-cadc-4fb6-9af9-0095222f9737" />

The Trick Simulation Environment is a physics framework developed at the NASA Johnson Space Center and is used to develop and design space vehicles. Trick provides tools for quickly simulating designs and allows for realtime synchronization, job scheduling, checkpoints, data recording and logging, and a input processor. Essentially, Trick gives you the tools needed so engineers can quickly model and simulate their designs without having to create a simulation from scratch for each design aspect. 

For NASA, this allowed them to apply this framework to numerous projects, including the Systems Engineering Simulator and Virtual Reality lab for training astronauts.

Read up on more information about Trick [here](https://github.com/nasa/trick?tab=readme-ov-file).
# Setting Up
If you are starting from scratch, NASA has very well written documentation about the [install steps](https://nasa.github.io/trick/documentation/install_guide/Install-Guide).

I highly recommend you complete the [cannonball tutorial](https://nasa.github.io/trick/tutorial/Tutorial) before proceeding to get comfortable with the basics of how Trick is setup and ran. 

# Physics Implementation
To model the brushless motor, the simulation is implemented in steps, where each are updated every timestep.

## Electrical
First, voltage and current is applied to the motor and induces torque to start turning the rotor. As the rotor spins, back-EMF is induced in the motor windings and opposes the applied voltage to turn the motor. According to Lenz's law, the induced voltage will reduce the net voltage and current across the windings, and act as a natural regulator that prevents overheating and efficient operation. 

$$e = K_e \cdot \omega$$

$$\frac{di}{dt} = \frac{V - R \cdot i - e}{L}$$

| Symbol | Variable | Description | Unit | 
|---|---|---|---|
| $V$ | `voltage` | Applied voltage | V | 
| $R$ | `R` | Phase resistance | Ω | 
| $L$ | `L` | Phase inductance | H | 
| $K_e$ | `Ke` | Back-EMF constant | V·s/rad | 
| $i$ | `current` | Phase current | A | 
| $e$ | `back_emf` | Back-EMF voltage | V | 

## Mechanical 
To show the speed of the rotor as a result of the induced electrical parameters, torque with frictional opposition is modeled. 

$$\tau_{em} = K_t \cdot i$$

$$\frac{d\omega}{dt} = \frac{\tau_{em} - B \cdot \omega - \tau_{load}}{J}$$

| Symbol | Variable | Description | Unit | 
|---|---|---|---|
| $K_t$ | `Kt` | Torque constant | N·m/A | 
| $B$ | `B` | Viscous damping | N·m·s/rad | 
| $J$ | `J` | Rotor inertia | kg·m² | 
| $\tau_{em}$ | `torque` | Electromagnetic torque | N·m | 
| $\tau_{load}$ | `tau_load` | Load torque | N·m | 
| $\omega$ | `omega` | Angular velocity | rad/s |

## Kinematic 
To view where the rotor is, intergrating angular velocity will show the position.

$$\frac{d\theta}{dt} = \omega$$

| Symbol | Variable | Description | Unit |
|---|---|---|---|
| $\theta$ | `theta` | Rotor angle | rad |
| $\omega$ | `omega` | Angular velocity | rad/s | 

## Intergrating Using Euler
Each timestep, Trick advances the three state variables forward.

$$i(t + \Delta t) = i(t) + \frac{di}{dt} \cdot \Delta t$$

$$\omega(t + \Delta t) = \omega(t) + \frac{d\omega}{dt} \cdot \Delta t$$

$$\theta(t + \Delta t) = \theta(t) + \omega \cdot \Delta t$$

| Symbol | Variable | Description | Unit |
|---|---|---|---|
| $i(t + \Delta t)$ | `current` | Current at next timestep | A |
| $\omega(t + \Delta t)$ | `omega` | Angular velocity at next timestep | rad/s |
| $\theta(t + \Delta t)$ | `theta` | Rotor angle at next timestep | rad |
| $\Delta t$ | `dt` | Timestep size | s |
| $\frac{di}{dt}$ | `di_dt` | Current derivative | A/s |
| $\frac{d\omega}{dt}$ | `domega_dt` | Angular acceleration | rad/s² |
| $\frac{d\theta}{dt}$ | `dtheta_dt` | Angular velocity | rad/s |
| Method | — | Euler integration | — |

## Derived Quantities

After integration, power and RPM are computed from the updated time state.

$$P = V \cdot i$$

$$RPM = \omega \times \frac{60}{2\pi}$$

| Symbol | Variable in Code | Description | Unit | 
|---|---|---|---|
| $P$ | `power` | Electrical input power | W | 
| $RPM$ | `rpm` | Rotor speed | RPM | 
| $t$ | `time` | Simulation time | s | 

# Code Implementation
Moving those equation to code, Trick provides the implementations to intergrate and derive equations provided you set them up properly. For the sake of brevity, the exact implementation can be viewed in the included dependencies in the **SIM_BLDC_Motor / models / include + src** directories.

## General File Setup
Trick requires a specific setup to be able to simulate properly, for more specific information, view their documentation. 

```
SIM_BLDC_Motor/
├── S_define                  # Assigns jobs to Trick's scheduler
├── makefile                  # Build configuration (Made by Trick compilation command)
├── RUN_test/
│   └── input.py              # Runtime parameters and data recording
├── Input Voltage Monte Carlo # Holds different values to try (I used voltages for my testing)
└── models/bldc/
    ├── include/
    │   ├── bldc.h            # Motor struct and function prototypes
    │   └── bldc_analytic.h   # Analytic model prototype
    └── src/
        ├── bldc_motor_default_data.c   # Sets motor parameters
        ├── bldc_motor_init.c           # Initializes state from ICs
        ├── bldc_motor_analytic.c       # Physics equations + integration
        └── bldc_motor_shutdown.c       # Prints final results
```

# Results
# Conclusion 
