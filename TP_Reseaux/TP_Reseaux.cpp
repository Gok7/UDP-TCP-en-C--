//******************************************************************************
/* 
 * File:   TP_Reseaux.cpp
 * Author: Administrateur
 *
 * Created on 17 janvier 2013, 14:43
 */
//******************************************************************************

#include <cstdlib>
#include <iostream>
#include <pcap.h>
#include <iomanip>
#define TIMEOUT_LECTURE 5000
using namespace std;
unsigned char Trame_recue[1514];

typedef struct T_RESEAU {
    unsigned char Sender_Mac[6];
    unsigned char Sender_IP[4];
    unsigned char Target_Mac[6];
    unsigned char Target_IP[4];
} T_RESEAU;

//******************************************************************************
pcap_if_t* Debut_Liste_Interface = NULL;
//******************************************************************************

void Cherche_Liste_cartes() {

    char L_Message[PCAP_ERRBUF_SIZE];
    int L_OK = pcap_findalldevs(&Debut_Liste_Interface, L_Message);

    if (L_OK == -1) {
        cerr << "ERREUR : " << L_Message << endl;
    }

}
//******************************************************************************

void Detruire_Liste_Carte() {
    pcap_freealldevs(Debut_Liste_Interface);
}
//******************************************************************************

void Affiche_Liste_Cartes() {
    pcap_if_t* L_Maillon;
    L_Maillon = Debut_Liste_Interface;
    while (L_Maillon != NULL) {
        cout << "Description : " << L_Maillon->description << endl;
        cout << "Nom : " << L_Maillon->name << endl << endl;
        L_Maillon = L_Maillon->next;
    }
}
//******************************************************************************

char* Donne_Nom_carte(int P_numero) {
    char* L_Nom_carte = NULL;
    int L_compteur = 1;
    pcap_if_t* L_Maillon = Debut_Liste_Interface;

    for (L_compteur; (L_compteur < P_numero) && (L_Maillon != NULL); L_compteur++) {
        L_Maillon = L_Maillon->next;
    }
    if (L_compteur == P_numero) {
        L_Nom_carte = L_Maillon->name;
    }
    return L_Nom_carte;
}
//******************************************************************************

pcap_t* Ouvre_Carte(int P_num) {
    char L_Message[PCAP_ERRBUF_SIZE];
    pcap_t* L_Handle=NULL;
    char* L_nom = Donne_Nom_carte(P_num);
    if(L_nom != NULL){
    L_Handle = pcap_open_live(L_nom, 1514, 1,TIMEOUT_LECTURE, L_Message);
    }
    if (L_Handle == NULL){
        cerr<<"Erreur lors de l'ouverture de la carte : "<<L_Message<<endl;
    }
    return L_Handle;
}
//******************************************************************************

void Fermer_carte(pcap_t* P_Handle) {
    pcap_close(P_Handle);
}
//******************************************************************************
void Affiche_Dump(const unsigned char* P_Dump,int P_Taille){  
    for(int L_index = 0 ;L_index < P_Taille;L_index++){
        cout<<hex<<setw(2)<<setfill('0')<<(int)P_Dump[L_index]<<" ";
        if(((L_index) %16)==15){
            cout<<endl;
        }
    }
    cout<<endl;
}
//******************************************************************************
void Capture_Et_Affiche_Trame(pcap_t*P_Handle){
    
    pcap_pkthdr L_Info_Capture;
    
    const unsigned char* L_Trame = pcap_next(P_Handle,&L_Info_Capture);  
    if(L_Trame!=NULL){
        Affiche_Dump(L_Trame,L_Info_Capture.len);
    }
}
void Message_ARP(pcap_t*P_Handle){
    pcap_pkthdr L_Info_Capture;
    const unsigned char* L_Trame = pcap_next(P_Handle,&L_Info_Capture); 
    int L_index;
    if ((L_Trame[12]==8)&&(L_Trame[13]==6)) {
    cout <<"Addresse Mac Emmeteur : " ;
        for(L_index=22;L_index<28;L_index++) {
         cout<<hex<<setw(2)<<setfill('0')<<(int)L_Trame[L_index]<<" ";;
        }
    cout <<endl;
    cout <<"Addresse IP Emmeteur : ";
        for(L_index=28;L_index<32;L_index++){
         cout <<dec<<(int)L_Trame[L_index]<<".";
        }
    cout <<endl;
     cout <<"Addresse Mac Recepteur : ";
         for(L_index=32;L_index<38;L_index++) {             
         cout<<hex<<setw(2)<<setfill('0')<<(int)L_Trame[L_index]<<" ";;
         }
     cout <<endl;
     cout <<"Addresse IP Recepteur : ";
         for(L_index=38;L_index<42;L_index++){
         cout <<dec<<(int)L_Trame[L_index]<<".";
         }
     cout <<endl<<endl;
         }
    
}

int main(int argc, char** argv) {
    pcap_t* L_carte;
    unsigned char Trame[1514];
    Trame[0]=0xFF;
    Trame[1]=0xFF;
    Trame[2]=0xFF;
    Trame[3]=0xFF;
    Trame[4]=0xFF;
    Trame[5]=0xFF;
    
    Trame[6]=0x12;
    Trame[7]=0x08;
    Trame[8]=0xFF;
    Trame[9]=0x54;
    Trame[10]=0x4F;
    Trame[11]=0xF2;
            
    
    Trame[12]=0x13;
    Trame[13]=0x37;
    
    Trame[14]=0x43;
    Trame[15]=0x4F;
    Trame[16]=0x43;
    Trame[17]=0x4F;
    
    
    Cherche_Liste_cartes();
    Affiche_Liste_Cartes();    
    L_carte = Ouvre_Carte(2); // WifI
    
    while(1){
        // Capture_Et_Affiche_Trame(L_carte);
        Message_ARP(L_carte);
       
        
        //cout<<"********************"<<endl;
       // pcap_sendpacket(L_carte, Trame, 18 );
        
    }
         
    Fermer_carte(L_carte);
    Detruire_Liste_Carte();
    
    return 0;
}
//******************************************************************************
