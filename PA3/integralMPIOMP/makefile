CC = mpicc
LDC = mpicc
LD_FLAGS = -fopenmp
FLAGS = -fopenmp
RM= /bin/rm

FILES = $(shell find *.c)

CPROGS = $(addsuffix x, $(FILES))

OBJSC=$(CPROGS:%.cx=%C.o) 

#all rule
all: $(CPROGS)


$(CPROGS):$(OBJSC)
	$(LDC) $^ $(LD_FLAGS) -o $@

%C.o: %.c
	$(CC) -c $^ $(FLAGS) -o $@



#clean rule
clean:
	$(RM) -rf *.o *.x *.cx *.mod

