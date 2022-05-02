OBJ = expressionSolver.o 
EXE = solver
CXX = g++
CXXFLAGS = -Wall -g -Wextra -MMD -pedantic -std=c++17

$(EXE): $(OBJ)
	$(CXX) $^ -o $@

%.o: %.cpp makefile
	$(CXX) $(CXXFLAGS) -c $<

-include $(OBJ:.o=.d)

clean:
	rm -f $(OBJ) $(EXE) $(OBJ:.o=.d)
.PHONY: clean
