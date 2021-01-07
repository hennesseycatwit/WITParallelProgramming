CC = tau_cc.sh
LDC = tau_cc.sh
LD_FLAGS = 
FLAGS = 
PROG = LA3.x
RM = /bin/rm
OBJS = LA3.o

all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $(LD_FLAGS) $(OBJS) -o $(PROG)

%.O: %.C
	$(CC) $(FLAGS) -c $<

clean:
	 $(RM) -rf *.o $(PROG) *.mod
