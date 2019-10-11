import numpy as np
from CubeState import*
import time




scramble  =  "R' U' F L2 F2 R2 D' L2 D' F2 D2 U L' F' L2 B2 U2 B L' B U L' R' U' F"

scramble_state = CubeState(alg_str=scramble)
print(scramble_state)
