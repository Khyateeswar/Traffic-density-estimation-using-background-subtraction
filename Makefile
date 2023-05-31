all:
	rm output
m1:
	g++  method1.cpp -o output -std=c++11 `pkg-config --cflags --libs opencv`
m2:
	g++  method2.cpp -o output -std=c++11 `pkg-config --cflags --libs opencv`
m3:
	g++  method3.cpp -o output -std=c++11 `pkg-config --cflags --libs opencv` -lpthread
m4:
	g++  method4.cpp -o output -std=c++11 `pkg-config --cflags --libs opencv` -lpthread
base:
	g++  base.cpp -o output -std=c++11 `pkg-config --cflags --libs opencv`
runm1:
	./output $(A) 
runm2:
	./output $(A) $(B)
runm3:
	./output $(A) 
runm4:
	./output $(A)
runbase:
	./output 

