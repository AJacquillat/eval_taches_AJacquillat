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

///explications sur les choix de reponses attendues 
// Q <- quit         
// W <- Write 
// S <- Show 
// E <- Edit         editer une tâche (pourcentage, avancement, sous taches ok?)


//// Nous interdirons à l'utilisateur d'utiliser le # qui sera utilise par le programme pour 
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

///fonction qui donne l'id de la tache à creer
int new_id()
{
    ///La premiere etape consiste à tester la bonne ouverture du fichier     
    fstream test_file("c.txt", ios::in| ios::out|ios::ate);
    test_file.seekp(0,ios::end);    
    int pos_end=test_file.tellp();

    ///Cas ou le fichier n'existe pas ou bien ne peut pas s'ouvrir
    //S'il n'existe pas, nous essayons de le creer
    if (pos_end ==-1) 
    {
        ofstream ofile("c.txt"); //cela cree le fichier
        ofile.close(); //cela ferme l'objet ofile
    }
    test_file.close();


    /// On recommence tout pour laisser une chance au fichier cree de s'ouvrir 
    fstream file("c.txt", ios::in| ios::out|ios::ate);
    file.seekp(0,ios::end);    
    pos_end=file.tellp();

    //Cas ou le fichier ne peut pas s'ouvrir, on releve l'erreur
    if (pos_end ==-1) 
        throw logic_error("probleme dans l'ouverture du fichier, merci de mettre les mains dans le cambouis\nVous creerez alors un fichier c.txt dans le même folder que l'executable\nMerci\n");

    if(pos_end<2)//alors le fichier existe mais est vide ou en tout cas n'a pas de tache ecrite, l'id est donc 0
    {
        return(0);
    } 
    else {
        //on se place à la fin du fichier texte
        //mais on veut trouver toute la ligne, si c'es( la 12e tâche, voir 12 et pas 2)
        //on met le lecteur à la fin, il faut qu'il lise la derniere ligne toute entiere
        file.seekg(pos_end-2,ios::beg); 
        int pos = pos_end-2;

        //on recupere le caractere de bout de fichier (le dernier choffre du nombre)
        char ch; 
        file.get(ch);

        //Tant que ce caractere n'est pas le # qui marque le debut de chaque sous section,
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

        //Nous recuperons toute la ligne à l'exception du dernier #
        string last_id_string;
        getline(file, last_id_string, '#');

        //on utilise un classe toute faite pour 
        //convertir la string en int 
        stringstream geek(last_id_string); 
        int last_id_int; 
        geek >> last_id_int;
        file.close();

        //Nous renvoyons l'int qui doit être le numero de la tache  
        return( last_id_int+1 );
    }
    
}

//cette fonction sert àn capturer ce qui est ecrit par l'utilisateur 
string saisie()
{
    
    std::cin.clear();
    std::cin.ignore(256,'\n');
    string buffer; //l'element qui va stocker la saisie
    std::cout << "Ecrivez # puis pressez retour-arriere pour enregistrer le texte.\n";
    char ch; //on separe la selection en differents char pour s'assurer qu'aucun des massages ne conteint le caractere interdit #
    do {
        ch = std::cin.get();
        buffer.append(1,ch); //on ajout la ch à la fin du buffer
    } while (ch!='#');

    //on supprime juste le # final
    buffer.pop_back(); 

    return buffer;
}

// même chose que saisie mais s'assure que le resultat communique est 
// un entier entre 1 et 3
string saisie_prio()
{
    
    std::cin.clear();
    std::cin.ignore(256,'\n');
    int entree;
    cout<<"entrez un entier entre 1 et 3\n";
    std::cin >> entree; 
    while (std::cin.fail() or entree>3 or entree<1)
    {
        cout<<"Entree non conforme\nRecommencez\n";
        std::cin.clear();
        std::cin.ignore(256,'\n');
        std::cin >> entree;        
    };
    string str=int_to_string(entree);
    return str;
}

/// Fonction pour ecrire une tâche 
void new_task()
{
    // Pour eviter d'avoir un probleme en cas de crash pendant la defnition d'une tache
    // et de recuperer un fichier pas aux normes, 
    // nous stockons toutes les reponses dans un buffer. 
    //C'est au dernier moment seulement que nous ajouterons ce buffer au fichier texte 

    // Pour comprendre le choix de mise en forme du buffer, se referer au fichier format_type.txt
    string buffer;

    ////////// Ecriture de l'id_tache dans le buffer 
    int id_tache = new_id(); //notons que le fichier est forcement ouvert car nous avons jete l'exception dans new_id()
    
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
    //precisee dans le fichier format_type.txt
    buffer=buffer+"#"+buffer_tempo+"\n";

    //vidons maintenant le buffer_tempo
    buffer_tempo.clear();




    /////////// description de la tache 
    //on dit a l'utilisateur ce qu'il doit faire 
    cout<<"\n   Saisissez la description de la nouvelle tache.\n";
    buffer_tempo=saisie();
    //on a maintenant dans buffer_tempo une string qui contient la saisie     
    // ajoutons dans le gros buffer le contenu du buffer_tempo selon la syntaxe 
    //precisee dans le fichier format_type.txt
    buffer=buffer+"#"+buffer_tempo.substr(1,buffer.length()-1)+"\n";

    //vidons maintenant le buffer_tempo
    buffer_tempo.clear();




    ///////////// date de creation de tache
    buffer=buffer+"#"+date_heure();


    //////////// date de fin de tache
    // l'utilisateur modifie lui même au format habituel 
    // on peut imaginer automatiser cela plus tard quand avancee passe à 100% ou quand le status passe à D (done)
    // on met le reste de ces informations liees entre elles à jour, 
    // en attendant, on reserve de la place pour cette date en remplissant une 'date' fausse
    buffer=buffer+"#"+"000 000 00 00:00:00 0000"+"\n";



    ///////// On ajoute un status, par defaut la tache est "open" ('O'), à l'utilisateur de la modifier quand "done" ('D')
    buffer=buffer+"#"+"O"+"\n";

    ///////// On ajoute un pourcentage, 000% par defaut 
    buffer=buffer+"#000%"+"\n";

    ///////// On ajoute une priorite
    //on dit a l'utilisateur ce qu'il doit faire 
    cout<<"\n   Saisissez la priorite (1<2<3) de la nouvelle tache.\n";
    buffer_tempo=saisie_prio();
    //on a maintenant dans buffer_tempo une string qui contient la saisie     
    // ajoutons dans le gros buffer le contenu du buffer_tempo selon la syntaxe 
    //precisee dans le fichier format_type.txt
    buffer=buffer+"#"+buffer_tempo+"\n";
    //vidons maintenant le buffer_tempo
    buffer_tempo.clear();


    ////////// on ajoute l'espace commentaires, vide pour l'instant
    buffer=buffer+"#"+"\n";

    ////////// on ajoute l'espace sous taches, à remplir à l'ouverture de la tache
    //Laissons l'utilisateur remplir. Cette zone n'etant pas faite pour être modifiee
    // nous nous passerons de separateur entre sous taches, laissant à l'utilisateur 
    // la responsabilite de bien organiser son espace 
    cout<<"sous taches? A vous de les organiser toutes d'un coup.\n";
    buffer_tempo=saisie();
    buffer=buffer+"#"+buffer_tempo.substr(1,buffer.length()-1)+"\n";
    buffer_tempo.clear();




    ////////// On repete une derniere fois l'id_tache, ce qui clos l'espace dedie à la tache
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


////////pour une tache donnee
//0&1er # : numero de la tache
// 2e: titre
// 3e: description 
// 4e: date debut
// 5e: date fin
// 6e: etat de la tache ('O' open ou 'D' done)
// 7e: pourcentage
// 8e: priorite (1<2<3)
// 9e: Commentaires (separateur &)
// 10e : sous taches 
// 11e: repetition du numero de tache 
// 12e: pour clore la tache 

///////// L'idee est de parcourir une fois pour toute la liste des taches, 
// les positions seront bonnes pdt tt l'exploration dans S (Select)
// Cette fonction pourra servir à nouveau pour les modifications.

// Chaque id_ligne correspond à une tache, 
// chaque colone correpond à une composante de tache


vector<vector< int> > sharp_positions()
{
    int nb=13; // le nb de # dans la definition d'une unique tache
    int id_lign, id_clmn =0;  

    // Nous creons une "matrice"
    vector<vector< int>> matrix;

    //Nous ouvrons le fichier dans lequel sont stockees les taches
    fstream file("c.txt", ios::in |ios::out |ios::ate);    

    //Nous determinons la longeur total du fichier     
    file.seekg(0,ios::end);    
    int pos_end=file.tellg();

    //On se place au debut du fichier
    file.seekg(0,ios::beg);

    /// Nous forcons la position manuellement. En effet, pour une 
    // raison mysterieuse, nous avons constate un decalage entre
    // les valeurs envoyees dans le vecteur et la position reelle. 
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
                id_clmn++;
            }
            pos_get++;
        }
        id_clmn=0;        
        
        matrix.push_back(one_lign);
        one_lign.clear();
    }


    
    file.close();
    return matrix;
}


////// fonction qui montre une section quand on donne l'adresse du # de debut de section 

void show_section(int position)
{
    fstream file("c.txt", ios::in|ios::out|ios::ate);    
    file.seekg(position+1, ios::beg);

    string str;

    //on lit la ligne/la section correspondante à l'id tache
    getline(file, str,'#');   
    cout<<"#"<<str; 

    file.close();  
}


///////// fonction qui montre les items i (dans la string)
// pour une tache donnee ie pour un vecteur de positions des # donne
// par defaut, supposons que l'on affiche toute la tache 
void show_one(vector<int> task, string items = {1,2,3,4,6,7,8,9})
{
    fstream file("c.txt", ios::out|ios::in|ios::ate);
    for (int i:items)
    {
        int sharp_pos= task[i];
        show_section(sharp_pos);
    }
    file.close();
}

/////////// fonction qui affiche toutes les taches
//[i][1] et [i][2] sont à regarder (le numero et la description)

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

        //on se deplace au bon endroit (afficher l'id_tache)
        pos=positions[compteur][1]+1;
        file.seekg(pos, ios::beg);

        //on lit la ligne/la section correspondante à l'id tache
        getline(file, str,'#');   
        cout<<"id_tache: "<<str;   


        /// bis repetita placent    (afficher le titre)    
        //on se deplace au bon endroit 
        pos=positions[compteur][2]+1;
        file.seekg(pos, ios::beg);

        //on lit la ligne/la section correspondante au titre
        getline(file, str,'#');   
        cout<<str;  


        // juste pour le style        
        cout<<endl<<endl;

        //on incremente le compteur de taches
        compteur++;


    }
}



////// fonction qui effectue une boucle de l'interface show
// pour l'instant, dans une vesrions tres primitive 
// Je passe la matrice à la fonction en passant plutot son adresse 
// pour ne pas avoir à copier à chaque fois une matrice (meme de taille raisonnable)

// Je ne suis pas sur de l'interet de proceder de la sorte car je fais appel à cette 
// matrice quand même mais bon... j'ai essaye 
void show_one_step(int entree, vector<vector<int>>* adr_positions )
{
    if(entree ==1)
    {
    //nous nous bornons pour l'instant à afficher toutes les taches
    show_all();
    }
    else if (entree ==2)
    {
        //tâche à consulter 
        cout<<"quelle tache voulez vous consulter?(rentrez son id)"<<endl;
        int id_tache;
        std::cin>>id_tache;

        //on prend la ligne des adresse 
        vector<int> lign = (*adr_positions)[id_tache];

        
        //Elements à consulter 
        cout<<"quels elements voulez vous consulter?"<<endl;
        cout<<"1er # : numero de la tache\n2e: titre\n3e: description\n4e: date debut\n5e: date fin\n6e: etat de la tache ('O' open ou 'D' done)\n7e:Pourcenatage\n8e: priorite (1<2<3)\n9e: Commentaires (separateur &)\n10e : sous taches"<<endl;
        cout<<"entre chaque nombre, pressez entree ou laissez un espace"<<endl;
        cout<<"quand vous avez termine la saisie, appuyez sur t puis entree "<<endl;
        string str;
        int ch;
        while((!std::cin.fail()))
        {
            str.push_back(ch);
            std::cin>>ch;
        }
    
        std::cin.clear();
        std::cin.ignore(256,'\n');
        show_one(lign,str);
    }
    else
    {
        cout<<"\n\nMAUVAISE SAISIE\n\n";
        std::cin.clear();
        std::cin.ignore(256,'\n');
    }
}


void show()
{
    // demander à l'utilisateur ce qu'il veut 
    /// 1) Afficher plusieurs taches en partie 
    /// 1.1) Afficher titre & id de toutes les taches 
    /// 1.2) Afficher titre & id de toutes les taches ouvertes 
    /// 2) Afficher tout de toutes les taches 
    /// 3) Afficher une tache (donner id) en entier
    cout<<"Vous etes dans l'espace show.\nQue voulez vous faire?\n0.Revenir au menu principal\n1.Afficher toutes les taches\n2.Afficher une tache\n";
    

    // calculons une fois pour toutes la matrice des positions 
    vector<vector<int>> matrix = sharp_positions();

    //ecoutons l'utillisateur 
    int entree;
    std::cin>> entree;
    while(entree!=0)
    {
        if(std::cin.fail())
        {
            cout<<"\n\nMAUVAISE SAISIE\n\n";
            std::cin.clear();
            std::cin.ignore(256,'\n');
            //on revient donc dans la fonction main et on revient dans l'interface show
            cout<<"Vous etes dans l'espace show.\nQue voulez vous faire?\n0.Revenir au menu principal\n1.Afficher toutes les taches\n2.Afficher une tache\n";
        }
        else
        {
            show_one_step(entree, &matrix);
        }

        cout<<endl<<endl<<"Vous etes dans l'espace show.\nQue voulez vous faire?\n0.Revenir au menu principal\n1.Afficher toutes les taches\n2.Afficher une tache\n";
        std::cin>> entree;
    }
    

}





////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////// FONCTIONS POUR EDITER ///////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


/// fonction qui copie tout le fichier à partir de la position pos
// dans une string
// nous nous en servirons dans l'ecriture de commentaires 
// cette fonction renvoie en premier element de string 
// l'element à la position pos (et non pos + 1)
// c'est à dire dans l'utilisation que nous en ferons à '\n'

string end_as_string(int pos)
{
    fstream file("c.txt", ios::in|ios::out|ios::ate);
    file.seekg(pos, ios::beg);
    string total;
    string buffer_tempo;
    while(!file.eof())
    {
        getline(file,buffer_tempo);
        total=total+buffer_tempo+"\n";
    }
    return total;
}

// fonction qui a une position donnee modifie ce que l'on trouve en s'assurant que 
// la mise en forme n'est pas detruite en faisant un write(remplacement, taille_de_ce_qu'il_y_avait_avant)
void remplace(int sharp_position, string remplacement)
{
    fstream file("c.txt", ios::in|ios::out|ios::ate);
    file.seekg(sharp_position+1, ios::beg);
    
    //on lit la ligne/la section correspondante
    string str;
    getline(file, str,'#'); 
    int size=str.size();

    file.seekp(sharp_position +1, ios::beg);

    file.write(remplacement.c_str(),size-1); // pour convertir la string en char* 
    // le -1 correspond à la taille prise par le retour à la ligne qui precede le # et que l'on ne veut pas voir disparaitre
}


//0&1er # : numero de la tache
// 2e: titre
// 3e: description 
// 4e: date debut
// 5e: date fin
// 6e: etat de la tache ('O' open ou 'D' done)
// 7e: pourcentage
// 8e: priorite (1<2<3)
// 9e: Commentaires (separateur &)
// 10e : sous taches 
// 11e: repetition du numero de tache 
// 12e: pour clore la tache 

// nous recalculons à chaque iteration la position des sharp 
// car l'insertion de commentaires peut tout decaler 
void edit_one_step(int entree)
{
    
    fstream file("c.txt", ios::in|ios::out|ios::ate);
    vector<vector<int>> positions=sharp_positions();
    if(entree ==1)
    {
        //nous nous bornons pour l'instant à afficher toutes les id_taches et leur titre
        show_all();
    }
    else if (entree ==2)
    {
        //tâche à consulter 
        cout<<"quelle tache voulez vous modifier?(rentrez son id)"<<endl;
        int id_tache;
        std::cin>>id_tache;

        //on prend la ligne des adresse 
        vector<int> lign = positions[id_tache];


        ///    2.1) Ne rien faire 
        ///    2.2) Modifier %age (le)
        ///    2.3) Clore la tache (ie chger date de fin & Pourcentage)
        ///    2.4) Ajouter un commentaire

        //Elements à modifier 
        cout<<"  Quel element voulez vous modifier?"<<endl;
        cout<<"1)Ne rien faire\n2)Modifier pourcentage \n3)Clore tache\n4)Ajouter un commentaire"<<endl;
        cout<<"Entre chaque nombre, pressez entree ou laissez un espace"<<endl;
        cout<<"Quand vous avez termine la saisie, appuyez sur entree "<<endl;

        //on nettoie les résidus         
        std::cin.clear();
        std::cin.ignore(256,'\n');

        int ch;
        std::cin>>ch;

        if (ch==1){ }//on ne fait rien
        else if (ch== 2) 
        {
            int sharp_pos=lign[7];
            cout<<"Entrez le nouveau pourcentage sous la forme xxx\nPour 100% ,x=1,y=z=0; pour 22%, x=0,y=z=2 (ou pas de x)) \n";
            int n;
            std::cin>>n;
            if(std::cin.fail()){cout<<"\nMAUVAISE SAISIE\n\n";}
            else
            {
                if (n==100)
                {
                    remplace(sharp_pos,"100%");
                    remplace(lign[6],"D");
                    remplace(lign[5],date_heure());
                }
                else if(n>9){remplace(sharp_pos,"0"+int_to_string(n)+"%");}//on corrige seulement les deux derniers avec l'int rentre
                else{remplace(sharp_pos,"00"+int_to_string(n)+"%");}
            }
        }
        else if (ch== 3)
        {
            //clore la tache
            remplace(lign[7],"100%");
            remplace(lign[6],"D");
            remplace(lign[5],date_heure());
        }
        else if (ch==4)
        {
            // nous n'autorisons pour l'instant que l'ajout de commentaire,
            // pas encore la modification des precedents (ce qui reviendrait
            // à utiliser la fonction remplace un peu modifiee avec le bon separateur)
            cout<<"ecrivez votre commentaire"<<endl;
            string comm=saisie();

            //nous inserons ce commentaire à la fin de la section commentaire (id: 9)
            // c'est à dire au debut -1 de la section sous taches (id: 10)
            int pos=lign[10]-1;

            string fin=end_as_string(pos);
            file.seekp(pos);
            file<<comm;
            file<<fin;
            
        }
        else{cout<<"\n\nMAUVAISE SAISIE\n\n";
            std::cin.clear();
            std::cin.ignore(256,'\n');}
    
        file.close();
    }
    else{cout<<"\n\nMAUVAISE SAISIE\n\n";
            std::cin.clear();
            std::cin.ignore(256,'\n');}

}


void edit()
{
    // demander à l'utilisateur ce qu'il veut 
    /// 0) Revenir au menu principal
    /// 1)Afficher titre & id de toutes les taches 
    /// 2) Modifier une tache slectionee par son id
    ///    2.1) "N": ne rien faire 
    ///    2.2) Modifier %age
    ///    2.3) Clore la tache (ie chger date de fin & etat)
    ///    2.4) Ajouter un commentaire
    cout<<"Vous etes dans l'espace edit.\nQue voulez vous faire?\n0.Revenir au menu principal\n1.Afficher toutes les taches\n2.Modifier une tache\n";
    

    // calculons une fois pour toutes la matrice des positions 
    vector<vector<int>> matrix = sharp_positions();

    //ecoutons l'utillisateur 
    int entree;
    std::cin>> entree;
    while(entree!=0)
    {
        if(std::cin.fail())
        {
            cout<<"\n\nMAUVAISE SAISIE\n\n";
            std::cin.clear();
            std::cin.ignore(256,'\n');
            //on revient donc dans la fonction main et on revient dans l'interface edit
            cout<<"Vous etes dans l'espace edit.\nQue voulez vous faire?\n0.Revenir au menu principal\n1.Afficher toutes les taches\n2.Modifier une tache\n";
        }
        else
        {
            edit_one_step(entree);
        }

        cout<<endl<<endl<<"Vous etes dans l'espace edit.\nQue voulez vous faire?\n0.Revenir au menu principal\n1.Afficher toutes les taches\n2.Modifier une tache\n";
        std::cin>> entree;
    }
      

}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////// FONCTION QUI DISPATCHE ///////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void dispatch(string entree)
{
    ////////// Si l'utilisateur demande à ecrire une nouvelle tache 
    if (entree == "W"){new_task();};

    ////////// Si l'utilisateur demande à lire une (plusieurs) tache(s)
    if (entree == "S")
    {
        show();
    }
    ////////// Si l'utilisateur demande à modifier une tache 
    // Nous affichons l' id_tache et le nom_tache  de toutes les taches

    if (entree=="E")
    {
        edit();
    }
    
    else
    {
        cout<<"entree non conforme\n";
        
        std::cin.clear();
        std::cin.ignore(256,'\n');
    }
};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////////// FONCTION MAIN ////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


int main()
{
    int on=1; //la variable d'etat du systeme 1: allume 0: eteint 
    while (on==1) 
    {
        string entree;
        cout<<"tappez la lettre correspondant à l'action desiree : \n Q pour quitter \n W pour ecrire une tache \n S pour montrer une(des) tache(s)\n E pour editer une tache\n\n";
        cin>>entree;
        if(entree=="Q"){on=0;}
        else{dispatch( entree );}
    }
    return 0;
}