CC = mpicc
LDC = mpicc
LD_FLAGS = 
FLAGS = 
PROG = mvp-student.x
RM = /bin/rm
OBJS = mvp-student.o

all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $(LD_FLAGS) $(OBJS) -o $(PROG)

%.O: %.C
	$(CC) $(FLAGS) -c $<

clean:
	 $(RM) -rf *.o $(PROG) *.mod
