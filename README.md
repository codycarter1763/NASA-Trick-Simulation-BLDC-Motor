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
Trick requires a specific setup to be able to simulate properly, for more specific information, view their documentation. Once built, more files will be populated here, but I only listed one files I had to add or edit.

```
SIM_BLDC_Motor/
├── S_define                              # Assigns jobs to Trick's scheduler
├── makefile                              # Build configuration (Made by Trick compilation command)
├── RUN_test/
│   └── input.py                          # Runtime parameters and data recording
├── Input Voltage Monte Carlo             # Holds different values to try (I used varying voltages for my testing)
├── S_overrides.mk                        # Sets path location for include and src files
├── BLDC_Display                          # Python code for recieving variables from Trick server using TCP
├── Modified_data/
│   └── realtime.py                       # Configuration for simulation control
└── models/bldc/
    ├── include/
    │   ├── bldc.h                        # Motor struct and function prototypes
    │   └── bldc_analytic.h               # Analytic model prototype
    └── src/
        ├── bldc_motor_default_data.c     # Sets motor parameters
        ├── bldc_motor_deriv.c            # Derives motor equations
        ├── bldc_motor_integ.c            # Intergrates motor equations
        ├── bldc_motor_init.c             # Initializes state from ICs
        ├── bldc_motor_analytic.c         # Physics equations + integration
        └── bldc_motor_shutdown.c         # Prints final results
```

# Results
Once built and opened, you can now view the results of different parameters of the brushless motor. Below are some examples of the types of plots you can expect to see.

## Single Variable Plots
Below shows single variable plots that demonstrate how brushless motors operate showing how as RPM increases, motor current goes down inversely with the increasing back EMF. This is the main reason brushless motors are super efficient and don't produce a ton of heat during operation.

### Motor Current
<img width="680" height="662" alt="Input Current Plot" src="https://github.com/user-attachments/assets/e355ef7d-ae43-4daa-93d6-b3267172dcef" />

### Motor Back_EMF
<img width="680" height="662" alt="Back_EMF Plot" src="https://github.com/user-attachments/assets/94f60ffe-328c-4fe9-a1c5-6970992ab436" />

## Monte Carlo Plot
### RPM as Voltage Increases
Viewing a Monte Carlo plot allows you to run the simulation multiple times with different parameters. To demonstrate this, I added an input file with different LiPO battery voltages from 2S (7.4V) to 6S (22.2V) to show the RPM increase as voltage increases.

<img width="1848" height="940" alt="RPM Ctasle Motor" src="https://github.com/user-attachments/assets/0f41a4d6-699f-4cef-ac3c-ed4590e83f26" />

### RPM as Load Increases
Below shows what happens to motor RPM as the load increases. The simulation was done with a 6S (22.2V) input and with a load range from 0.0 - 8.0 N.

<img width="1848" height="940" alt="RPM as Load Increases 6S" src="https://github.com/user-attachments/assets/bd8be55f-16e5-4c6e-b3c9-ac01477939ee" />

## Python TCP Display
With each Trick simulation, a TCP server is started to allow an external program to request and access data for external processing. For example, I included a Python script that will connect to the Trick port, request motor RPM and current, and then print the data out in the command line.

Below is what would show in the command line as an example.

| Message Type | RPM | Current (A) |
|---|---|---|
| 0 | 48,247 | 12.56 |
| 0 | 48,589 | 12.69 |
| 0 | 48,591 | 12.69 |
| 0 | 48,591 | 12.69 |
| 0 | 48,591 | 12.69 |

# Conclusion 
This repository shows my learning journey of NASA's Trick Simulation Environment by modeling a brushless DC motor. Using core electrical and mechanical equations of a brushless DC motor, including parameters of phase, back-EMF, torque contants, rotor inertia, and viscous damping, allows for a simulated physics model to be realized and analyzed.

This is just one of the many ways that Trick can be used to realize data and simulate physics. Try it out for yourself!
