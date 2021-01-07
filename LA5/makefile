CC = mpicc
LDC = mpicc
LD_FLAGS = 
FLAGS = -lm
PROG = Monte.x
RM = /bin/rm
OBJS = Monte.o

all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $(LD_FLAGS) $(OBJS) -o $(PROG)

%.O: %.C
	$(CC) $(FLAGS) -c $<

clean:
	 $(RM) -rf *.o* $(PROG) *.mod
