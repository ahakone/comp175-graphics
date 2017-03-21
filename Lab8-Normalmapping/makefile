EXE = lab
DLIBS = -L/Users/anzuhakone/glui-2.36/src/lib -L/Users/anzuhakone/glew-1.13.0/src
INC = -I/Users/anzuhakone/glui-2.36/src/include -framework OpenGL -framework GLUT -lglui -I/Users/anzuhakone/glew-1.13.0/include

$(EXE) : 
	g++ -g $(DLIBS) *.cpp -o $@ $(INC)

clean:
	rm -rf *.o *~ $(EXE)

