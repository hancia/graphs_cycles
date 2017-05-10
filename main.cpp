#include <iostream>
#include<ctime>
#include<cstdlib>
#include <fstream>
using namespace std;
int n=20;
double d=0.6;

struct lista
{
    int nazwa;
    lista* next;
    lista *prev;
    lista *mirror;
};
struct wierzcholek
{
    int nazwa;
    lista *sasiad;
};

bool eulerowski(int **M)
{
    int flaga=0,b;
    for(int i=0; i<n; i++)
    {
        flaga=0;
        for(int j=0; j<n; j++) if(M[i][j]==1)flaga++;
        if(flaga%2!=0||flaga<2) return false;
    }
    return true;
}

//generuje macierz
void generuj_macierz(int **M, double gestosc,int &krawedzie, int &spojny)
{
    gestosc=gestosc*n*(n-1)/2;
    int a,b,c;
    while(krawedzie>=gestosc||krawedzie%2!=0)
    {
        a=rand()%n;
        b=rand()%n;
        M[a][b]=0;
        M[b][a]=0;
        krawedzie--;
    }
    if(!eulerowski(M)) spojny=0;
    else spojny=1;
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
                nowy->prev=NULL;
                if(T[i].sasiad==NULL)
                {
                    nowy->next=NULL;
                }
                else
                {
                    nowy->next=T[i].sasiad;
                    T[i].sasiad->prev=nowy;
                }
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
void usun(wierzcholek T[],lista *temp, wierzcholek &stos)
{
    lista *pom2=T[temp->nazwa].sasiad;
    if(pom2!=NULL)
    {
        while(pom2->nazwa!=stos.nazwa) pom2=pom2->next;
        if(pom2->prev!=NULL&&pom2->next!=NULL)
        {
            pom2->prev->next=pom2->next;
            pom2->next->prev=pom2->prev;
        }
        else
        {
            if(pom2->next==NULL&&pom2->prev!=NULL)
                {
                    pom2->prev->next=NULL;
                }
            else
            {
                if(pom2->next!=NULL)
                {
                    T[temp->nazwa].sasiad=T[temp->nazwa].sasiad->next;
                    T[temp->nazwa].sasiad->prev=NULL;
                }
                else
                {
                    T[temp->nazwa].sasiad=NULL;
                }
            }
        }
    }
}
void dfs(wierzcholek T[], wierzcholek &stos, lista *temp)
{
    while(temp!=NULL)
    {
        usun(T,temp,stos);
        lista *pom=temp;
        temp=temp->next;
        if(temp==NULL) stos.sasiad=NULL;
        else
        {
            temp->prev=NULL;
            stos.sasiad=temp;
        }
        dfs(T,T[pom->nazwa],T[pom->nazwa].sasiad);
    }
   cout<<stos.nazwa+1<<"->";
}
void szukaj_euler(wierzcholek T[], lista *stos)
{
    wierzcholek *pom = new wierzcholek[n];
    pom=T;
    dfs(pom,pom[0],pom[0].sasiad);
    delete []pom;
}

int main()
{
    srand(time(NULL));
    int krawedzie=0,spojny=0,odwiedzone=0, krok=0;
    lista *stos=NULL;
    wierzcholek *T = new wierzcholek[n];
    for(int i=0; i<n; i++)
    {
        T[i].nazwa=i;
        T[i].sasiad=NULL;
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
    if(d>(double)2/n)
    {
        while(spojny!=1)
            {
                for(int i=0;i<n;i++)
                {
                    for(int j=0;j<n;j++)
                        {
                            if(M[i][j]!=1&&i!=j)
                            {
                                M[i][j]=1;
                                krawedzie++;
                            }
                        }
                }
                generuj_macierz(M,d,krawedzie,spojny);
            }
    }
    if(spojny==0) cout<<"dla takiej gestosci i losci wierzcholkow nie istnieje graf eulerowski!"<<endl;
    else
    {
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
                cout<<M[i][j]<<" ";
            cout<<endl;
        }
        generuj_nast(T,M);
        wyswietl_nast(T);
        szukaj_euler(T,stos);
    }
    delete []T;
    for(int i=0; i<n; i++)
        delete []M[i];
    delete []M;
    return 0;
}

