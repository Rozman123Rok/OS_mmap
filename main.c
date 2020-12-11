#include "mymalloc.h"

// Rok Rozman

int main(){
	char* niz = (char*) mymalloc(45*sizeof(char));
	int *polje1 = (int *) mymalloc(60 * sizeof(int));
	int *polje2 = (int *) mymalloc(40 * sizeof(int));
 
    for(int i = 0; i < 60; i++) {
        polje1[i] = i;
    }

    for(int i = 0; i < 40; i++) {
        polje2[i] = 123 + i;
    }

    printf("polje[2]: %d\n", polje2[2]);
    printf("polje[49]: %d\n", polje2[39]);
    
	printf("polje1[0]: %d\n", polje1[0]);
    printf("polje1[30]: %d\n", polje1[30]);

    myfree(polje1);
	
	///printf("tuuu\n");
	myfree(polje2);

	printf("Pred niz\n");
	myfree(niz);
	
	/// NAPAKA KER HOCEMO PISAT KO SMO ZE ZBISALI
	//printf("polje[2] = 1000\n");
	//polje1[2] = 10000; /// napaka ker smo ga zbrisali
    return 0;
}