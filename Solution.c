#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

typedef enum Beneficiu Beneficiu;
typedef struct PastaDeDinti PastaDeDinti;
typedef struct NodGrafLP NodGrafLP;
typedef struct NodGrafLS NodGrafLS;
typedef struct NodArbore NodArbore;
typedef struct NodLD NodLD;
typedef struct LD LD;
typedef struct StivaStergereNodGraf StivaStergereNodGraf;

enum Beneficiu { AlbireDinti=1, ExtraProtectie=2, StopSangerare=3 };

struct PastaDeDinti
{
	int id;
	char* marca;
	float pret;
	int cantitate;
	long codDeBare;
	Beneficiu beneficiu;
	int* rating;
	int nrRating;
};

struct NodGrafLP
{
	PastaDeDinti info;
	NodGrafLP* next;
	NodGrafLS* vecini;
};

struct NodGrafLS
{
	NodGrafLP* inf;
	NodGrafLS* next;
};

struct StivaStergereNodGraf
{
	int id;
	StivaStergereNodGraf* next;
};
struct NodArbore
{
	PastaDeDinti info;
	NodArbore* stanga;
	NodArbore* dreapta;
};

struct NodLD
{
	PastaDeDinti info;
	NodLD* next;
	NodLD* prev;

};

struct LD
{
	NodLD* prim;
	NodLD* ultim;
};

PastaDeDinti initializarePastaDeDinti(int _id, char* _marca, float _pret, int _cantitate, long _codDeBare, Beneficiu _beneficiu, int* _rating, int _nrRating)
{
	PastaDeDinti pd;
	pd.id = _id;
	pd.pret = _pret;
	pd.cantitate = _cantitate;
	pd.codDeBare = _codDeBare;
	pd.beneficiu = _beneficiu;
	pd.marca = (char*)malloc(strlen(_marca) + 1);
	strcpy(pd.marca, _marca);
	
	pd.nrRating = _nrRating;
	pd.rating = (int*)malloc(sizeof(int) * _nrRating);
	for (int i = 0; i < _nrRating; i++)
	{
		pd.rating[i] = _rating[i];
	}
}
//cerinta e - citire din fisier
PastaDeDinti citireFisier(FILE* streamFisier)
{
	if (streamFisier != NULL)
	{
		PastaDeDinti pd;
		char aux[30];

		//id
		fgets(aux, 10, streamFisier);
		pd.id = atoi(aux);

		//marca
		fgets(aux, 30, streamFisier);
		char* sir = strtok(aux, "\n");
		pd.marca = (char*)malloc(strlen(sir) + 1);
		strcpy(pd.marca, sir);

		//pret
		fgets(aux, 10, streamFisier);
		pd.pret = atof(aux);
		//cantitate
		fgets(aux, 10, streamFisier);
		pd.cantitate = atoi(aux);

		//cod de bare
		fgets(aux, 20, streamFisier);
		pd.codDeBare = atol(aux);

		//beneficiu
		fgets(aux, 30, streamFisier);
		char* sirb = strtok(aux, "\n");
		char* beneficiuChar= (char*)malloc(strlen(sirb) + 1);
		strcpy(beneficiuChar, sir);
		if (strcmp(beneficiuChar, "AlbireDinti")==0)
		{
			pd.beneficiu = 1;
		}
		if (strcmp(beneficiuChar, "ExtraProtectie") == 0)
		{
			pd.beneficiu = 2;
		}
		if (strcmp(beneficiuChar, "StopSangerare") == 0)
		{
			pd.beneficiu = 3;
		}


		//nrRating
		fgets(aux, 10, streamFisier);
		pd.nrRating = atoi(aux);

		//rating
		pd.rating = (int*)malloc(pd.nrRating * sizeof(int));
		for (int i = 0; i < pd.nrRating; i++)
		{
			fgets(aux, 10, streamFisier);
			pd.rating[i] = atoi(aux);
		}
		return pd;
	}
}

char* getBeneficiuInCaractere(Beneficiu b)
{
	if (b == 1)
	{
		return "AlbireDinti";
	}
	if (b == 2)
	{
		return "ExtraProtectie";
	}
	if (b == 3)
	{
		return "StopSangerare";
	}
	return "Beneficiu gresit";
}

void afisarePastaDeDinti(PastaDeDinti pd)
{
	printf("id - %d, marca - %s, pret - %.2f, cantitate - %d, cod de bare - %ld, beneficiu - %s, numar rating - %d, valori rating - ",
		pd.id, pd.marca, pd.pret, pd.cantitate, pd.codDeBare, getBeneficiuInCaractere(pd.beneficiu), pd.nrRating);
	for (int i = 0; i < pd.nrRating; i++)
	{
		printf("%d, ", pd.rating[i]);
	}
	printf("\n");
}


//GRAF
NodGrafLP* inserareGrafLP(NodGrafLP* graf, PastaDeDinti pd)
{
	NodGrafLP* nou = (NodGrafLP*)malloc(sizeof(NodGrafLP));
	nou->info = pd;
	nou->next = NULL;
	nou->vecini = NULL;

	if (graf == NULL)
	{
		return nou;
	}
	else
	{
		NodGrafLP* aux = graf;
		while (aux->next)
			aux = aux->next;
		aux->next = nou;
	}
	return graf;
}
NodGrafLS* inserareGrafLS(NodGrafLS* cap, NodGrafLP* info)
{
	NodGrafLS* nou = (NodGrafLS*)malloc(sizeof(NodGrafLS));
	nou->inf = info;
	nou->next = NULL;
	if (cap == NULL)
	{
		return nou;
	}
	else
	{
		NodGrafLS* aux = cap;
		while (aux->next)
			aux = aux->next;
		aux->next = nou;
	}
	return cap;
}

void afisareVecini(NodGrafLP* nod)
{
	NodGrafLS* aux = nod->vecini;
	while (aux)
	{
		afisarePastaDeDinti(aux->inf->info);
		aux = aux->next;
	}
}

void afisareGrafLP(NodGrafLP* graf)
{
	while (graf)
	{
		printf("Pasta de dinti din lista principala:\n");
		afisarePastaDeDinti(graf->info);
		printf("Pastele de dinti din lista secundara:\n");
		afisareVecini(graf);
		printf("\n");
		graf = graf->next;
	}
}

NodGrafLP* cautareNod(NodGrafLP* graf, int id)
{
	while (graf && graf->info.id != id)
	{
		graf = graf->next;
	}
	return graf;
}

void inserareVecini(NodGrafLP* graf, int id1, int id2)
{
	NodGrafLP* nod1 = cautareNod(graf, id1);
	NodGrafLP* nod2 = cautareNod(graf, id2);

	if (nod1 && nod2)
	{
		nod1->vecini=inserareGrafLS(nod1->vecini, nod2);
		nod2->vecini=inserareGrafLS(nod2->vecini, nod1);
	}
}


//dezalocarea vecinilor unui nod
NodGrafLS* dezalocareVecini(NodGrafLP* nod)
{
	NodGrafLS* vecini = nod->vecini;
	while (vecini)
	{
		NodGrafLS* vecin_urmator = vecini->next;
		free(vecini);
		vecini = vecin_urmator;
	}
	return vecini;
}

//cerinta b - dezalocarea grafului
NodGrafLP* dezalocareGraf(NodGrafLP* graf)
{
	NodGrafLP* aux = graf;
	while (aux)
	{
		NodGrafLP* aux2 = aux->next;
		if (aux->vecini)
		{
			aux->vecini = dezalocareVecini(aux);
		}
		free(aux->info.rating);
		free(aux->info.marca);
		free(aux);
		aux = aux2;

	}
	return aux;
}

//adaugarea si scoaterea in/din stiva a nodurilor care il au vecin pe nodul de sters
StivaStergereNodGraf* push(StivaStergereNodGraf* stiva, int id)
{
	StivaStergereNodGraf* nod = (StivaStergereNodGraf*)malloc(sizeof(StivaStergereNodGraf));
	nod->id = id;
	nod->next = stiva;
	return nod;

}
int pop(StivaStergereNodGraf** stiva)
{
	int id;
	if ((*stiva) != NULL)
	{
		id = (*stiva)->id;
		StivaStergereNodGraf* aux = (*stiva);
		(*stiva) = (*stiva)->next;
		free(aux);
	}
	else 
	{
		id = -1;
	}

	return id;

}
//dezalocarea unui nod dupa id din lista de vecini a unui nod principal
NodGrafLS* dezalocareNodLS(NodGrafLP* nod, int id)
{
	NodGrafLS* vecini = nod->vecini;
	NodGrafLS* vecini2 = nod->vecini;
	if (!vecini)
	{
		return NULL;
	}
	else
	{
		if (vecini->inf->info.id == id)
		{
			nod->vecini = nod->vecini->next;
			free(vecini);
			return nod->vecini;
		}
		else
		{
			while (vecini->next)
			{
				if (vecini->inf->info.id == id)
				{
					vecini2->next = vecini->next;
					free(vecini);
					return nod->vecini;
				}
				else
				{
					vecini2 = vecini;
					vecini = vecini->next;
				}
			}
			if (vecini->inf->info.id == id)
			{
				vecini2->next = vecini->next;
				free(vecini);
				return nod->vecini;
			}
			else
			{
				return nod->vecini;
			}
		}
	}
}

NodGrafLP* dezalocareNodLP(NodGrafLP* nod, int id)
{
	NodGrafLP* aux = nod;
	NodGrafLP* aux2 = nod;
	if (!aux)
	{
		return NULL;
	}
	else
	{
		if (aux->info.id == id)
		{
			nod = nod->next;
			free(aux);
			return nod;
		}
		else
		{
			while (aux->next)
			{
				if (aux->info.id == id)
				{
					aux2->next = aux->next;
					free(aux);
					return nod;
				}
				else
				{
					aux2 = aux;
					aux = aux->next;
				}
			}
			if (aux->info.id == id)
			{
				aux2->next = aux->next;
				free(aux);
				return nod;
			}
			else
			{
				return nod;
			}
		}
	}
}

//cerinta a - dezalocarea unui nod din graf
NodGrafLP* dezalocareGrafNodId(NodGrafLP* graf, int id)
{
	NodGrafLP* nodDeSters = cautareNod(graf, id);
	//parcurg vecinii si le pun id-urile in stiva
	NodGrafLS* auxVecini = nodDeSters->vecini;
	StivaStergereNodGraf* stiva = NULL;
	while (auxVecini)
	{
		stiva = push(stiva, auxVecini->inf->info.id);
		auxVecini = auxVecini->next;
	}
	while (stiva)
	{
		NodGrafLP* nod = cautareNod(graf, pop(&stiva));
		nod->vecini = dezalocareNodLS(nod, id);
	}
	//sterg vecinii nodului de sters
	nodDeSters->vecini=dezalocareVecini(nodDeSters);


	//elimin nodul de sters din LP
	graf = dezalocareNodLP(graf, id);
	return graf;
}




//ARBORE
NodArbore* inserareArbore(NodArbore* radacina, PastaDeDinti pd)
{
	if (radacina == NULL)
	{
		NodArbore* nod = (NodArbore*)malloc(sizeof(NodArbore));
		nod->info = pd;
		nod->stanga = NULL;
		nod->dreapta = NULL;
		radacina = nod;
	}
	else
	{
		if (pd.id < radacina->info.id)
		{
			radacina->stanga= inserareArbore(radacina->stanga, pd);
		}
		else
		{
			radacina->dreapta = inserareArbore(radacina->dreapta, pd);
		}
	}
	return radacina;
}

void parcurgereInordine(NodArbore* radacina)
{
	if (radacina)
	{
		parcurgereInordine(radacina->stanga);
		afisarePastaDeDinti(radacina->info);
		parcurgereInordine(radacina->dreapta);
	}
}


//cerinta c
void stergereArboreNodFaraDescendenti(NodArbore** nod)
{
	NodArbore* aux = *nod;
	free(aux->info.marca);
	free(aux->info.rating);
	free(aux);
	*nod = NULL;
}
void stergereArboreNodDescendentStanga(NodArbore** nod)
{
	NodArbore* aux = (*nod);
	NodArbore* aux2 = (*nod)->stanga;
	free(aux->info.marca);
	free(aux->info.rating);
	free(aux);
	(*nod) = aux2;
}
void stergereArboreNodDescendentDreapta(NodArbore** nod)
{
	NodArbore* aux = (*nod);
	NodArbore* aux2 = (*nod)->dreapta;
	free(aux->info.marca);
	free(aux->info.rating);
	free(aux);
	(*nod) = aux2;
}
void stergereArboreNodDoiDescendenti(NodArbore** nod)
{
	NodArbore* aux = (*nod)->stanga;
	NodArbore* aux2 = (NodArbore*)malloc(sizeof(NodArbore));
	while (aux->dreapta)
	{
		aux2 = aux;
		aux = aux->dreapta;
	}
	PastaDeDinti auxpd = aux->info;
	aux->info = (*nod)->info;
	(*nod)->info = auxpd;
	if (aux->stanga == NULL)
	{
		NodArbore* auxx = aux;
		free(auxx->info.marca);
		free(auxx->info.rating);
		free(auxx);
		if (aux2)
		{
			aux2->dreapta = NULL;
		}
		aux = NULL;
	}
	else
	{
		NodArbore* auxx2 = aux->stanga;
		free(aux->info.marca);
		free(aux->info.rating);
		free(aux);
		aux = auxx2;
		(*nod)->stanga = aux;
	}
}
void stergereArboreNodId(NodArbore** radacina, int id)
{
	if (*radacina)
	{
		if ((*radacina)->info.id != id)
		{
			if (id < (*radacina)->info.id)
			{
				stergereArboreNodId(&(*radacina)->stanga, id);
			}
			else
			{
				stergereArboreNodId(&(*radacina)->dreapta, id);
			}
		}
		else
		{
			if ((*radacina)->info.id == id)
			{
				if ((*radacina)->stanga == NULL && (*radacina)->dreapta == NULL)
				{
					stergereArboreNodFaraDescendenti(radacina);
				}
				else
				{
					if ((*radacina)->stanga == NULL)
					{
						stergereArboreNodDescendentDreapta(radacina);
					}
					else
					{
						if ((*radacina)->dreapta == NULL)
						{
							stergereArboreNodDescendentStanga(radacina);
						}
						else
						{
							stergereArboreNodDoiDescendenti(radacina);
						}
					}
				}
			}
		}
	}
	else
	{
		printf("Nodul cu acest id nu exista in arbore");
	}
}




//LISTA DUBLA
LD inserareInceputLD(LD ld, PastaDeDinti pd)
{
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->info = pd;
	nou->prev = NULL;
	nou->next = ld.prim;
	if (ld.prim)
	{
		ld.prim->prev = nou;
		ld.prim = nou;
	}
	else
	{
		ld.prim = ld.ultim = nou;
	}
	return ld;

}
LD inserareFinalLD(LD ld, PastaDeDinti pd)
{
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->info = pd;
	nou->next = NULL;
	nou->prev = ld.ultim;
	if (ld.ultim)
	{
		ld.ultim->next = nou;
		ld.ultim= nou;
	}
	else
	{
		ld.prim = ld.ultim = nou;
	}
	return ld;

}

void traversareStDRLD(LD ld)
{
	NodLD* aux = ld.prim;
	while (aux)
	{
		afisarePastaDeDinti(aux->info);
		aux = aux->next;
	}
}

//cerinta d
void stergereLD(LD* ld, char* marca)
{
	NodLD* aux = ld->prim;
	if (!aux)
	{
		printf("lista este null");
	}
	else
	{
		if (strcmp(aux->info.marca, marca) == 0)
		{
			ld->prim = ld->prim->next;
			if (ld->prim)
				ld->prim->prev = NULL;
			free(aux);
			return;
		}
		else
		{
			while (aux->next)
			{
				if (strcmp(aux->info.marca, marca) == 0)
				{
					aux->prev->next = aux->next;
					aux->next->prev = aux->prev;
					free(aux);
					return;
				}
				else
				{
					aux = aux->next;
				}
			}
			if (strcmp(aux->info.marca, marca) == 0)
			{
				ld->ultim = aux->prev;
				ld->ultim->next = NULL;
				free(aux);
				return;

			}
			else
			{
				printf("Pasta de dinti cautata nu exista in lista");
			}
		}
	}
}
void main()
{
	//citirea din fisier - cerinta e
	char numefisier[11] = "pasta0.txt";
	PastaDeDinti p[8];
	printf("CITIRE DIN FISIER SI AFISARE PASTE DE DINTI\n\n");
	for (int i = 0; i < 8; i++)
	{
		numefisier[5]++;
		FILE* fisier = fopen(numefisier, "r");
		p[i] = citireFisier(fisier);
		afisarePastaDeDinti(p[i]);
		fclose(fisier);
	}
	printf("\n\n----------\n\n");
	NodGrafLP* graf = NULL;
	for (int i = 0; i < 8; i++)
	{
		graf = inserareGrafLP(graf, p[i]);
	}
	inserareVecini(graf, 1, 2);
	inserareVecini(graf, 1, 3);
	inserareVecini(graf, 1, 4);
	inserareVecini(graf, 2, 4);
	inserareVecini(graf, 2, 8);
	inserareVecini(graf, 2, 7);
	inserareVecini(graf, 3, 5); 
	inserareVecini(graf, 5, 6);
	inserareVecini(graf, 7, 6);
	inserareVecini(graf, 8, 7);
	printf("\n\nAFISAREA GRAFULUI CREAT\n\n");
	afisareGrafLP(graf);
	printf("\n\n----------\n\n");
	//dezalocare nod dupa id - cerinta a 
	graf = dezalocareGrafNodId(graf, 2);
	printf("\n\nAFISAREA GRAFULUI DUPA DEZALOCAREA NODULUI CU ID 2\n\n");
	afisareGrafLP(graf);
	printf("\n\n----------\n\n");
	//dezalocare Graf - cerinta b
	graf = dezalocareGraf(graf);
	//graf = dezalocareNodId(graf, 5);
	printf("\n\nAFISAREA GRAFULUI DUPA DEZALOCARE (NULL)\n\n");
	afisareGrafLP(graf);

	numefisier[5] = '0';
	for (int i = 0; i < 8; i++)
	{
		numefisier[5]++;
		FILE* fisier = fopen(numefisier, "r");
		p[i] = citireFisier(fisier);
		fclose(fisier);
	}
	printf("\n\n----------\n\n");
	NodArbore* radacina = NULL;
	radacina = inserareArbore(radacina, p[4]);
	radacina = inserareArbore(radacina, p[2]);
	radacina = inserareArbore(radacina, p[5]);
	radacina = inserareArbore(radacina, p[7]);
	radacina = inserareArbore(radacina, p[6]);
	radacina = inserareArbore(radacina, p[3]);
	radacina = inserareArbore(radacina, p[1]);
	radacina = inserareArbore(radacina, p[0]);
	printf("\n\nAFISAREA ARBORELUI\n\n");
	parcurgereInordine(radacina);
    
	stergereArboreNodId(&radacina, 3);

	printf("\n\n----------\n\n");
	printf("\n\nAFISAREA ARBORELUI DUPA DEZALOCAREA NODULUI CU ID 3\n\n");
	parcurgereInordine(radacina);

	numefisier[5] = '0';
	for (int i = 0; i < 8; i++)
	{
		numefisier[5]++;
		FILE* fisier = fopen(numefisier, "r");
		p[i] = citireFisier(fisier);
		fclose(fisier);
	}
	LD listaDubla;
	
	listaDubla.prim = NULL;
	listaDubla.ultim = NULL;
	for (int i = 0; i < 8; i++)
	{
		if (i % 2 == 0)
		{
			listaDubla = inserareInceputLD(listaDubla, p[i]);
		}
		else
		{
			listaDubla = inserareFinalLD(listaDubla, p[i]);
		}
	}
	printf("\n\nAFISAREA STANGA-DREAPTA A LISTEI DUBLE\n\n");
	traversareStDRLD(listaDubla);
	printf("\n\n----------\n\n");

	stergereLD(&listaDubla, "Sensodyne");
	printf("\n\nAFISAREA STANGA-DREAPTA A LISTEI DUBLE DUPA STERGEREA NODULUI CU MARCA \"Sensodyne\"\n\n");
	traversareStDRLD(listaDubla);



}
