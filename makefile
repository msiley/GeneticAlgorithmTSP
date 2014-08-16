
CC=g++

UTILITY = utility.o random.o randombits.o
TSP = tspfilereader.o chromosome.o tspchromosome.o tspchromofactory.o tsp.cpp
MODTSP = tspfilereader.o chromosome.o tspchromosome.o tspchromofactory.o modtsp.cpp
STDGA = geneticalgorithm.o
BINARY = chromosome.o binarychromosome.o binarychromofactory.o ga2.cpp
MODGA = geneticalgorithm.o modga.o

run_mod : $(UTILITY) $(MODGA) $(MODTSP)
	$(CC) $(UTILITY) $(MODGA) $(MODTSP) -o run_mod
 

run_tsp : $(UTILITY) $(STDGA) $(TSP)
	$(CC) $(UTILITY) $(STDGA) $(TSP) -o run_tsp

run_ga : $(UTILITY) $(BINARY) $(STDGA) 
	$(CC) $(UTILITY) $(BINARY) $(STDGA) -o run_ga


modga.o : modga.cpp
	$(CC) modga.cpp -c

tspfilereader.o : tspfilereader.cpp
	$(CC) tspfilereader.cpp -c

tspchromosome_test : tspchromosome.cpp random.o utility.o
	$(CC) tspchromosome.cpp random.o utility.o -D__TSPCHROMOSOME_TEST__ -o tspchromosome_test

tspchromosome.o : tspchromosome.cpp
	$(CC) tspchromosome.cpp -c

binarychromofactory.o : binarychromofactory.cpp
	$(CC) binarychromofactory.cpp -c

tspchromofactory.o : tspchromofactory.cpp
	$(CC) tspchromofactory.cpp -c

geneticalgorithm.o : geneticalgorithm.cpp
	$(CC) geneticalgorithm.cpp -c

binarychromosome_test : binarychromosome.cpp random.o randombits.o utility.o
	$(CC) binarychromosome.cpp random.o randombits.o utility.o -D__BINARYCHROMOSOME_TEST__ -o binarychromosome_test

binarychromosome.o: binarychromosome.cpp
	$(CC) binarychromosome.cpp -c

chromosome.o : chromosome.cpp
	$(CC) chromosome.cpp -c

randombits_test : randombits.cpp random.o
	$(CC) randombits.cpp random.o -D__RANDOMBITS_TEST__ -o randombits_test

randombits.o : randombits.cpp
	$(CC) randombits.cpp -c

random_test : random.cpp
	$(CC) random.cpp -D__RANDOM_TEST__ -o random_test

random.o : random.cpp
	$(CC) random.cpp -c

utility_test : utility.cpp
	$(CC) utility.cpp -D__UTILITY_TEST__ -o utility_test

utility.o : utility.cpp
	$(CC) utility.cpp -c











