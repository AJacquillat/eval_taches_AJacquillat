#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>
#include <stdexcept>
using namespace std;

///explications sur les choix de réponses attendues 
// Q <- quit         
// W <- Write 
// S <- Show 
// E <- Edit         editer une tâche (pourcentage, avancement, sous taches ok?)


//// Nous interdirons à l'utilisateur d'utiliser le # qui sera utilisé par le programme pour 
//// structurer le fichier taches.txt dans lequel nous incoprorons les taches 


// objectif: créer un fichier de tâches
// ajouter une tache au fichier & structurer le fichier 


///Pour les commentaires nous créons une fonction insert qui inséère sans écraser...



char* date_heure()
{       
    chrono::system_clock::time_point today = chrono::system_clock::now();
    std::time_t tt;
    tt = chrono::system_clock::to_time_t ( today );
    return(ctime(&tt));
}

int new_id()
{
    ///La première étape consiste à tester la bonne ouverture du fichier     
    fstream test_file("test.txt", ios::in| ios::out|ios::ate);
    test_file.seekp(0,ios::end);    
    int pos_end=test_file.tellp();

    ///Cas ou le fichier n'existe pas ou bien ne peut pas s'ouvrir
    //S'il n'existe pas, nous essayons de le créer
    if (pos_end ==-1) 
    {
        ofstream ofile("test.txt"); //cela crée le fichier
        ofile.close(); //cela ferme l'objet ofile
    }
    test_file.close();


    /// On recommence tout pour laisser une chance au fichier créé de s'ouvrir 
    fstream file("test.txt", ios::in| ios::out|ios::ate);
    file.seekp(0,ios::end);    
    pos_end=file.tellp();

    //Cas ou le fichier ne peut pas s'ouvrir
    if (pos_end ==-1) 
    {
        cout<<"probleme dans l'ouverture du fichier, merci de mettre les mains dans le cambouis\nVous créerez alors un fichier test.txt dans le même folder que l'executable\nMerci\n";
        return(-1);
    }
    else if(pos_end<2)//alors le fichier existe mais est vide ou en tout cas n'a pas de tache écrite, l'id est donc 0
    {
        return(0);
    } 
    else {
        //on se place à la fin du fichier texte
        //mais on veut trouver toute la ligne, si c'es( la 12è tâche, voir 12 et pas 2)
        //on met le lecteur à la fin, il faut qu'il lise la dernière ligne toute entière
        file.seekg(pos_end-1,ios::beg); 
        int pos = pos_end;

        //on récupère le caractère de bout de fichier
        char ch; 
        file.get(ch);

        //Tant que ce caractère n'est pas le # qui marque le début de chaque sous section,
        //on recule dans le fichier 
        while( ch != '#')
        {
            pos=pos-1; 
            file.seekg(pos,ios::beg);
            file.get(ch);
        };

        //Nous récupérons toute la ligne
        string last_id_string;
        getline(file, last_id_string);

        cout<<last_id_string<<endl;

        //on utilise un classe toute faite pour 
        //convertir la string en int 
        stringstream geek(last_id_string); 
        int last_id_int; 
        geek >> last_id_int;

        //Nous renvoyons l'int qui doit être le numéro de la tache  
        return( last_id_int+1 );
    }
}

void new_task()
{
    fstream file("taches.txt", ios::in|ios::out|ios::ate);  //on ouvre un fichier pour écrire et lire dedans 
        file.seekp(0, ios::end);
        std::cout << "Ecrivez du texte. Ecrivez # puis pressez retour-arrière pour quitter:\n";
        char ch; //on sépare la selction en différents char pour tester à chacun si il y a un pb
        do {
            ch = std::cin.get();
            file.put(ch);
        } while (ch!='#');

        file.close(); //on ferme le fichier 
}

void dispatch(string entree)
{
    ////////// Si l'utilisateur demande à écrire une nouvelle tache 
    if (entree == "W")
    {
        new_task();
    }

    ////////// Si l'utilisateur demande à lire une (plusieurs) tache(s)

    ////////// Si l'utilisateur demande à modifier une tache 
    // Nous affichons l' id_tache et le nom_tache  de toutes les taches

    // L'utilisateur choisit la tache qu'il veut modifier par son id_tache 

    // On affiche alors l'état de toutes les composantes  
};



int main()
{
    int on=1; //la variable d'état du système 1: allumé 0: éteint 
    while (on==1) 
    {
        string entree;
        cout<<"tappez la lettre correspondant à l'action désirée : \n Q pour quitter \n W pour écrire une tache \n S pour montrer une(des) tache(s)\n E pour éditer une tache";
        cin>>entree;
        if(entree=="Q"){on=0;}
        else{dispatch( entree );}
    }
    return 0;
}