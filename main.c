//GRECU STEFANIA 313CB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int main(int arg, char *argv[])
{
    int i, j;
    if(strcmp("-d", argv[1]) != 0) {

        //cerentele 1 + 2

        //deschiderea fisierului
        FILE *in = fopen(argv[3], "rb");
        if (in == NULL) {
            fprintf(stderr, "ERROR: Can't open file");
            return -1;
        }

        unsigned int n, m, max;
        unsigned char x;
        char a[2];

        //citim din fisier cele 4 valori
        fscanf(in, "%s\n", a);
        fscanf(in, "%u %u\n", &n, &m);
        fscanf(in, "%u", &max);
        fseek(in, sizeof(unsigned char), SEEK_CUR);

        //alocam spatiu pentru matrice
        TMat **mat = malloc(n * sizeof(unsigned char **));
        if(!mat) {
            return 0;
        }

        for (i = 0; i < n; i++) {
            mat[i] = calloc (m, sizeof(unsigned char *));
            if (!mat[i])        //eliberare memorie alocata pana atunci in cazul in care mat[i] nu se aloca
            {
                for(j = 0; j < i; j++)
                free(mat[j]);
                free(mat);
                return 0;
            }
        }

        //citim din fisier matricea de pixeli
        for (i = 0; i < n; i++) {
            for(j = 0; j < m; j++)
            {
                fread(&x, sizeof(unsigned char), 1, in);
                mat[i][j].red = x;

                fread(&x, sizeof(unsigned char), 1, in);
                mat[i][j].green = x;

                fread(&x, sizeof(unsigned char), 1, in);
                mat[i][j].blue = x;
            }
        }

        TArb arb = NULL;

        //construire arbore 
        arb = construire(0, 0, atoi(argv[2]), mat, n, max);

        if(strcmp("-c1", argv[1]) == 0) {
            //cerinta 1

            //deschidere fisier
            FILE *out = fopen(argv[4], "wt");
            if (out == NULL) {
                fprintf(stderr, "ERROR: Can't open file");
                return -1;
            }

            //dimensiunea celui mai mare bloc este dimensiunea imaginii / 2 la puterea numarului nivelului min
            int dim = n;
            printf("%d\n", niv_min(arb, 0));
            for (i = 0; i < niv_min(arb, 0); i++)
                dim = dim / 2;

            //introducere in fisier a valorilor cerute
            fprintf(out, "%d\n%d\n%d\n", NrNiv(arb), nr_blocuri(arb), dim);
            fclose(out);
        }
        else {
            //cerinta 2

            //deschidere fisier
            FILE *out = fopen(argv[4], "wb");
            if (out == NULL) {
                fprintf(stderr, "ERROR: Can't open file");
                return -1;
            }

            //declarare coada si initializare
            TCoada *c;
            c = InitCoada();

            //parcurgerea pe latime si introducerea in fisier a tipului de noduri si valorilor pixelilor frunzelor
            parc_latime(arb, c, n, out);

            //eliberarea memoriei ocupare de coada
            DistrCoada(&c);

            fclose(out);
        }

        //distrugerea arborelui
        distruge(arb);

        //eliberarea memoriei alocate pentru matrice
        for(j = 0; j < n; j++)
                free(mat[j]);
                free(mat);

        fclose(in);
    }
    else {
        //cerinta 3

        //deschiderea fisierelor
        FILE *in = fopen(argv[2], "rb");
        if (in == NULL) {
            fprintf(stderr, "ERROR: Can't open file");
            return -1;
        }
        FILE *out = fopen(argv[3], "wb");
        if (out == NULL) {
            fprintf(stderr, "ERROR: Can't open file");
            return -1;
        }

        //citirea din fisierul de intrare a dimensiunii imaginii
        unsigned int dim_img;
        fread(&dim_img, sizeof(unsigned int), 1, in);

        TCoada *c;
        c = InitCoada();

        //acolare memorie arbore si o celula tipul TListaC
        TArb arb = (TArb) calloc (1, sizeof(TNod));
        if(!arb) return 0;
        TListaC aux = (TListaC) calloc (1, sizeof(TCelulaCoada));
        if(!aux) return 0; 

        //legarea fiilor arborelui cu NULL
        arb->f1 = NULL;
        arb->f2 = NULL;
        arb->f3 = NULL;
        arb->f4 = NULL;

        //nodului din celula alocata ii atribuim valoarea arborelui alocat
        aux->nod = arb;

        //legarea inteputului si sfarsitului cozii la celula alocata
        c->inc = aux;
        c->sf = aux;

        //cu ajutorul variabilei q vom parcurge coada
        TListaC q = c->inc;

        //crearea cozii si a arborelui cu informatiile din fisierul de intrare
        decomp_arb(in, c, q);
        //parcurge(c->inc->nod);

        //alocarea memorie pentru matrice
        TMat **mat = malloc(dim_img * sizeof(unsigned char **));
        if(!mat) {
            return 0;
        }

        for (i = 0; i < dim_img; i++) {
            mat[i] = calloc (dim_img, sizeof(unsigned char *));
            if (!mat[i])
            {
                for(j = 0; j < i; j++)
                free(mat[j]);
                free(mat);
                return 0;
            }
        }

        //construirea matricei cu informatiile din arborele ce are radacina in prima celula din coada
        const_matrix(c->inc->nod, mat, dim_img, 0, 0);

        //introducerea in fisier a informatiile ce ajuta la contruirea fisierului PPM
        fprintf(out, "P6\n%u %u\n255\n", dim_img, dim_img);

        //introducerea in fisier a matricei
        for(i = 0; i < dim_img; i++)
            for(j = 0; j < dim_img; j++) {
                fwrite(&mat[i][j].red, 1, sizeof(unsigned char), out);
                fwrite(&mat[i][j].green, 1, sizeof(unsigned char), out);
                fwrite(&mat[i][j].blue, 1, sizeof(unsigned char), out);
            }

        //eliberarea memoriei alocate cozii
        DistrCoada(&c);

        //eliberarea matricei
        for(j = 0; j < dim_img; j++)
                free(mat[j]);
                free(mat);

        fclose(in);
        fclose(out);

    }

    return 0;
}