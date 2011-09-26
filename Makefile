LDLIBS = -L/usr/X11R6/lib -lglut -lGLU -lGL -lXmu -lXi -lXext -lX11 -lpthread -lm
TARGETS = tree
OBJECTS = main.o extrusion.o Tree.o Matrix.o Trackball.o

$(TARGETS): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

main.o: tree.h Trackball.h
tree.o: tree.h extrusion.h Matrix.h
extrusioh.o: extrusion.h
Matrix.o: Matrix.h
Trackball.o: Trackball.h

clean:
	-rm -f $(TARGETS) *.o *~
