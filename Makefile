# Définition des variables
# CC = g++
CFLAGS = -Wall
STD = -std=c++20
TEST_DIR = test
LIB_DIR = lib
INC = -I$(LIB_DIR)
OPT = -O1 
OBJ_DIR = obj

test: obj
	$(CXX) $(STD) $(OPT) $(INC) $(TEST_DIR)/permutation_test.cpp -o $(OBJ_DIR)/permutation

.PHONY: all clean

# Ojects directory
obj:
	mkdir -p $(OBJ_DIR)

# Règle de nettoyage
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/test