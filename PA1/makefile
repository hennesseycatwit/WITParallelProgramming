CC = mpicc
LDC = mpicc
LD_FLAGS =
FLAGS = 
PROG = ring.x
RM = /bin/rm
OBJS = ring.o

all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $(LD_FLAGS) $(OBJS) -o $(PROG)

%.O: %.C
	$(CC) $(FLAGS) -c $<

clean:
	 $(RM) -rf *.o $(PROG) *.mod
