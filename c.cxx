#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

//une fonction qui attribue les tâches en fonction de l'entrée cin reçue 


class tache
{
    public:
    int on; //la variable d'état du système, ie si on=1, on veut que le programme tourne, si on=0, on veut quitter le programme 
    virtual void dispatch(string entree){}
};


void dispatch(string entree)
{
    cout<<"in dispatch"<<endl;
};



int main()
{
    int on=1; //la variable d'état du système 1: allumé 0: éteint 
    while (on==1) 
    {
        string entree;
        cin>>entree;
        if(entree=="Q"){on=0;}
        else{dispatch( entree );          
    };
    return 0;
}