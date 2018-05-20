CXX=g++
CXXFLAGS=-std=c++14 -Wall -MMD -lX11 -L/opt/X11/lib -I/opt/X11/include
OBJECTS=displayable.o displayable/snakenode.o displayable/snake3.o displayable/food.o displayable/food/frog.o displayable/food/fruit.o displayable/food/potion.o main.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=snake.exe
${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}
clean:
	rm *.o *.d displayable/*.o displayable/*.d displayable/food/*.o displayable/food/*.d main
.PHONY: clean
