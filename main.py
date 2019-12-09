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
stored_algs = [[] for i in range(nmax)]
stored_algs[0] = [Algorithm([i]) for i in range(1,19)]
c = CubeState()

for k in range(1,nmax):
    cube_path = "maneuvers/%i/cubes.dat"%k
    alg_path = "maneuvers/%i/algorithms.dat"%k
    with open(cube_path, "a+") as f_cubes:
        with open(alg_path, "a+") as f_algs:
            for state in stored_states[k-1]:
                for move in all_moves:
                    c = state*all_moves[move]
                    stored_states[k].append(c)
                    c.tofile(f_cubes)
                    


algo = Algorithm()
cu   = CubeState()
with open("maneuvers/1/algorithms.dat", "r") as f_algs:
    with open("maneuvers/1/cubes.dat", "r") as f_cubes:
        for i in range(5):
            # algo.fromfile(f_algs)
            cu.fromfile(f_cubes)
            print(cu)
