SRC=main.cc $(wildcard *.hh)
main: $(SRC)
	g++ -o$@ -O3 main.cc