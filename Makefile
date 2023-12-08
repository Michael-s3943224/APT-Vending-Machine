.default: all

all: ppd

clean:
	rm -rf ppd *.o *.dSYM

ppd: Coin.o Node.o LinkedList.o ppd.o Helper.o VendingMachine.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

test:
	cp ./testCases/${name}/stock_original.dat ./testCases/${name}/stock.dat 
	cp ./testCases/${name}/coins_original.dat ./testCases/${name}/coins.dat
	./ppd ./testCases/${name}/stock.dat ./testCases/${name}/coins.dat < ./testCases/${name}/${name}.input > ./testCases/${name}/${name}.actual_ppd_out
	-diff -w ./testCases/${name}/${name}.output ./testCases/${name}/${name}.actual_ppd_out
	-diff -w -y ./testCases/${name}/${name}.expcoins ./testCases/${name}/coins.dat

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^
