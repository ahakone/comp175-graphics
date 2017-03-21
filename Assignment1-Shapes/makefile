EXE = a1
DLIBS = -L./ -L/usr/local/glui-2.36/src/lib -lglui -lGL -lGLU -lglut 
INC = -I./ -I/usr/local/glui-2.36/src/include

$(EXE) : 
	g++ $(INC) Assignment1.cpp -o $@ $(DLIBS)

clean:
	rm -rf *.o *~ $(EXE)
