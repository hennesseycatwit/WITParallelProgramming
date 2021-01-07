CC=mpicc
CPP=mpic++
LDC=mpicc
LDP=mpic++
LD_FLAGS = -lstdc++ -lopencv_core -lopencv_highgui -lopencv_imgproc
FLAGS= -I/usr/include/opencv
PROGC = test.cx
OBJSC = imageTestC.o imageToolsC.o

OBJSF = imageToolsC.o imgBindingF.o imageTestF.o 
RM = /bin/rm

#all rule
all: $(PROGC)

$(PROGC): $(OBJSC)
	$(LDP) $^ $(LD_FLAGS) -o $@
$(PROGF): $(OBJSF)
	$(LD) $^ $(LD_FLAGS) -o $@
%C.o: %.c
	$(CC) $(FLAGS) -c $^ -o $@
%C.o: %.cpp
	$(CPP) $(FLAGS) -c $^ -o $@



#clean rule
clean:
	$(RM) -rf *.o $(PROGC) *.mod
