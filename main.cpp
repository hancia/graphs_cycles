#include <iostream>
#include<ctime>
#include<cstdlib>
#include <fstream>
#include <string.h>
#include <vector>
#include <windows.h>
using namespace std;
int n=24;
double d=0.2;

struct lista
{
    int nazwa;
    lista* next;
    lista *prev;
};
struct wierzcholek
{
    int nazwa;
    lista *sasiad;
    bool visited;
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
void generuj_macierz(int **M, double gestosc,int &krawedzie)
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
    for(int i=0; i<n; i++)
    {
        int stp=0;
        for(int x=0;x<n;x++) if(M[i][x]) stp++;
        if(stp%2!=0)
        {
            int j=rand()%(n-i-1)+i+1;
                if(M[i][j])
                {
                    M[i][j]=0;
                    M[j][i]=0;
                    krawedzie--;
                }
                else
                {
                    M[i][j]=1;
                    M[j][i]=1;
                    krawedzie++;

                }
        }
    }
    if(!eulerowski(M))
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
        generuj_macierz(M,d,krawedzie);
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
}

void wprowadz_nast(wierzcholek T[])
{
    ifstream plik;
    plik.open("graf.txt");
    for(int i=0; i<n; i++)
    {
        int a;
        plik>>a;
        for(int j=0;j<a;j++)
        {
            int m;
            plik>>m;
            m=m-1;
                lista *nowy= new lista;
                nowy->nazwa=m;
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
    plik.close();
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
void usun(wierzcholek T[],lista *temp, wierzcholek &obecny)
{
    lista *pom2=T[temp->nazwa].sasiad;
    if(pom2!=NULL)
    {
        while(pom2->nazwa!=obecny.nazwa) pom2=pom2->next;
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
void dfs(wierzcholek T[], wierzcholek &obecny, lista *temp)
{
    while(obecny.sasiad!=NULL)
    {
        usun(T,temp,obecny);
        lista *pom=temp;
        temp=temp->next;
        if(temp==NULL) obecny.sasiad=NULL;
        else
        {
            temp->prev=NULL;
            obecny.sasiad=temp;
        }
        dfs(T,T[pom->nazwa],T[pom->nazwa].sasiad);
    }
    //cout<<obecny.nazwa+1<<"->";
}
void szukaj_euler(wierzcholek T[], lista *obecny)
{
    wierzcholek *pom = new wierzcholek[n];
    memcpy(pom,T,n * sizeof(wierzcholek));
    dfs(pom,pom[0],pom[0].sasiad);
    delete []pom;
}
void print_vector (vector <int> tab)
{
    cout<<"\nCYKL:\n";
    for(int i=0;i<tab.size();i++)
        cout<<tab[i]<<" ";
    cout<<endl;
}
int flaga_znalezienia = 0;
void szukaj_hamilton(wierzcholek T[],wierzcholek &V, lista *obecny, int &odwiedzone,vector<int>&cykle,int znajdz_jeden=0)
{
    if(!(flaga_znalezienia && znajdz_jeden))
    {
        V.visited=1;
        odwiedzone++;
        cykle.push_back(V.nazwa+1);
        if(odwiedzone==n)
        {
            int flaga=0;
            while(obecny!=NULL)
            {
                if(obecny->nazwa==0)
                {
                    flaga=1;
                    break;
                }
                else obecny=obecny->next;
            }
            if(flaga==1)
            {
                //print_vector(cykle);
                flaga_znalezienia=1;
            }
        }
        while(obecny!=NULL)
        {
            if(!T[obecny->nazwa].visited)
            {
                szukaj_hamilton(T,T[obecny->nazwa],T[obecny->nazwa].sasiad,odwiedzone,cykle,znajdz_jeden);
            }
            obecny=obecny->next;
        }
    V.visited=0;
    cykle.pop_back();
    odwiedzone--;
    }
}
int main()
{
    srand(time(NULL));
    int krawedzie=0,spojny=0,odwiedzone=0, krok=0;
    lista *vert=NULL;
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
        generuj_macierz(M,d,krawedzie);
        generuj_nast(T,M);
        //wprowadz_nast(T);
        //wyswietl_nast(T);
        cout<<endl;

        clock_t start;

        vector<int> cykle;
        start = clock();
        szukaj_hamilton(T,T[0],T[0].sasiad,odwiedzone,cykle,1);
        cout<<"czas dla jednego cyklu hamiltona: "<<(long double)(clock()-start)/CLOCKS_PER_SEC<<endl;
        cykle.clear();

        odwiedzone=0;
        start = clock();
        szukaj_hamilton(T,T[0],T[0].sasiad,odwiedzone,cykle);
        cout<<"czas dla wszystkich cykli hamiltona: "<<(long double)(clock()-start)/CLOCKS_PER_SEC<<endl;

        start = clock();
        szukaj_euler(T,vert);
        cout<<"czas dla eulera: "<<(long double)(clock()-start)/CLOCKS_PER_SEC<<endl;

    }
    delete []T;
    for(int i=0; i<n; i++)
        delete []M[i];
    delete []M;
    return 0;
}

