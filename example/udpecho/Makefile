
TARGETS = udpecho

# ngflow library and header 
NGFLOW_INC = -I${NGFLOW_SDK}/include
NGFLOW_LIB = -L${NGFLOW_SDK}/lib -lngflow

INC = ${NGFLOW_INC} 
LIBS = ${NGFLOW_LIB}

CC=gcc 
CFLAGS = -pthread -Wall -O2
LDFLAGS = -lpthread 


all: udpecho

udpecho.o: udpecho.c
	${CC} -c $< ${CFLAGS} ${INC}

udpecho: udpecho.o
	${CC} $< ${LIBS} ${LDFLAGS} -o $@

clean:
	rm -f *~ *.o ${TARGETS} 

