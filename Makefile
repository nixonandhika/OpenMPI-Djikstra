#make file - this is a comment section

all:    #target name
	mpicc src/dijkstra.c -o out
	scp out 13517125@167.205.35.150:~
	scp out 13517125@167.205.35.151:~
	scp out 13517125@167.205.35.152:~
	scp out 13517125@167.205.35.153:~
	scp out 13517125@167.205.35.154:~
	scp out 13517125@167.205.35.155:~
