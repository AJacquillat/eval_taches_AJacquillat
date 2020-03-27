#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

int main()
{   

    fstream file("test.txt", ios::in|ios::out|ios::ate);

    // get size of file
    file.seekg(0,file.end);
    int size = file.tellg();
    file.seekg (0); //on remet le curseur de lecture au début 
    cout<<size;

    //essayons d'écrire sur la première case du fichier et de remplacer le 'b' par un 'B'
    file.seekp(0, ios::beg);
    file<<"B";
    file.seekp(0,ios::end); //on essaie de se remettre à la fin 
    file<<" et vous?\n il me semble qu'il fait beau ajd'hui";

    file.close();
    file.close();
    return 0;
}