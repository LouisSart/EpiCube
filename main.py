from core.CubeState import *
from core.Algorithm import *
import time



# Efface les fichiers
f=open("maneuvers/0/cubes.dat", "w")
f.close()
f=open("maneuvers/0/algorithms.dat", "w")
f.close()
nmax = 3
stored_states = [[] for i in range(nmax)]
stored_states[0] = [all_moves[i] for i in range(1,19)]
c = CubeState()

with open("maneuvers/0/cubes.dat", "a") as f_cubes:
    with open("maneuvers/0/algorithms.dat", "a") as f_algs:
        for k in range(1,nmax):
            for state in stored_states[k-1]:
                for move in all_moves:
                    c = state*all_moves[move]
                    stored_states[k].append(c)
                    c.tofile(f_cubes)




algo = Algorithm()
cu   = CubeState()
with open("maneuvers/0/algorithms.dat", "r") as f_algs:
    with open("maneuvers/0/cubes.dat", "r") as f_cubes:
        for i in range(18):
            # algo.fromfile(f_algs)
            cu.fromfile(f_cubes)
