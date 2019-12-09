from core.CubeState import *
from core.Algorithm import *
import time

# scramble = "D2 F2 U' F2 U R2 D' L2 U F2 D F D2 L B2 U2 R B' D U F2"
# alg = Algorithm(str=scramble)
# c = CubeState()
# c = c.applyAlg(alg)
# print(alg)
# print(c)

# Efface les fichiers
f=open("maneuvers/0/cubes.dat", "w")
f.close()
f=open("maneuvers/0/algorithms.dat", "w")
f.close()

with open("maneuvers/0/cubes.dat", "a") as f_cubes:
    with open("maneuvers/0/algorithms.dat", "a") as f_algs:
        for move in all_moves:
            move_alg = Algorithm(move_int_to_str[move])
            move_alg.tofile(f_algs)
            all_moves[move].tofile(f_cubes)

algo = Algorithm()
cu   = CubeState()
with open("maneuvers/0/algorithms.dat", "r") as f_algs:
    with open("maneuvers/0/cubes.dat", "r") as f_cubes:
        for i in range(18):
            algo.fromfile(f_algs)
            cu.fromfile(f_cubes)
            print(algo)
            print(cu)
