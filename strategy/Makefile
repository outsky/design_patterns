CFLAGS = -Wall

test : main.cpp str_processor.h stralg.h stralg_lower.h stralg_upper.h stralg_trim.h
	c++ -o $@ $(CFLAGS) main.cpp -lreadline

clean :
	rm - f test
