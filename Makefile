# Définition des variables
CFLAGS = -Wall
STD = -std=c++20
TEST_DIR = test
LIB_DIR = lib
INC = -I$(LIB_DIR)
OPT = -O1 
OBJ_DIR = obj

LIB := $(wildcard $(LIB_DIR)/*.hpp) # Tous les fichiers headers de lib
222SRC := $(wildcard 222/*.cpp) # Tous les fichiers sources pour le 2x2x2
# HDR := $(wildcard $(SRC_DIR)/*.hpp) # Tous les headers sauf main et test
# OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) # Tous les fichiers objets

permutation: obj $(LIB)
	$(CXX) $(STD) $(OPT) $(INC) $(TEST_DIR)/permutation_test.cpp -o $(OBJ_DIR)/permutation

222: $(TEST_DIR)/222.cpp $(OBJ_DIR)/222.o
	$(CXX) $(STD) $(OPT) $(INC) -c $(TEST_DIR)/222.cpp -o $(OBJ_DIR)/222_test.o
	$(CXX) $(STD) $(OPT) $(INC) $(OBJ_DIR)/222.o  $(OBJ_DIR)/222_test.o -o $(OBJ_DIR)/222

$(OBJ_DIR)/222.o: $(LIB) $(222SRC)
	$(CXX) $(STD) $(OPT) $(INC) -c $(222SRC) -o $(OBJ_DIR)/222.o

.PHONY: all clean

# Ojects directory
obj:
	mkdir -p $(OBJ_DIR)

# Règle de nettoyage
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/test