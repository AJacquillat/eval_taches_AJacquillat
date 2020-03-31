#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>
#include <sstream>
#include <stdexcept>

using namespace std;

///explications sur les choix de réponses attendues 
// Q <- quit         
// W <- Write 
// S <- Show 
// E <- Edit         editer une tâche (pourcentage, avancement, sous taches ok?)


//// Nous interdirons à l'utilisateur d'utiliser le # qui sera utilisé par le programme pour 
//// structurer le fichier taches.txt dans lequel nous incoprorons les taches 

////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// FONCTIONS DE CONVERSION   //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


int string_to_int(string str)
{
    stringstream geek(str); 
    int n; 
    geek >> n;
    return n;
}

string int_to_string(int n)
{
    stringstream ss;
    ss << n;
    string str = ss.str();

    return str;
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////  FONCTIONS POUR L'ECRITURE ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

//fonction qui donne la date et l'heure actuelles
char* date_heure()
{       
    chrono::system_clock::time_point today = chrono::system_clock::now();
    std::time_t tt;
    tt = chrono::system_clock::to_time_t ( today );
    return(ctime(&tt));
}

///fonction qui donne l'id de la tache à créer
int new_id()
{
    ///La première étape consiste à tester la bonne ouverture du fichier     
    fstream test_file("c.txt", ios::in| ios::out|ios::ate);
    test_file.seekp(0,ios::end);    
    int pos_end=test_file.tellp();

    ///Cas ou le fichier n'existe pas ou bien ne peut pas s'ouvrir
    //S'il n'existe pas, nous essayons de le créer
    if (pos_end ==-1) 
    {
        ofstream ofile("c.txt"); //cela crée le fichier
        ofile.close(); //cela ferme l'objet ofile
    }
    test_file.close();


    /// On recommence tout pour laisser une chance au fichier créé de s'ouvrir 
    fstream file("c.txt", ios::in| ios::out|ios::ate);
    file.seekp(0,ios::end);    
    pos_end=file.tellp();

    //Cas ou le fichier ne peut pas s'ouvrir, on relève l'erreur
    if (pos_end ==-1) 
        throw logic_error("probleme dans l'ouverture du fichier, merci de mettre les mains dans le cambouis\nVous créerez alors un fichier c.txt dans le même folder que l'executable\nMerci\n");

    if(pos_end<2)//alors le fichier existe mais est vide ou en tout cas n'a pas de tache écrite, l'id est donc 0
    {
        return(0);
    } 
    else {
        //on se place à la fin du fichier texte
        //mais on veut trouver toute la ligne, si c'es( la 12è tâche, voir 12 et pas 2)
        //on met le lecteur à la fin, il faut qu'il lise la dernière ligne toute entière
        file.seekg(pos_end-2,ios::beg); 
        int pos = pos_end-2;

        //on récupère le caractère de bout de fichier (le dernier choffre du nombre)
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

        pos=pos+1;
        file.seekg(pos, ios::beg);
        file.get(ch);

        //Nous récupérons toute la ligne à l'exception du dernier #
        string last_id_string;
        getline(file, last_id_string, '#');

        //on utilise un classe toute faite pour 
        //convertir la string en int 
        stringstream geek(last_id_string); 
        int last_id_int; 
        geek >> last_id_int;
        file.close();

        //Nous renvoyons l'int qui doit être le numéro de la tache  
        return( last_id_int+1 );
    }
    
}

//cette fonction sert àn capturer ce qui est écrit par l'utilisateur 
string saisie()
{
    string buffer; //l'élément qui va stocker la saisie
    std::cout << "Ecrivez # puis pressez retour-arriere pour enregistrer le texte.\n";
    char ch; //on sépare la selection en différents char pour s'assurer qu'aucun des massages ne conteint le caractère interdit #
    do {
        ch = std::cin.get();
        buffer.append(1,ch); //on ajout la ch à la fin du buffer
    } while (ch!='#');

    //on supprime juste le # final
    buffer.pop_back(); 

    return buffer;
}

// même chose que saisie mais s'assure que le résultat communiqué est 
// un entier entre 1 et 3
string saisie_prio()
{
    int entree;
    cout<<"entrez un entier entre 1 et 3\n";
    cin >> entree; 
    while (cin.fail() or entree>3 or entree<1)
    {
        cout<<"Entree non conforme\nRecommencez\n";
        std::cin.clear();
        std::cin.ignore(256,'\n');
        cin >> entree;        
    };
    string str=int_to_string(entree);
    return str;
}

/// Fonction pour écrire une tâche 
void new_task()
{
    // Pour éviter d'avoir un problème en cas de crash pendant la défnition d'une tache
    // et de récupérer un fichier pas aux normes, 
    // nous stockons toutes les réponses dans un buffer. 
    //C'est au dernier moment seulement que nous ajouterons ce buffer au fichier texte 

    // Pour comprendre le choix de mise en forme du buffer, se réferer au fichier format_type.txt
    string buffer;

    ////////// Ecriture de l'id_tache dans le buffer 
    int id_tache = new_id(); //notons que le fichier est forcément ouvert car nous avons jeté l'exception dans new_id()
    
    //assurons nous quand même que l'on arrive à ouvrir le fichier avant de tout faire pour rien
    fstream file("c.txt", ios::in | ios::out |ios::ate);
    if(!file.is_open())
        throw logic_error("Fichier ne s'ouvre pas...");
    
    buffer.append("\n\n##");
    buffer.append(to_string(id_tache));
    buffer.append("\n");
    


    ////////// Titre de la tache
    //on dit a l'utilisateur ce qu'il doit faire 
    cout<<"\n    Saisissez le titre de la nouvelle tache.\n";
    string buffer_tempo=saisie();
    //on a maintenant dans buffer_tempo une string qui contient la saisie     
    // ajoutons dans le gros buffer le contenu du buffer_tempo selon la syntaxe 
    //précisée dans le fichier format_type.txt
    buffer=buffer+"#"+buffer_tempo+"\n";

    //vidons maintenant le buffer_tempo
    buffer_tempo.clear();




    /////////// description de la tache 
    //on dit a l'utilisateur ce qu'il doit faire 
    cout<<"\n   Saisissez la description de la nouvelle tache.\n";
    buffer_tempo=saisie();
    //on a maintenant dans buffer_tempo une string qui contient la saisie     
    // ajoutons dans le gros buffer le contenu du buffer_tempo selon la syntaxe 
    //précisée dans le fichier format_type.txt
    buffer=buffer+"#"+buffer_tempo.substr(1,buffer.length()-1)+"\n";

    //vidons maintenant le buffer_tempo
    buffer_tempo.clear();




    ///////////// date de creation de tache
    buffer=buffer+"#"+date_heure();


    //////////// date de fin de tache
    // l'utilisateur modifie lui même au format habituel 
    // on peut imaginer automatiser cela plus tard quand avancée passe à 100% ou quand le status passe à D (done)
    // on met le reste de ces informations liées entre elles à jour, 
    // en attendant, on réserve de la place pour cette date en remplissant une 'date' fausse
    buffer=buffer+"#"+"000 000 00 00:00:00 0000"+"\n";



    ///////// On ajoute un status, par défaut la tache est "open" ('O'), à l'utilisateur de la modifier quand "done" ('D')
    buffer=buffer+"#"+"O"+"\n";

    ///////// On ajoute une priorité
    //on dit a l'utilisateur ce qu'il doit faire 
    cout<<"\n   Saisissez la priorité (1<2<3) de la nouvelle tache.\n";
    buffer_tempo=saisie_prio();
    //on a maintenant dans buffer_tempo une string qui contient la saisie     
    // ajoutons dans le gros buffer le contenu du buffer_tempo selon la syntaxe 
    //précisée dans le fichier format_type.txt
    buffer=buffer+"#"+buffer_tempo+"\n";
    //vidons maintenant le buffer_tempo
    buffer_tempo.clear();


    ////////// on ajoute l'espace commentaires, vide pour l'instant
    buffer=buffer+"#"+"\n";

    ////////// on ajoute l'espace sous taches, à remplir à l'ouverture de la tache
    //Laissons l'utilisateur remplir. Cette zone n'étant pas faite pour être modifiée
    // nous nous passerons de séparateur entre sous taches, laissant à l'utilisateur 
    // la responsabilité de bien organiser son espace 
    cout<<"sous taches? A vous de les organiser toutes d'un coup.\n";
    buffer_tempo=saisie();
    buffer=buffer+"#"+buffer_tempo.substr(1,buffer.length()-1)+"\n";
    buffer_tempo.clear();




    ////////// On répète une dernière fois l'id_tache, ce qui clos l'espace dédié à la tache
    buffer.append("#");
    buffer.append(to_string(id_tache));
    buffer.append("#");


    ///////////// on ajoute seulement maintenant les informations au fichier texte 
    file.seekp(0, ios::end);
    file<<buffer;
    file.close(); //on ferme le fichier 
}






///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////  FONCTIONS POUR LA LECTURE ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



////////pour une tache donnée
//1&2è # : numéro de la tache
// 3è: titre
// 4è: description 
// 5è: date début
// 6è: date fin
// 7è: état de la tache ('O' open ou 'D' done)
// 8è: priorité (1<2<3)
// 9è: Commentaires (séparateur &)
// 10è : sous taches (séparateur &)
// 11è: répétition du numéro de tache 
// 12è: pour clore la tache 

///////// L'idée est de parcourir une fois pour toute la liste des taches, 
// les positions seront bonnes pdt tt l'exploration dans S (Select)
// Cette fonction pourra servir à nouveau pour les modifications.

// Chaque id_ligne correspond à une tache, 
// chaque colone correpond à une composante de tache


vector<vector< int> > sharp_positions()
{
    int nb=9; // le nb de # dans la définition d'une unique tache
    int id_lign, id_clmn =0;  

    // Nous créons une "matrice"
    vector<vector< int>> matrix;

    //Nous ouvrons le fichier dans lequel sont stockées les taches
    fstream file("c.txt", ios::in |ios::out |ios::ate);    

    //Nous déterminons la longeur total du fichier     
    file.seekg(0,ios::end);    
    int pos_end=file.tellg();

    //On se place au début du fichier
    file.seekg(0,ios::beg);

    /// Nous forcons la position manuellement. En effet, pour une 
    // raison mystérieuse, nous avons constaté un décalage entre
    // les valeurs envoyées dans le vecteur et la position réelle. 
    int pos_get =0; 

    //on commence à parcourrir le fichier texte
    while(pos_get<pos_end)
    {   
        vector<int> one_lign;
        while (id_clmn<nb)
        {
            char ch;

            //int sharp_pos=file.tellg();
            file.seekp(pos_get,ios::beg);

            file.get(ch);
            if(ch=='#')
            {
                //one_lign.push_back(sharp_pos);
                //cout<<sharp_pos<<endl;    

                one_lign.push_back(pos_get);
                cout<<pos_get<<endl;    
                            
                id_clmn++;
            }
            pos_get++;
        }
        id_clmn=0;        
        
        matrix.push_back(one_lign);
        one_lign.clear();

        cout<<endl<<endl;
    }


    
    file.close();
    return matrix;
}

/////////// fonction qui affiche toutes les taches
//[i][1] et [i][2] sont à regarder (le numéro et la description)

void show_all()
{
    //On se dote de l'ensemble des positions des #
    vector<vector<int>> positions=sharp_positions();

    int nb_taches=positions.size();
    int compteur =0;

    //ouvrons le fichier 
    fstream file("c.txt", ios::in|ios::out|ios::ate);

    int pos; //les positions de # auxquelles sont l'information utile

    //parcourons le 
    while(compteur<nb_taches)
    {
        string str;

        //on se déplace au bon endroit (afficher l'id_tache)
        pos=positions[compteur][1]+1;
        file.seekg(pos, ios::beg);

        //on lit la ligne/la section correspondante à l'id tache
        getline(file, str,'#');   
        cout<<"id_tache: "<<str;   


        /// bis repetita placent    (afficher le titre)    
        //on se déplace au bon endroit 
        pos=positions[compteur][2]+1;
        file.seekg(pos, ios::beg);

        //on lit la ligne/la section correspondante au titre
        getline(file, str,'#');   
        cout<<str;  


        // juste pour le style        
        cout<<endl<<endl;

        //on incrémente le compteur de taches
        compteur++;


    }
}



//////











////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////// FONCTION QUI DISPATCHE ///////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void dispatch(string entree)
{
    ////////// Si l'utilisateur demande à écrire une nouvelle tache 
    if (entree == "W"){new_task();};

    ////////// Si l'utilisateur demande à lire une (plusieurs) tache(s)

    ////////// Si l'utilisateur demande à modifier une tache 
    // Nous affichons l' id_tache et le nom_tache  de toutes les taches

    // L'utilisateur choisit la tache qu'il veut modifier par son id_tache 

    // On affiche alors l'état de toutes les composantes  
};


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////////// FONCTION MAIN ////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

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