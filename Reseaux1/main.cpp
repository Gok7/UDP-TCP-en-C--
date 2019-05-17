/* 
 * File:   main.cpp
 * Author: Administrateur
 *
 * Created on 17 janvier 2013, 14:44
 */
#include<iomanip>
#include <cstdlib>
#include<iostream>
#define TIMEOUT_LECTURE 1000
using namespace std;

/*
 * 
 */

#include "pcap.h"

pcap_if_t * debut_liste_interface = NULL;

void cherche_liste_carte() {

    char L_message[PCAP_ERRBUF_SIZE];

    int L_ok = pcap_findalldevs(&debut_liste_interface, L_message);
    if (L_ok == 1) {

        cerr << "ERREUR :" << L_message << endl;

    }

}

void affiche_liste_carte() {

    pcap_if_t * L_maillon = debut_liste_interface;

    while (L_maillon) {


        cout << "Nom: " << L_maillon-> name << endl;
        cout << "Description: " << L_maillon-> description << endl;
        cout << "-----------------------" << endl << endl;
        L_maillon = L_maillon-> next;
    }

}

char* donne_nom_carte(int P_num) {

    pcap_if_t* L_maillon = debut_liste_interface;
    char* L_nom = NULL;
    int L_index = 0;
    while ((L_maillon != NULL) && (L_index < P_num)) {

        L_maillon = L_maillon-> next;
        L_index++;

    }

    if (L_index == P_num) {

        L_nom = L_maillon-> name;
        return (L_nom);

    }
}

/*void detruire_liste_carte() {

    pcap_freealldevs();

}*/

void ferme_carte(pcap_t* P_handle) {

    pcap_close(P_handle);

}

pcap_t * ouvre_carte(int P_num){
    
    pcap_t* L_handle= NULL;
    char L_message[PCAP_ERRBUF_SIZE];
    char* L_nom = donne_nom_carte(P_num);
    
    if(L_nom!=NULL){
        
        L_handle = pcap_open_live(L_nom,1514,1,TIMEOUT_LECTURE, L_message);
        
    }
        if(L_nom==NULL){
        
            cerr<<"erreur lors de l'ouverture de la carte"<<L_message<<endl;
        
    }
    
    return (L_handle);
}

void affiche_dump(int P_taille, const unsigned char* P_dump){
    
 for(int L_index=0;L_index<P_taille;L_index++){
     
     cout<<hex<<setw(2)<< setfill('0')<<(int) P_dump[L_index]<<" ";
     
     if((L_index%16)==15) cout<<endl;
     
 }   
    
    
    
}

void capture_et_affiche(pcap_t* P_handle){
    
    pcap_pkthdr L_info_capture;
    
    const unsigned char* L_trame = pcap_next(P_handle, &L_info_capture);
    
    if(L_trame!=NULL){
        
     affiche_dump(L_info_capture.len,L_trame);   
        
    }
}

int main(int argc, char** argv) {


    char* L_nom;
    unsigned char L_dump[]={1,2,100,255};
    //unsigned char* L_dump=0X12354;
    
    cherche_liste_carte();
    //affiche_liste_carte();
    L_nom = donne_nom_carte(0);
    cout << "Carte 1:" << L_nom << endl;
    L_nom = donne_nom_carte(1);
    cout << "Carte 2:" << L_nom << endl;
    L_nom = donne_nom_carte(2);
    cout << "Carte 3:" << L_nom << endl<<endl<<endl;
    
    //affiche_dump(16,L_dump);
    pcap_t* L_carte= ouvre_carte(1);
    
    while(1){
    
    capture_et_affiche(L_carte);
    
    cout<<"-------------------"<<endl;
    }
    
    
    ferme_carte(L_carte);
    //detruire_liste_carte();
            
    

    return 0;
}

