OBJ = expressionSolver.o Expression.o Numbers.o Operations.o
EXE = solver
CXX = g++
CXXFLAGS = -Wall -g -Wextra -MMD -pedantic -std=c++17
SRC_PATH = src

$(EXE): $(OBJ)
	$(CXX) $^ -o $@

%.o: $(SRC_PATH)/%.cpp makefile
	$(CXX) $(CXXFLAGS) -c $<

-include $(OBJ:.o=.d)

clean:
	rm -f $(OBJ) $(EXE) $(OBJ:.o=.d)
.PHONY: clean
