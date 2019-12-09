import numpy as np
from CubeState import*
import time


scramble  =  "R' U' F L2 F2 R2 D L2 D' F2 D2 U L' F' L2 B L' B U L' R' U'"

scramble_state = CubeState((np.array([5, 1, 2, 3, 0, 4, 6, 7]),
                np.array([1, 1, 1, 0, 2, 1, 0, 0]),
                np.array([5, 1, 2, 0, 3, 8, 6, 7, 4, 9, 10, 11]),
                np.array([1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0])))

print(scramble_state.I.isSolvable())
# print(scramble_state.I)
