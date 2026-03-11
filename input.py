#=========================================================
# Trick input file for BLDC motor simulation
#=========================================================
exec(open("Modified_data/bldc.dr").read())
exec(open("Modified_data/realtime.py").read())

# Enable Monte Carlo.
# trick.mc_set_enabled(1)
# trick.mc_set_num_runs(8)

# Castle Creations 2200Kv Motor
bldc_sim.motor.init_voltage  = 22.2         # V
bldc_sim.motor.init_tau_load = 0.05         # N  (small load)
bldc_sim.motor.R             = 0.010        # Ω
bldc_sim.motor.L             = 0.000005     # kg.m/s2
bldc_sim.motor.Ke            = 0.00434      # V.s/rad
bldc_sim.motor.Kt            = 0.00434      # N.m/A
bldc_sim.motor.J             = 0.00004      # kg.m2
bldc_sim.motor.B             = 0.000001     # N.m.s/rad

# mcvar_voltage = trick.MonteVarFile("bldc_sim.motor.init_voltage", "Input Voltage Monte Carlo", 1, "V") 
# mcvar_load =  trick.MonteVarFile("bldc_sim.motor.init_tau_load", "Load Monte Carlo", 1, "N") 
# Data recording
dr = trick.DRBinary("bldc_data")
dr.freq = trick.DR_Always
dr.add_variable("bldc_sim.motor.time")
dr.add_variable("bldc_sim.motor.current")
dr.add_variable("bldc_sim.motor.omega")
dr.add_variable("bldc_sim.motor.theta")
dr.add_variable("bldc_sim.motor.torque")
dr.add_variable("bldc_sim.motor.back_emf")
dr.add_variable("bldc_sim.motor.power")
dr.add_variable("bldc_sim.motor.rpm")

# trick.mc_add_variable(mcvar_voltage)
# trick.mc_add_variable(mcvar_load)

trick.add_data_record_group(dr)

# Run for # seconds
trick.stop(5)