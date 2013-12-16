CFLAGS = -Wall
LIBINC = ../subject.h ../observer.h
LIBSRC = ../subject.cpp ../observer.cpp

test : $(LIBINC) $(LIBSRC) hp.h text.h bar.h num.h main.cpp
	c++ -o $@ $(LIBSRC) main.cpp

clean :
	rm -f test core core.*
