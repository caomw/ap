#Makefile for Clustering Aggregation
#Modified by: R.J. López Sastre

CC = g++
FLAGS = -Wall -g -fopenmp
INCL = .

AGGSRC = aggregated.cc Matrix.cc LabeledSet.cc DynArr.cc Agglomerative.cc LocalSearch.cc Balls.cc Optimal.cc Utils.cc Sampling.cc
AGGOBJ = ${AGGSRC:.cc=.o}
AGGBIN = aggregated

C2MSRC = c2m.cc distance.cc
C2MOBJ = ${C2MSRC:.cc=.o}
C2MBIN = c2m

BESTSRC = bestclustering.cc
BESTOBJ = ${BESTSRC:.cc=.o}
BESTBIN = bestclustering

S2ISRC = str2int.cc
S2IOBJ = ${S2ISRC:.cc=.o}
S2IBIN = str2int

OBJS = ${AGGOBJ} ${C2MOBJ} ${BESTOBJ} ${S2IOBJ} 
BINS = ${AGGBIN} ${C2MBIN} ${BESTBIN} ${S2IBIN}

all: ${AGGBIN}

%.o: %.cc
	${CC} -I${INCL} ${FLAGS} -c $<

${AGGBIN}: ${AGGOBJ}
	${CC} ${FLAGS} ${AGGOBJ} -lm -o ${AGGBIN}

${C2MBIN}: ${C2MOBJ}
	${CC} ${FLAGS} ${C2MOBJ} -lm -o ${C2MBIN}

${S2IBIN}: ${S2IOBJ}
	${CC} ${FLAGS} ${S2IOBJ} -lm -o ${S2IBIN}

${BESTBIN}: ${BESTOBJ}
	${CC} ${FLAGS} ${BESTOBJ} -lm -o ${BESTBIN}

clean:
	rm -rf ${OBJS} ${BINS} *~


