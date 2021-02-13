HEADERS=$(wildcard *.hh)
main: main.cc $(HEADERS)
	g++ -o$@ -O3 main.cc

final: final.cc $(HEADERS)
	g++ -o$@ -O3 final.cc