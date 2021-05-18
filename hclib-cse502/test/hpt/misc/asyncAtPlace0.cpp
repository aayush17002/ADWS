#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hclib.h"

int main(int argc, char ** argv) {	
	hclib::init(&argc, argv);
	int numPlaces = hclib::hc_get_num_places(hclib::CACHE_PLACE);
	hclib::place_t ** cachePlaces = (hclib::place_t**) malloc(sizeof(hclib::place_t*) * numPlaces);
	hclib::hc_get_places(cachePlaces, hclib::CACHE_PLACE);

	hclib::place_t * p1 = cachePlaces[0];
	hclib::place_t * p2 = cachePlaces[1];

	hclib::finish([=]() {
		for(int i=0; i<numPlaces; i++) {
			hclib::asyncAtHpt(cachePlaces[i], [=]() {
                        	printf("AsyncAtHpt at Place-%d\n",cachePlaces[i]->id);
                	});
		}
	});
	hclib::finalize();
	return 0;
}
