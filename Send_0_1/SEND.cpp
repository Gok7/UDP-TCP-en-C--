/* 
 * File:   CAPTURE
 * Author: Administrateur
 *
 * Created on 17 janvier 2013, 14:43
 */

#include <cstdlib>
#include <pcap.h>
#include <iostream>
#include <iomanip>


using namespace std;

/*
 * 
 */


pcap_if_t* Debut_liste_interface = NULL;

void Cherche_liste_carte() {
    char L_message[PCAP_ERRBUF_SIZE];
    int L_ok;
    L_ok = pcap_findalldevs(&Debut_liste_interface, L_message);
    if (L_ok == -1) {
        cerr << "ERREUR : " << L_message << endl;
    }
}

//------------------------------------------------------------------------------

void afficher_la_liste() {
    int L_numero = 1;
    pcap_if_t* l_maillon_courant = Debut_liste_interface;
    while (l_maillon_courant != NULL) {
        cout << "Carte numero " << L_numero << endl;
        cout << "Nom : " << l_maillon_courant->name << endl;
        cout << "Description : " << l_maillon_courant->description << endl;
        l_maillon_courant = l_maillon_courant->next;
        L_numero++;
    }


}

//------------------------------------------------------------------------------

char* afficher_carte_numero(int P_numero_carte) {
    char* L_nom = NULL;
    int L_numero = 1;
    pcap_if_t* l_maillon_courant = Debut_liste_interface;
    while ((l_maillon_courant != NULL) && (L_numero != P_numero_carte)) {
        L_numero++;
        l_maillon_courant = l_maillon_courant->next;

    }
    if (L_numero == P_numero_carte) {
        L_nom = l_maillon_courant->name;
        return L_nom;
    }
}

//------------------------------------------------------------------------------

pcap_t* Ouvrir_carte() {
    int l_numero;  
    char* l_nom = NULL;
    char l_message_erreur[PCAP_ERRBUF_SIZE];
    pcap_t* L_HANDLE;
    cout << "Donner un numero de carte : ";
    cin >> l_numero;

    l_nom = afficher_carte_numero(l_numero);
    if (l_nom != NULL) {

        L_HANDLE = pcap_open_live(l_nom, 1514, 1, 1000, l_message_erreur);
        if (L_HANDLE== NULL) cerr<<"ERREUR lors de l'ouverture : "<< l_message_erreur<< endl;

    }
    return L_HANDLE;
}

//------------------------------------------------------------------------------

void ferme_carte(pcap_t* P_HANDLE) {
    pcap_close(P_HANDLE);

}

//------------------------------------------------------------------------------

void envoie_trame (  pcap_t * P_handle,  u_char *  trame, int  P_taille ){
    int Erreur;
             Erreur= pcap_sendpacket  (P_handle, trame, P_taille   ); 
             if(Erreur==0){
                 cout<<"Bien envoye";
             }
             else{
                 cout<<"erreur";
             }
}
//------------------------------------------------------------------------------

void detruire_les_cartes() {

    pcap_freealldevs(Debut_liste_interface);
}

//------------------------------------------------------------------------------


int main(int argc, char** argv) {

    pcap_t* L_HANDLE;
    Cherche_liste_carte();
    afficher_la_liste();
    L_HANDLE=Ouvrir_carte();
    unsigned char Trame[1514];
    
    Trame[0]=0xFF;
    Trame[1]=0xFF;
    Trame[2]=0xFF;
    Trame[3]=0xFF;
    Trame[4]=0xFF;
    Trame[5]=0xFF;        
    
    Trame[6]=0xAA;
    Trame[7]=0x12;
    Trame[8]=0x21;
    Trame[9]=0x00;
    Trame[10]=0x99;
    Trame[11]=0x10;
    
    Trame[12]=0x12;//CP
    Trame[13]=0x89;
    //41 au revoir bye bye                    41   
    Trame[14]=0x41;
    Trame[15]=0x55;
    Trame[16]=0x52;
    Trame[17]=0x45;
    Trame[18]=0x56;
    Trame[19]=0x4F;
    Trame[20]=0x49;
    Trame[21]=0x52;
    while(1){
     envoie_trame (L_HANDLE, &Trame[0] , 20 );       
    }
     ferme_carte(L_HANDLE);
     
    detruire_les_cartes();

    return 0;
}

