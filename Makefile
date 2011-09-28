CXXFLAGS = -DX11 -Wall
LDLIBS = -L/usr/X11R6/lib -lglut -lGLU -lGL -lm
TARGETS = tree
OBJECTS = main.o extrusion.o Tree.o Matrix.o Trackball.o

$(TARGETS): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

main.o: Tree.h Trackball.h
extrusioh.o: extrusion.h
Tree.o: Tree.h extrusion.h Matrix.h
Matrix.o: Matrix.h
Trackball.o: Trackball.h

clean:
	-rm -f $(TARGETS) *.o *~
