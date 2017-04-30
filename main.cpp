#include <iostream>
#include<ctime>
#include<cstdlib>
#include <fstream>
using namespace std;
int n=30;
double d=0.2;

struct lista
{
    int nazwa;
    lista* next;
    lista *mirror;
};
struct wierzcholek
{
    lista *sasiad;
    bool visited;
};

//generuje macierz
void generuj_macierz(int **M, double gestosc,int krawedzie, int &spojny)
{
    gestosc=gestosc*n*(n-1)/2;
    int a,b, flaga=0;
    while(krawedzie>=gestosc)
    {
        int current=krawedzie;
        while(current-2!=krawedzie)
        {
            flaga=0;
            a=rand()%n;
            b=rand()%n;
            if(d>2/n)
            {
                for(int j=0; j<n; j++) if(M[a][j]==1&&M[b][j]==1) flaga++;
            }
            else
            {
                flaga=2;
                spojny=0;
            }
            if(flaga>1)
            {
                M[a][b]=0;
                M[b][a]=0;
                krawedzie--;
            }
        }
    }
}

//generuje liste nastepnikow na podstawie macierzy
void generuj_nast(wierzcholek T[], int **M)
{
    for(int i=0; i<n; i++)
    {
        for(int m=0; m<n; m++)
        {
            if(M[i][m]==1)
            {
                lista *nowy= new lista;
                nowy->nazwa=m;
                nowy->mirror=NULL;
                if(T[i].sasiad==NULL) nowy->next=NULL;
                else nowy->next=T[i].sasiad;
                T[i].sasiad=nowy;
            }
        }
    }
    for(int i=0; i<n; i++)
    {
        lista *current, *szukaj;
        current=T[i].sasiad;
        while(current!=NULL)
        {
            szukaj=T[current->nazwa].sasiad;
            while(szukaj!=NULL&&szukaj->nazwa!=current->nazwa) szukaj=szukaj->next;
            if(szukaj!=NULL&&szukaj->nazwa==current->nazwa) current->mirror=szukaj;
            current=current->next;
        }
    }
}

 void wyswietl_nast(wierzcholek T[])
 {
     for(int i=0; i<n; i++)
     {
        lista *obecny=T[i].sasiad;
        cout<<endl<<"wierzcholek "<<i+1<<" i lista jego sasiadow: ";
         while(obecny!=NULL)
         {
             cout<<obecny->nazwa+1<<" ";
             obecny=obecny->next;
         }
     }
}

int main()
{
    srand(time(NULL));
    int krawedzie=0,spojny=1;
    wierzcholek *T = new wierzcholek[n];
    for(int i=0; i<n; i++)
    {
        T[i].sasiad=NULL;
        T[i].visited=0;
    }
    int **M=new int *[n];
    for(int i=0; i<n; i++)
    {
        M[i]= new int [n];
        for(int j=0; j<n; j++)
            if(i!=j)
            {
                M[i][j]=1;
                krawedzie++;
            }
            else
            M[i][j]=0;
    }
    generuj_macierz(M,d,krawedzie,spojny);
    if(spojny==0) cout<<"dla takiej gestosci i losci wierzcholkow nie istnieje graf eulerowski!"<<endl;
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
            cout<<M[i][j]<<" ";
        cout<<endl;
    }
    generuj_nast(T,M);
    wyswietl_nast(T);
    delete []T;
    for(int i=0; i<n; i++)
        delete []M[i];
    delete []M;
    return 0;
}

