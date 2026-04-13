CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -fPIC
LIBS = -lgmpxx -lgmp

all: lib/libmatrixlib.a lib/libmatrixlib.so

Fraction.o: src/Fraction.cpp include/Fraction.h
	$(CXX) $(CXXFLAGS) -c src/Fraction.cpp -o Fraction.o

Matrix.o: src/Matrix.cpp include/Matrix.h include/Fraction.h
	$(CXX) $(CXXFLAGS) -c src/Matrix.cpp -o Matrix.o

lib/libmatrixlib.a: Fraction.o Matrix.o
	mkdir -p lib
	ar rcs lib/libmatrixlib.a Fraction.o Matrix.o

lib/libmatrixlib.so: Fraction.o Matrix.o
	mkdir -p lib
	$(CXX) -shared -o lib/libmatrixlib.so Fraction.o Matrix.o $(LIBS)

test_static: test/test_matrix.cpp lib/libmatrixlib.a
	$(CXX) -std=c++17 -Iinclude test/test_matrix.cpp lib/libmatrixlib.a $(LIBS) -o test/test_static

test_dynamic: test/test_matrix.cpp lib/libmatrixlib.so
	$(CXX) -std=c++17 -Iinclude test/test_matrix.cpp -Llib -lmatrixlib $(LIBS) -Wl,-rpath,'$$ORIGIN/../lib' -o test/test_dynamic

test_all: test_static test_dynamic

run_static: test_static
	cd test && ./test_static

run_dynamic: test_dynamic
	cd test && ./test_dynamic

clean:
	rm -f Fraction.o Matrix.o lib/libmatrixlib.a lib/libmatrixlib.so test/test_static test/test_dynamic
