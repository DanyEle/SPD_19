all: kmeans generatedata

clean:
	rm -f ./kmeans ./generatedata ./plotexample.data ./plotexample2.data 

kmeans:	kmeans.c kmeans.h util.c
	#${CC} kmeans.c util.c -o kmeans
	mpicc kmeans.c util.c -o kmeans


generatedata:	generatedata.c kmeans.h util.c 
	${CC} generatedata.c util.c -o generatedata

plotexample.data:	generatedata
	./generatedata 5 1 >plotexample.data

plotexample2.data:	generatedata
	./generatedata 15 1 >plotexample2.data

.PHONY:	plotexample test

plotexample:	generatedata plotexample.gplot plotexample.data
	gnuplot plotexample.gplot

test:	kmeans plotexample.data plottest.gplot
	./kmeans 5 1 <plotexample.data >plottest.cent
	gnuplot plottest.gplot

test2:	kmeans plotexample2.data plottest.gplot
	./kmeans 5 1 <plotexample2.data >plottest.cent
	gnuplot plottest2.gplot

