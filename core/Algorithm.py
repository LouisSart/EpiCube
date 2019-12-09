import numpy as np

move_str_to_int  = {"R":1,"R'":2,"R2":3,"U":4,"U'":5,"U2":6,"L":7,"L'":8,"L2":9,"F":10,"F'":11,"F2":12,"B":13,"B'":14,"B2":15,"D":16,"D'":17,"D2":18}
move_int_to_str  = {1:"R",2:"R'",3:"R2",4:"U",5:"U'",6:"U2",7:"L",8:"L'",9:"L2",10:"F",11:"F'",12:"F2",13:"B",14:"B'",15:"B2",16:"D",17:"D'",18:"D2"}

class Algorithm:

    def __init__(self, arg=None):
        self.moves = []
        if arg is not None:
            if arg.__class__ == str:
                move_str_list = arg.split(" ")
                self.moves = [0]*len(move_str_list)
                for i, move_str in enumerate(move_str_list):
                    n = move_str_to_int[move_str]
                    self.moves[i] = n
            elif arg.__class__==list:
                self.moves = arg


    def tofile(self, f):
        assert f.mode[0] == "a"
        assert not f.closed
        np.array([len(self.moves)]).tofile(f)
        np.array(self.moves).tofile(f)

    def fromfile(self, f):
        assert f.mode[0] == "r"
        assert not f.closed
        n_moves = np.fromfile(f, dtype=int, count=1)[0]
        self.moves = list(np.fromfile(f, dtype=int, count=n_moves))

    def append(self, alg):
        if alg.__class__==Algorithm:
            self.moves += alg.moves
        elif alg.__class__==str:
            self.moves.append(move_str_to_int[alg])

    def __repr__(self):
        str = ""
        for n in self.moves:
            str += move_int_to_str[n] + " "
        return str[:-1]

    def __getitem__(self, k):
        return move_int_to_str[self.moves[k]]

    def __iter__(self):
        return iter(self.moves)

    def __len__(self):
        return len(self.moves)
