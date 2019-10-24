import sys
import os
sys.path.append('/home/louis/EpiCube/core')
import numpy as np
from CubeState import*
import time

cube = CubeState()
start = time.time()
alg_counter = 0
move_counter = 0
with open("/home/louis/EpiCube/test/scrambles.dat", "r") as scramble_file:
    for scramble in scramble_file:
        scramble=scramble[:-1]
        cube, k = cube.applyAlg(scramble)
        alg_counter+=1
        move_counter += k

end = time.time()

total_time = end - start
print("Nombre d'algos lancés : %i"%alg_counter)
print("*"*30)
print("Nombre de moves appliqués : %i"%move_counter)
print("*"*30)
print("Total time :", total_time)
print("*"*30)
print("Average time per alg : %.3e"%(total_time/alg_counter))
print("*"*30)
print("Average time per move : %.3e"%(total_time/move_counter))
