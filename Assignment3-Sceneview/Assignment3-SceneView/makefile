EXE = a3
DLIBS = -L/Users/anzuhakone/glui-2.36/src/lib 
INC = -I/Users/anzuhakone/glui-2.36/src/include -framework OpenGL -framework GLUT -lglui

$(EXE) : 
	g++ -g $(DLIBS) *.cpp -o $@ $(INC)

clean:
	rm -rf *.o *~ $(EXE)

