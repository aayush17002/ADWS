

===============
Building HClib:
===============

1) Set the path to the 'BASE' variable correctly in ./scripts/setup.sh   
2) Add the following line in your .bashrc file:   
source /absolute path to hclib-cse502/scripts/setup.sh   
3) Source manually if you just added the above command to .bashrc   
source ./scripts/setup.sh   
4) Start clean build   
./clean.sh   
./install.sh   
OR   
HCLIB_FLAGS="--enable-perfcounter" ./install.sh # WITH LIKWID SUPPORT FOR LINUX PERFORMANCE COUNTERS   

===================
BUILDING TESTCASES
===================

1) Tests are inside "./test/misc"  

2) Simply use "make" in that directory to build the testcases  

====================
EXECUTING TESTCASES
====================

Setup all environment variables properly (see above)

----------> LWSR backend

1) Set total number of workers

a) export HCLIB_WORKERS=<N>

OR 

b) use an HPT xml file. Some sample files in directory ./hpt

export HCLIB_HPT_FILE=/absolute/path/hclib/hpt/hpt-testing.xml

2) See runtime statistics

export HCLIB_STATS=1

3) Pin worker threads in round-robin fashion (supported only on Linux)

export HCLIB_BIND_THREADS=1

4) Execute the testcase:

./a.out command_line_args


