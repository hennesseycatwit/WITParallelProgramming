CC = mpicc
LDC = mpicc
LD_FLAGS =
FLAGS = 
PROG = dot1.x
RM = /bin/rm
OBJS = dot1.o

all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $(LD_FLAGS) $(OBJS) -o $(PROG)

%.O: %.C
	$(CC) $(FLAGS) -c $<

clean:
	 $(RM) -rf *.o* $(PROG) *.mod
