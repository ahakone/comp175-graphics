EXE = ply
DLIBS = -L./ -L/Users/anzuhakone/glui-2.36/src/lib
INC = -I./ -I/Users/anzuhakone/glui-2.36/src/include -framework OpenGL -framework GLUT -lglui

$(EXE) : 
	g++ -Wall -Wextra ply.cpp main.cpp entity.cpp $(DLIBS) $(INC) -o $@ 

clean:
	rm -rf *.o *~ $(EXE)
