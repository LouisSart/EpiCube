# Définition des variables
CFLAGS = -Wall
STD = -std=c++20
TEST_DIR = test
LIB_DIR = lib
INC = -I$(LIB_DIR)
OPT = -O1 
OBJ_DIR = obj

LIB := $(wildcard $(LIB_DIR)/*.hpp) # Tous les fichiers headers de lib
222SRC = 222/222.cpp # Le fichier source pour le 2x2x2
333SRC = 333/333.cpp # Le fichier source pour le 3x3x3

permutation: obj $(LIB)
	$(CXX) $(STD) $(OPT) $(INC) $(TEST_DIR)/permutation_test.cpp -o $(OBJ_DIR)/permutation

222: obj $(TEST_DIR)/222.cpp $(OBJ_DIR)/222.o
	$(CXX) $(STD) $(OPT) $(INC) -c $(TEST_DIR)/222.cpp -o $(OBJ_DIR)/222_test.o
	$(CXX) $(STD) $(OPT) $(INC) $(OBJ_DIR)/222.o  $(OBJ_DIR)/222_test.o -o $(OBJ_DIR)/222_test

$(OBJ_DIR)/222.o: obj $(LIB) $(222SRC)
	$(CXX) $(STD) $(OPT) $(INC) -c $(222SRC) -o $(OBJ_DIR)/222.o

333: obj $(TEST_DIR)/333.cpp $(OBJ_DIR)/333.o
	$(CXX) $(STD) $(OPT) $(INC) -c $(TEST_DIR)/333.cpp -o $(OBJ_DIR)/333_test.o
	$(CXX) $(STD) $(OPT) $(INC) $(OBJ_DIR)/333.o  $(OBJ_DIR)/333_test.o -o $(OBJ_DIR)/333_test

$(OBJ_DIR)/333.o: obj $(LIB) $(333SRC)
	$(CXX) $(STD) $(OPT) $(INC) -c $(333SRC) -o $(OBJ_DIR)/333.o

.PHONY: all clean

# Ojects directory
obj:
	mkdir -p $(OBJ_DIR)

# Règle de nettoyage
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/test 