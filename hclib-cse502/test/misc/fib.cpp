#include "hclib.h"

using namespace std;

static int threshold = 10;

int fib_serial(int n) {
    if (n <= 2) return 1;
    return fib_serial(n-1) + fib_serial(n-2);
}

int fib(int n)
{
    if (n <= threshold) {
        return fib_serial(n);
    }
    else {
	int x, y;
	hclib::finish([n, &x, &y]( ) {
  	    hclib::async([n, &x]( ){x = fib(n-1);},2);
        hclib::async([n, &y]( ){y = fib(n-2);},1);
//  	    y = fib(n-2);
	}, 3);
  printf("Succesful %d\n", n);
	return x + y;
    }
}

int main (int argc, char ** argv) {
  hclib::init(&argc, argv);
  int n = 40;
  if(argc > 1) n = atoi(argv[1]);
  if(argc > 2) threshold = atoi(argv[2]);

  // printf("Starting Fib(%d)..\n",n);
  int res; 
  hclib::kernel([&]() {
    res = fib(n);
  });
  printf("Fib(%d) = %d\n",n,res);
  hclib::finalize();
  return 0;
}

