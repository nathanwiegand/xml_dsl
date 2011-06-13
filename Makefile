all: dsl

dsl: dsl.cc html.o
	g++ -o $@ $< html.o xml_dsl.o

xml_dsl.o: xml_dsl.cc xml_dsl.h
	g++ -c $<

html.o: html.cc html.h xml_dsl.o
	g++ -c $< xml_dsl.o

run: dsl
	./dsl

clean:
	rm dsl *.o
