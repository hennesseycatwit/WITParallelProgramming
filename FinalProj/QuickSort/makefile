CC = tau_cc.sh
LDC = tau_cc.sh
LD_FLAGS = -fopenmp
FLAGS = -fopenmp
PROG = quick.cx
RM = /bin/rm
OBJS=$(PROG:%.cx=%.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $^ $(LD_FLAGS) -o $@

%.O: %.C
	$(CC) $(FLAGS) -c $<

clean:
	 $(RM) -rf *.o* $(PROG) *.mod
