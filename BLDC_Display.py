#!/usr/bin/python3
import sys
import socket

# Process command line arguments
if (len(sys.argv) == 2):
    trick_varserver_port = int(sys.argv[1])
else:
    print( "Usage: python<version_number> CannonDisplay_Rev1.py <port_number>")
    sys.exit()

# Connecting to BLDC variable Trick server
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(("localhost", trick_varserver_port))
insock = client_socket.makefile("r")

# Request variable
client_socket.send(b"trick.var_pause()\n")
client_socket.send(b"trick.var_ascii()\n")
client_socket.send(b"trick.var_add(\"bldc_sim.motor.rpm\") \n" + 
                   b"trick.var_add(\"bldc_sim.motor.current\") \n" 
)
client_socket.send(b"trick.var_unpause()\n")

# Read the input socket and print the results
while(True):
    line = insock.readline()
    if line == '':
        break

    print(line)
                   
