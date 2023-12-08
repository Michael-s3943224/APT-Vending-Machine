APT-Assignment-2

Information for compiling and running the Penelope's Pastry Delights program.

1. Change directory to the files location by running the command "cd *insert file location*"
2. Compile the program by running the command "make".
3. Run the program by running the command "./ppd stock.dat coins.dat".

Test Case Compile:
"make test name=<test_name>"

This runs the following commands in order to run test cases:
TEST CASES REMOVED
1. cp ./testCases/${name}/stock_original.dat ./testCases/${name}/stock.dat 
2. cp ./testCases/${name}/coins_original.dat ./testCases/${name}/coins.dat
3. ./ppd ./testCases/${name}/stock.dat ./testCases/${name}/coins.dat < ./testCases/${name}/${name}.input > ./testCases/${name}/${name}.actual_ppd_out
4. diff -w ./testCases/${name}/${name}.output ./testCases/${name}/${name}.actual_ppd_out
5. diff -w -y ./testCases/${name}/${name}.expcoins ./testCases/${name}/coins.dat