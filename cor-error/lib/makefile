GXX = g++
GXXFLAGS = -std=c++11
DYNFLAGS = -fPIC -shared
OBJS = utils.cc bktree.cc	

default: clean dynamic pybind

utils.o:
	$(GXX) $(GXXFLAGS) -c utils.cc

bktree.o:
	$(GXX) $(GXXFLAGS) -c bktree.cc	

dynamic:
	$(GXX) $(GXXFLAGS) $(DYNFLAGS) $(OBJS) -o libtree.so

pybind:
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` bktree_pybind.cc -o bktree_pybind`python3-config --extension-suffix` -L. -ltree

clean:
	rm -rf *.o *.so *.a bktree