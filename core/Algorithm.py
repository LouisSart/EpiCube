import numpy as np

move_str_to_int  = {"R":1,"R'":2,"R2":3,"U":4,"U'":5,"U2":6,"L":7,"L'":8,"L2":9,"F":10,"F'":11,"F2":12,"B":13,"B'":14,"B2":15,"D":16,"D'":17,"D2":18}
move_int_to_str  = {1:"R",2:"R'",3:"R2",4:"U",5:"U'",6:"U2",7:"L",8:"L'",9:"L2",10:"F",11:"F'",12:"F2",13:"B",14:"B'",15:"B2",16:"D",17:"D'",18:"D2"}

class Algorithm:

    def __init__(self, str=None):
        self.moves = []
        if str is not None:
            move_str_list = str.split(" ")
            for move_str in move_str_list:
                n = move_str_to_int[move_str]
                self.moves.append(n)

    def tofile(self, f):
        np.save(f, self.moves)



    def __str__(self):
        str = ""
        for n in self.moves:
            str += move_int_to_str[n] + " "
        str = str[:-1]
        return str

    def __iter__(self):
        return iter(self.moves)

    def __len__(self):
        return len(self.moves)
