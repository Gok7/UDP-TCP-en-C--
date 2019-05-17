/* 
 * File:   CAPTURE
 * Author: Administrateur
 *
 * Created on 17 janvier 2013, 14:43
 */
//utiliser pcap send packet
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

void Affiche_dump(const unsigned char* P_dump,int P_taille){
    
    for (int l_index=0;l_index<P_taille;l_index++){
        
        cout<<hex<<setw(2)<<setfill('0')<<(int)P_dump[l_index]<<" ";
        if(((l_index)%16) == 15) cout<<endl;
    }
    cout<<endl;
}

//------------------------------------------------------------------------------

void detruire_les_cartes() {

    pcap_freealldevs(Debut_liste_interface);
}

//------------------------------------------------------------------------------

unsigned char capture_de_trame(pcap_t* P_HANDLE)
{
    pcap_pkthdr L_Info_Trame;
    const unsigned char* L_trame=pcap_next(P_HANDLE,&L_Info_Trame);
    if (L_trame != NULL ){
        Affiche_dump(L_trame,L_Info_Trame.len);
    }
    
}

int main(int argc, char** argv) {

    pcap_t* L_HANDLE;
    Cherche_liste_carte();
    afficher_la_liste();
    L_HANDLE=Ouvrir_carte();
    
    unsigned char L_Trame[1514];
    
    L_Trame[0]=0x08;
    L_Trame[1]=0x00;
    L_Trame[2]=0x02;
    L_Trame[3]=0x31;
    L_Trame[4]=0x20;
    L_Trame[5]=0x16;
    L_Trame[6]=0x09;
    
    
    ferme_carte(L_HANDLE);
    detruire_les_cartes();

    return 0;
}

