from core.CubeState import *
from core.Algorithm import *
import time

scramble = "D2 F2 U' F2 U R2 D' L2 U F2 D F D2 L B2 U2 R B' D U F2"
alg = Algorithm(scramble)
c = CubeState()
c = c.applyAlg(alg)

print(c)

with open("cube.dat", "a") as f:
    c.tofile(f)

a = CubeState()
with open("cube.dat", "r") as f:
    a.fromfile(f)

print(a)
