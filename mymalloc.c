#	include "mymalloc.h"

struct malloc_info *zadnja = NULL; ///nastavimo poinet na zadnjo stran na NULL ker se ni

void *mymalloc(size_t size){
	int page_size = getpagesize(); // VELIKOST STRANI 
	uint8_t malloc_info_size = sizeof(struct malloc_info); // VELIKOST MALLOC_INFO
    uint8_t odsek_velikost = sizeof(struct odsek); // VELIKOST ODSEK
	//printf("size of odsek: %d, size of alloc: %d\n", odsek_velikost, malloc_info_size);

	if(zadnja == NULL){
		/// SE NISMO NAREDILI DOBENE STRANI
		//printf("Nimamo se strani rabimo novo!\n");
		uint32_t total_size = malloc_info_size + size + odsek_velikost; /// Vsa velikost ki jo rabimo
		//printf("Total_size: %d\n", total_size);
		//printf("Size: %ld\n", size);
		///printf("Odsek velikost: %d\n", odsek_velikost);
		//printf("Page velikost: %d\n", page_size);
		zadnja = (struct alloc_info *) mmap(NULL, page_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0); /// dodelimo celo stran
		
		zadnja[0].alloc_size = total_size;  /// koliko prostora zavzame
		zadnja[0].st_odsekov = 1; /// ker smo dodali prvi odsek
        zadnja[0].prvi_prost = ((uint8_t*)zadnja) + total_size + 1; /// od kod naprej je prosto
        
		/// NAREDIMO ODSEK S PODATKI
		struct odsek *novOdsek = (struct odsek *) (((uint8_t*)zadnja) + malloc_info_size);
        novOdsek[0].zac_strani = zadnja; // kje je zacetek strani
        novOdsek[0].odsek_velikost = size; /// kako veliko prostora za podatke
		
		return ((uint8_t*)novOdsek) + odsek_velikost; /// Vrnemo od kod lahko pisemo podatke
	}
	else{
		/// ŽE IMAMO STRAN
		printf("Imamo stran!\n");

		uint32_t total_size = size + odsek_velikost; /// rabimo samo velikost podatkov in info o odseku
		if(zadnja->alloc_size + total_size <= page_size){
			/// še imamo prostor na tej strani
			///printf("total: %d\n", total_size);
			//printf("zadnja alloc: %d\n", zadnja->alloc_size);
			//printf("total+ zadnja->all: %d\n", zadnja->alloc_size + total_size);
			printf("Se imamo prostor na tej strani!\n");
			
			struct odsek *novOdsek = (struct odsek *) zadnja->prvi_prost; /// nov odsek od tam kjer je zadnji prost
			novOdsek[0].odsek_velikost = size; /// kako je velik
            novOdsek[0].zac_strani = zadnja; // kjer se zacne

			zadnja[0].prvi_prost = ((uint8_t*)zadnja[0].prvi_prost) + total_size + 1; /// kateri je novi prvi prost
            zadnja[0].alloc_size += total_size + 2; /// nova velikost
            zadnja[0].st_odsekov++; // povecamo st odsekkov

			return ((uint8_t*)novOdsek) + odsek_velikost; /// vrnemo poinet na novOdsek plus njegova velikost
		}
		else{
			/// premalo prostora
			printf("Premalo prostora rabimo novo!\n");
			//printf("total: %d\n", total_size);
			//printf("zadnja alloc: %d\n", zadnja->alloc_size);
			//printf("total+ zadnja->all: %d\n", zadnja->alloc_size + total_size);

			/// ustvarjamo novo

			uint32_t total_size = malloc_info_size + size + odsek_velikost; /// Vsa velikost ki jo rabimo
			printf("Ustvarjamo novo!\n");
			//printf("Total_size: %d\n", total_size);
			//printf("Size: %ld\n", size);
			//printf("Odsek velikost: %d\n", odsek_velikost);
			//printf("Page velikost: %d\n", page_size);
            zadnja = (struct alloc_info *) mmap(NULL, page_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0); /// dodelimo celo stran
			zadnja[0].alloc_size = total_size;  /// koliko prostora zavzame
			zadnja[0].st_odsekov = 1; /// ker smo dodali prvi odsek
        	zadnja[0].prvi_prost = ((uint8_t*)zadnja) + total_size + 1; /// od kod naprej je prosto

			/// NAREDIMO ODSEK S PODATKI
			struct odsek *novOdsek = (struct odsek *) (((uint8_t*)zadnja) + malloc_info_size); /// naredimo nov odsek
        	novOdsek[0].zac_strani = zadnja; // kje je zacetek strani
        	novOdsek[0].odsek_velikost = size; /// kako veliko prostora za podatke

            return ((uint8_t*)novOdsek) + odsek_velikost;
		}


	}

}

void myfree(void* addr){
	/// zelimo sprostit prostor
	printf("Zelimo sprostit prostor\n");
	int page_size = getpagesize(); // VELIKOST STRANI 
	uint8_t odsek_velikost = sizeof(struct odsek); // VELIKOST ODSEK
	
	struct odsek* zbrisan = (struct odsek*) (((uint8_t*)addr) - sizeof(struct odsek)); /// tisti kateri bo izbrisan
	//struct odsek* zbrisan;
	//zbrisan = (struct odsek*)(addr - odsek_velikost);
	
    struct malloc_info *stran = (struct malloc_info *) zbrisan[0].zac_strani; /// podatki o celi strani na kateri je

    if(stran->st_odsekov == 0) {
		/// Nimamo odsekov torej je prazna
        printf("Napaka prazna stran\n");
        _exit(1); /// vrnemo napako
    }
	/// MOREMO ZMANSATI ST ODSEKOV
    stran->st_odsekov--; /// zmansamo st odsekov

    if(stran->st_odsekov == 0) {
		// ZBRISEMO CELOTNO STRAN
        printf("Stran izbrisana\n");
        munmap(stran, page_size); /// zbrisemo stran
    }

}



/*
void function() {
	printf("AVTOR ROK ROZMAN! \n"); /// izpise avtorja 
}
*/