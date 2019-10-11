import numpy as np

CP0 = np.array([0, 1, 2, 3, 4, 5, 6, 7])
CO0 = np.array([0, 0, 0, 0, 0, 0, 0, 0])
EP0 = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11])
EO0 = np.array([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

class CubeState:

    def __init__(self, state_arrays=(CP0, CO0, EP0, EO0), alg_str=None):

        (CP,CO,EP,EO)=state_arrays
        assert np.shape(CP)==(8,),  "Vérifiez le format de CP."
        assert np.shape(CO)==(8,),  "Vérifiez le format de CO."
        assert np.shape(EP)==(12,), "Vérifiez le format de EP."
        assert np.shape(EO)==(12,), "Vérifiez le format de EO."
        self.CP = CP
        self.CO = CO
        self.EP = EP
        self.EO = EO

        # FixMe: Ceci retourne l'état résolu ...
        # if not alg_str is None:
        #     alg=alg_str.split(" ")
        #
        #     for move in alg:
        #         self *= all_moves[move]
        #         print(move, self)

    def i(self):
        # Calcul de l'inverse. Retourne un objet CubeState tel que self*self.i()=solved
        CP  =  np.arange(8)
        CO  =  np.zeros(8)
        EP  =  np.arange(12)
        EO  =  np.zeros(12)

        for i, p in enumerate(self.CP):
            CP[p] = i
        for i, p in enumerate(self.EP):
            EP[p] = i

        CO  =  np.remainder(-self.CO[CP], 3)
        EO  =  np.remainder(-self.EO[EP], 2)
        return CubeState((CP, CO, EP, EO))

    def __mul__(self, other):
        assert other.__class__ is CubeState, "Vous essayez de multiplier nimp"

        CP   =  self.CP[other.CP]
        CO   =  np.remainder(self.CO[other.CP] + other.CO, 3)
        EP   =  self.EP[other.EP]
        EO   =  np.remainder(self.EO[other.EP] + other.EO, 2)

        return CubeState((CP, CO, EP, EO))

    def __str__(self):
        return "CP= " + self.CP.__str__() + "\n" + \
               "CO= " + self.CO.__str__() + "\n" + \
               "EP= " + self.EP.__str__() + "\n" + \
               "EO= " + self.EO.__str__() + "\n"

# Définition des mouvements autorisés du Cube
# Définition de R, R', R2
CP = np.array([0, 2, 6, 3, 4, 1, 5, 7])
CO = np.array([0, 1, 2, 0, 0, 2, 1, 0])
EP = np.array([0, 6, 2, 3, 4, 1, 9, 7, 8, 5, 10, 11])
EO = np.array([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

R  = CubeState((CP, CO, EP, EO))
R2 = R*R
Rp = R2*R
R4 = Rp*R

# Définition de U, U', U2
CP = np.array([0, 1, 2, 3, 5, 6, 7, 4])
CO = np.array([0, 0, 0, 0, 0, 0, 0, 0])
EP = np.array([0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 8])
EO = np.array([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

U  = CubeState((CP, CO, EP, EO))
U2 = U*U
Up = U2*U
U4 = Up*U

# Définition de F, F', F2
CP = np.array([1, 5, 2, 3, 0, 4, 6, 7])
CO = np.array([1, 2, 0, 0, 2, 1, 0, 0])
EP = np.array([5, 1, 2, 3, 0, 8, 6, 7, 4, 9, 10, 11])
EO = np.array([1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0])

F  = CubeState((CP, CO, EP, EO))
F2 = F*F
Fp = F2*F
F4 = Fp*F

# Définition de L, L', L2
CP = np.array([4, 1, 2, 0, 7, 5, 6, 3])
CO = np.array([2, 0, 0, 1, 1, 0, 0, 2])
EP = np.array([0, 1, 2, 4, 11, 5, 6, 3, 8, 9, 10, 7])
EO = np.array([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

L  = CubeState((CP, CO, EP, EO))
L2 = L*L
Lp = L2*L
L4 = Lp*L

# Définition de D, D', D2
CP = np.array([3, 0, 1, 2, 4, 5, 6, 7])
CO = np.array([0, 0, 0, 0, 0, 0, 0, 0])
EP = np.array([3, 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11])
EO = np.array([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

D  = CubeState((CP, CO, EP, EO))
D2 = D*D
Dp = D2*D
D4 = Dp*D

# Définition de B, B', B2
CP = np.array([0, 1, 3, 7, 4, 5, 2, 6])
CO = np.array([0, 0, 1, 2, 0, 0, 2, 1])
EP = np.array([0, 1, 7, 3, 4, 5, 2, 10, 8, 9, 6, 11])
EO = np.array([0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0])

B  = CubeState((CP, CO, EP, EO))
B2 = B*B
Bp = B2*B
B4 = Bp*B

all_moves  = {"R":R,"R'":Rp,"R2":R2,"U":U,"U'":Up,"U2":U2,"L":L,"L'":Lp,"L2":L2,"F":F,"F'":Fp,"F2":F2,"B":B,"B'":Bp,"B2":B2,"D":D,"D'":Dp,"D2":D2}
