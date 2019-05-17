/* 
 * File:   main.cpp
 * Author: Administrateur
 *
 * Created on 7 février 2013, 14:03
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <conio.h>
using namespace std;



#include "CAPTURE.h"
#include "LISTE.h"

/*
 * 
 */




//=============================================================================



int main(int argc, char** argv) {
    Cherche_Liste_Cartes();
    Affiche_Liste_Cartes();

    pcap_t* L_Carte = Ouvre_Carte(1); 
    
    char L_Touche;

    bool L_Fin_Application=false;
    do {
        while (_kbhit()==0) {
            Capture_Et_Analyse(L_Carte);
        }
        
        L_Touche=getch();
        if ( (L_Touche=='l') || (L_Touche=='L')) {
           system("cls");
           cout<<endl<<"LISTE : "<<endl;
           Affiche_Liste_Adresses(); 
           cout<<endl<<" reprise de la capture ..."<<endl;

        } else {
            L_Fin_Application=true;
        }
    } while (L_Fin_Application==false);
    
    cout<<endl<<" Fin de la capture...."<<endl;
    cout<<endl<<"LISTE : "<<endl;
    Affiche_Liste_Adresses();
    
    Ferme_Carte(L_Carte);
    Detruire_Liste_Cartes();
    
    Detruire_Liste_Adresses();
    return 0;
    

}

