//GRECU STEFANIA 313CB

#include <stdio.h>

//structura pentru nod
typedef struct nod {
	unsigned char red, green, blue;
	unsigned char tip;
	struct nod *f1, *f2, *f3, *f4;
} TNod, *TArb;

//structura pentru matrice
typedef struct matrice {
	unsigned char red, green, blue;
} TMat;

unsigned long long formulaR(int x, int y, int size, TMat **mat);
unsigned long long formulaG(int x, int y, int size, TMat **mat);
unsigned long long formulaB(int x, int y, int size, TMat **mat);
unsigned long long formulaMEAN (int x, int y, int size, TMat **mat, unsigned char red, unsigned char green, unsigned char blue);

TArb construire(int x, int y, int factor, TMat **mat, int n, int max);

void parcurge(TArb r);

int NrNiv(TArb r);
int niv_min(TArb r, int niv);
int nr_blocuri(TArb r);

//structura pentru o  celiula din coada
typedef struct celcd{
	TArb nod;
    struct celcd *urm;
} TCelulaCoada, *TListaC;

//structura pentru coada
typedef struct coada{
    TListaC inc, sf;
} TCoada;

void parc_latime(TArb arb, TCoada *c, unsigned int n, FILE *out);
TCoada *InitCoada();
void IntrCoada(TCoada *c, TArb arb);
void DistrCoada(TCoada **c);

void distruge(TArb r);

void decomp_arb(FILE *in, TCoada *c, TListaC q);

void const_matrix(TArb arb, TMat **mat, unsigned int dim_imag, int x, int y);