#include <stdio.h>
#include <stdlib.h>
#include "mersenne.c"
#include <time.h>

double functie(double *p, int k)  //Calculeaza functia de repartitie in functie de k, returnat de functia simulare
{
    int i;
    double s=0;
    for(i=0;i<=k;i++)
      s=s+p[i];
    return s;
}
void eroare()  //Afiseaza mesaj de eroare in cazul in care alocarea dinamica nu este efectuata corect
{
    printf("Eroare alocare dinamica a memoriei.");
    printf("\n");
}
int simulare (double *p, int nrm) //Algoritmul de generare a unui numar pseudo-aleator, ce returneaza masa pe care o alege un student
{
    int k=0;
    double u;
    u=genrand_real2();
    while(u>functie(p,k) && k<nrm)
        k++;
    return k+1;
}
void distributie(double alfa, int *nr, int nrm, int k, double *p ) //Calculeaza distributia de probabilitate pentru fiecare student separat
{
    int i;
    for(i=0;i<nrm;i++)
    p[i]=nr[i]/(alfa+k);
}
void verificare (int nrm, int *nr, int n)// Verificare daca suma studentilor atribuiti la mese este egala cu numarul lor initial si afisarea unui mesaj aferent
{
    int s=0, i;
    for(i=0;i<nrm;i++)
     s=s+nr[i];
    if(s==n) printf("Simularea s-a realizat corect.");
}

int main()
{
    typedef struct Student
    {
        int nrcrt;
        char *NumePrenume;
        int masa;
    }Student;

    Student  *S;
    FILE *f;
    char *line;
    int i, nrm, *nr, j, n=175,  m=1;
    double *p, alfa=0.9;
    time_t secunde;
    secunde=time(NULL);
    init_genrand(secunde);
    p = (double *) malloc(n* sizeof(double)); // Am alocat dinamic memorie pentru p, care va retine distributiile de probabilitate
    if (!p)
      eroare();
    line = (char *) malloc(50* sizeof(char));
    if (!line)
      eroare();
    f=fopen("studenti.txt", "r");
    if(!f)
      eroare();
    S=(Student *)malloc(n*sizeof(Student)); // am alocat memorie dinamic pentru structura Student
    if (!S)
      eroare();
    i=0;
    while(fgets(line, 50, f)!=NULL)
    {
        strcpy(line+strlen(line)-1,line+strlen(line)); //Stergerea elementului /n de la sfarsitul numelui
        S[i].nrcrt=i;
        S[i].NumePrenume=(char *) malloc ((strlen(line)+1)*sizeof(char));
        if (!S[i].NumePrenume)
        {
            printf("Eroare alocare dinamica a memoriei.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(S[i].NumePrenume, line);
        i++;
    }
    nr = (int *) malloc(1 * sizeof(int));
    if (!nr)
      eroare();
    nrm=0;
    for(i=0;i<n;i++)
    {
        distributie(alfa, nr,nrm,i,p);
        int z;
        z=simulare(p,nrm);
        if(z>nrm)
        {
            m++;
            nr = (int *) realloc(nr, m * sizeof(int));
            nr[nrm]=1;
            S[i].masa=z;
            nrm++;
        }

        else
        {
            nr[z-1]++;
            S[i].masa=z;
        }
    }
    for(i=0;i<nrm;i++)
    {
        printf("Masa %d: \nNumarul de persoane de la masa= %d \nStudentii de la masa: \n", i+1, nr[i]);
        for(j=0;j<n;j++)
        {
            if(S[j].masa==(i+1))
            printf("%-4d %-30s \n", S[j].nrcrt+1, S[j].NumePrenume);
        }
        printf("\n");
    }
    verificare(nrm,nr,n);
    free(p);
    free(S);
    free(nr);
    free(line);
    fclose(f);
    return 0;
}
