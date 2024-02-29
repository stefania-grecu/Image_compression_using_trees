//GRECU STEFANIA 313CB

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "header.h"
//calcularea pigmentului mediei rosu
unsigned long long formulaR(int x, int y, int size, TMat **mat) {
	unsigned long long r = 0;
    int i, j;

	for(i = x; i < x + size; i++)
		for (j = y; j < y + size; j++)
			r = r + mat[i][j].red;
	r = r / (size * size);

    return floor(r);
}

//calcularea pigmentului mediei verde
unsigned long long formulaG(int x, int y, int size, TMat **mat) {
    unsigned long long g = 0;
    int i, j;

	for( i = x; i < x + size; i++)
		for (j = y; j < y + size; j++)
			g = g + mat[i][j].green;
	g = g / (size * size);

    return floor(g);
}

//calcularea pigmentului mediei albastru
unsigned long long formulaB(int x, int y, int size, TMat **mat) {
    unsigned long long b = 0;
    int i, j;

	for(i = x; i < x + size; i++)
		for (j = y; j < y + size; j++)
			b = b + mat[i][j].blue;
	b = b / (size * size);

    return floor(b);
}

//calcularea lui mean aplicand formula pe cele 3 medii de mai sus
unsigned long long formulaMEAN (int x, int y, int size, TMat **mat, unsigned char red, unsigned char green, unsigned char blue) {
	unsigned long long mean = 0, result;
    unsigned long long a, b, c;
    double current_mean;
    int i, j;
	for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            a = (red - mat[i][j].red) * (red - mat[i][j].red);
            b = (green - mat[i][j].green) * (green - mat[i][j].green);
            c = (blue - mat[i][j].blue) * (blue - mat[i][j].blue);
            mean = mean + a + b + c;
        }
    }
	mean =  mean / (3 * size * size);

    return floor(mean);
}

//construirea arborelui
TArb construire(int x, int y, int factor, TMat **mat, int n, int max) {
    //aflam mediile celor 3 pigmenti si mean
    unsigned long long r, g, b;
    r = formulaR(x, y, n, mat);
    g = formulaG(x, y, n, mat);
    b = formulaB(x, y, n, mat);

	unsigned long long mean = formulaMEAN(x, y, n, mat, r, g, b);

    //comparare mean cu factor
	if(mean <= factor) {
        //nu trebuie divixat asa ca punem in arbore rgb-ul calculat mai sus si legam fii la NULL
        //in tip punem 1 pentru a sugera ca este frunza
        TArb arb = (TArb) malloc (sizeof(TNod));
        arb->red = r;
        arb->green = g;
        arb->blue = b;
        arb->tip = 1;

        arb->f1 = NULL;
        arb->f2 = NULL;
        arb->f3 = NULL;
        arb->f4 = NULL;

        return arb;
    }
	else {
        //trebuie divizat
        TArb a, arb;
        //alocam spatiu pentru noul arbore
        arb = (TArb) malloc ( sizeof(TNod));

        a = arb;

        //punem in arbore valoarea maxima pe care o poate avea un pixel
        //si in tip punem 0, asta insemnand ca este nod intern
        arb->red = max;
		arb->green = max;
		arb->blue = max;
        arb->tip = 0;

        //apelam recursiv functia de construire arbore pentru fiecare dintre cei 4 fii
        //doar ca schimbam x si y (coltul din stanga sus a matricei) la fiecare reapel
		arb->f1 = construire(x, y, factor, mat, n / 2, max);
		arb->f2 = construire(x, y + n / 2, factor, mat, n / 2, max);
		arb->f3 = construire(x + n / 2, y + n / 2, factor, mat, n / 2, max);
		arb->f4 = construire(x + n / 2, y, factor, mat, n / 2, max);

        //returnam inceputul arborelui(radacina)
        return a;
	}

}

//functie de parcurgere a arborelui care m-a ajutat sa vad daca l-am construit bine 
void parcurge(TArb r) {
    if(r == NULL) {
        printf("arbore vid");
        return;
    }
    
    printf("%u --- %d %d %d\n",r->tip, r->red, r->green, r->blue);
	if(r->f1 == NULL) {
        return;
    } 
	parcurge(r->f1);
    parcurge(r->f2);
    parcurge(r->f3);
    parcurge(r->f4);
}

//numarul de niveluri
int NrNiv(TArb r)           //numar niveluri (0 pentru arbore vid)
{
	int n1 = 0, n2 = 0, n3 = 0, n4 = 0;
	if (r == NULL) return 0;
    //apelez recursiv functia si daca unul dintre cele 4 niveluri este mai mare sau egal decat toate celelalte 
    //la intoarcerea din recursivitate se aduna unu in asa fel incat cand ajunge la radacina functia sa intoarca numarul corect
	n1 = NrNiv(r->f1);
    n2 = NrNiv(r->f2);
    n3 = NrNiv(r->f3);
    n4 = NrNiv(r->f4);
    if(n1 >= n2 && n1 >= n3 && n1 >= n4)
	return 1 + n1;
    if(n2 >= n1 && n2 >= n3 && n2 >= n4)
	return 1 + n2;
    if(n3 >= n1 && n3 >= n2 && n3 >= n4)
	return 1 + n3;
    if(n4 >= n1 && n4 >= n2 && n4 >= n3)
	return 1 + n4;
}

//nivelul minim
int niv_min(TArb r, int niv)
{
	if(r == NULL)
		return -1;
	if(r->f1 == NULL) return niv;
	int n1, n2, n3, n4;
    //apelam recursiv functia primind ca parametru un subarbore si nivelul curent + 1
	n1 = niv_min(r->f1, niv + 1);
	n2 = niv_min(r->f2, niv + 1);
    n3 = niv_min(r->f3, niv + 1);
	n4 = niv_min(r->f4, niv + 1);
    //functia returneaza valoarea minima dintre cele 4
	if(n1 <= n2 && n1 <= n3 && n1 <= n4)
		return n1;
    if(n2 <= n1 && n2 <= n3 && n2 <= n4)
		return n2;
    if(n3 <= n1 && n3 <= n2 && n3 <= n4)
		return n3;
	return n4;
}

//numarul de blocuri
//functie identica pentru acest task cu functia de numarare a frunzelor dintr-un arbore
int nr_blocuri(TArb r) {
    //daca arborele este vid atunci returneaza 0 (frunze = 0)
    if(r == NULL)
        return 0;
    else {
        //altfel daca tipul este 1 returneaza 1
        if(r->tip == 1)
            return 1;
        return nr_blocuri(r->f1) + nr_blocuri(r->f2) + nr_blocuri(r->f3) + nr_blocuri(r->f4);
    }
}

//functia de initializare a unei cozi
TCoada *InitCoada() {
    TCoada *c;
    c = (TCoada*)malloc(sizeof(TCoada));    //alocare spatiu
    if (!c) return NULL;
    c->inc = NULL;                          //legam cele doua celule la null
    c->sf = NULL;
    return c;
}

//functia de introducere in coada
void IntrCoada(TCoada *c, TArb arb)  /* adauga element la sfarsitul cozii */
{
    TListaC aux;
    aux = (TListaC) malloc (sizeof(TCelulaCoada));      /* aloca o noua celula */
    if(!aux) return;             /* alocare imposibila -> "esec" */

    //punem in celula alocata nodul
    aux->nod = arb;

    aux->urm = NULL;

    if (c->sf == NULL && c->inc == NULL)          /* coada nevida */
        c->inc = c->sf = aux;                   /* -> leaga celula dupa ultima din coada */
    else                              /* coada vida */
        c->sf->urm = aux;                    /* -> noua celula se afla la inceputul cozii */

    c->sf = aux;  	            /* actualizeaza sfarsitul cozii */
}

//distrugerea cozii
void DistrCoada(TCoada **c)
{
  TListaC p, aux;
  p = (*c)->inc;
  while(p)
  {
    aux = p;
    p = p->urm;
    //eliberare nod
    free(aux->nod);
    //eliberare celula coada
    free(aux);
  }
  free(*c);
  *c = NULL;
}

//distrugerea arborelui
void distruge(TArb r) 	/* functie auxiliara - distruge toate nodurile */
{
	if (!r) return;
	distruge (r->f1);
	distruge (r->f2);
    distruge (r->f3);
	distruge (r->f4);
	free (r);             /* distruge nodul radacina */
}

//parcurgearea arborelui pe latine si introducerea in fisierul out a valorilor cerute
void parc_latime(TArb arb, TCoada *c, unsigned int n, FILE *out) {
    //scriere in fisier dimensiunea
    fwrite(&n, 1, sizeof(unsigned int), out);

    //introducerea in coada a unui nod
    IntrCoada(c, arb);

    //parcurgerea cozii
    while(c->inc != NULL) {
        //scriere in fisier tipul nodului
        fwrite(&c->inc->nod->tip, 1, sizeof(unsigned char), out);
        

        if(c->inc->nod->tip == 1) {
            //daca tipul este 1 introducem si cele 3 valori pentru pixel
            fwrite(&c->inc->nod->red, 1, sizeof(unsigned char), out);
            fwrite(&c->inc->nod->green, 1, sizeof(unsigned char), out);
            fwrite(&c->inc->nod->blue, 1, sizeof(unsigned char), out);
            
        }

        //daca un nod parcurs mai are copii ii introducem si pe acestea la finalul cozii
        if(c->inc->nod->f1 != NULL) {
            IntrCoada(c, c->inc->nod->f1);
            IntrCoada(c, c->inc->nod->f2);
            IntrCoada(c, c->inc->nod->f3);
            IntrCoada(c, c->inc->nod->f4);
        }
        
        TListaC aux = c->inc;
        //mutam inceputul de coada pentru a trece la urmatorul nod
        c->inc = c->inc->urm;
        //eliberam memoria pentru celula prin care am trecut
        //in asa fel incat cand iesim din functie coada sa nu mai aiba niciun element
        free(aux);
    }
}

//construirea cozii ce contine nodurile arborelui cuaternar
//cu q parcurg coada (q = celula curenta)
void decomp_arb(FILE *in, TCoada *c, TListaC q) {
    unsigned char x;
    
    //citesc tipul nodului
    while(fread(&x, sizeof(unsigned char), 1, in)) {
        //il pun in nodul din celula curenta
        q->nod->tip = x;

        //daca este 0 aloc alte 4 noduri si 4 celule 
        // si le leg in asa fel incat acestea sa vina la sfarsitul cozii, mutand dupa fiecare legatura sfarsitul(c->sf)
        if(x == 0) {
            TArb arb1 = (TArb) malloc (sizeof(TNod));
            if(!arb1) return;
            TListaC aux = (TListaC) malloc (sizeof(TCelulaCoada));
            if(!aux) return;

            //aux->nod ii atasez noul nod alocat
            aux->nod = arb1;
            //il leg lui aux NULL
            aux->urm = NULL;
            //il leg pe aux la sfarsitul cozii
            c->sf->urm = aux;
            //ii fac legatura primului fiu cu noul arbore alocat
            q->nod->f1 = arb1;
            //mut sfarsitul
            c->sf = aux;

            arb1 = (TArb) malloc (sizeof(TNod));
            aux = (TListaC) malloc (sizeof(TCelulaCoada));
            aux->nod = arb1;
            aux->urm = NULL;
            q->nod->f2 = arb1;
            c->sf->urm = aux;
            c->sf = aux;

            arb1 = (TArb) malloc (sizeof(TNod));
            aux = (TListaC) malloc (sizeof(TCelulaCoada));
            aux->nod = arb1;
            aux->urm = NULL;
            c->sf->urm = aux;
            q->nod->f3 = arb1;
            c->sf = aux;

            arb1 = (TArb) malloc (sizeof(TNod));
            aux = (TListaC) malloc (sizeof(TCelulaCoada));
            aux->nod = arb1;
            aux->urm = NULL;
            c->sf->urm = aux;
            q->nod->f4 = arb1;
            c->sf = aux;
        
        }
        else {
            //daca este 1 inseamna ca am dat de o frunza si pot sa pun in arbore valorile pixelului

            fread(&x, sizeof(unsigned char), 1, in);
            q->nod->red = x;
            fread(&x, sizeof(unsigned char), 1, in);
            q->nod->green = x;
            fread(&x, sizeof(unsigned char), 1, in);
            q->nod->blue = x;
        }

        //ne mutam pe urmatoarea celula
        q = q->urm;
    }
}

//construirea matricel cu pixelii din arbore
void const_matrix(TArb arb, TMat **mat, unsigned int dim_imag, int x, int y) {
    int i, j;
    //daca tipul este 1(frunza) pun in matrice incepand de la x, y pana la x + dimensiunea imaginii, respectiv y + dimensiunea imaginii, valorile pixelilor
    if(arb->tip == 1) {
        for(i = x; i < x + dim_imag; i++)
            for(j = y; j < y + dim_imag; j++) {
                mat[i][j].red = arb->red;
                mat[i][j].green = arb->green;
                mat[i][j].blue = arb->blue;
            }
    }
    else {
        //altfel inseamna ca imaginea se imparte in 4 sferturi
        //si apelam recursiv functia doar ca fiecare fiu va alea alte coordonate de inceput
        dim_imag = dim_imag / 2;
        const_matrix(arb->f1, mat, dim_imag, x, y);
        const_matrix(arb->f2, mat, dim_imag, x, y + dim_imag);
        const_matrix(arb->f3, mat, dim_imag, x + dim_imag, y + dim_imag);
        const_matrix(arb->f4, mat, dim_imag, x + dim_imag, y);
    }
}