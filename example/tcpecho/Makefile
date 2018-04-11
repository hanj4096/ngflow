
TARGETS = tcpecho

# ngflow library and header 
NGFLOW_INC = -I${NGFLOW_SDK}/include
NGFLOW_LIB = -L${NGFLOW_SDK}/lib -lngflow

INC = ${NGFLOW_INC} 
LIBS = ${NGFLOW_LIB}

CC=gcc 
CFLAGS = -pthread -Wall -O2
LDFLAGS = -lpthread 


all: tcpecho

tcpecho.o: tcpecho.c
	${CC} -c $< ${CFLAGS} ${INC}

tcpecho: tcpecho.o
	${CC} $< ${LIBS} ${LDFLAGS} -o $@

clean:
	rm -f *~ *.o ${TARGETS} 

