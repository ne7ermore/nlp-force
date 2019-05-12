GXX = g++
GXXFLAGS = -std=c++11
DYNFLAGS = -fPIC -shared
OBJS = utils.cc bktree.cc	

utils.o:
	$(GXX) $(GXXFLAGS) -c utils.cc

bktree.o:
	$(GXX) $(GXXFLAGS) -c bktree.cc	

dynamic:
	$(GXX) $(GXXFLAGS) $(DYNFLAGS) $(OBJS) -o libtree.so

pybind:
	$(GXX) $(GXXFLAGS) $(DYNFLAGS) -0 -Wall `python3 -m pybind11 --includes` bktree_pybind.cc -o bktree_pybind`python3-config --extension-suffix` -L. -lbktree

clean:
	rm -rf *.o *.so *.a bktree