CPPFLAGS = -Wall -Werror -pedantic -ggdb3 -std=gnu++98
test: bsttest.o
	g++ $(CPPFLAGS) -o bsttest bsttest.o

test.o: test.cpp bstmap.h
	g++ $(CPPFLAGS) -c bsttest.cpp bstmap.h

.PHONY: clean
clean:
	rm -f *~ *.o bsttest