CC=tau_cc.sh
CPP=mpic++
LDC=tau_cc.sh
LDP=mpic++
LD_FLAGS= -lstdc++ -fopenmp -lopencv_core -lopencv_highgui -lopencv_imgproc
CPPFLAGS= -I/usr/include/opencv $(FLAGS)
FLAGS= -fopenmp
PROG = mandelbrot.cx
RM = /bin/rm

#all rule
OBJS=$(PROG:%.cx=%.o)
TOOLS=imageTools.o

all: $(PROG)

$(PROG): $(OBJS) $(TOOLS)
	$(LDC) $^ $(LD_FLAGS) -o $@


%.o: %.c
	$(CC) $(FLAGS) -c $^ -o $@
%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@



#clean rule
clean:
	$(RM) -rf *.o $(PROGC) *.mod
