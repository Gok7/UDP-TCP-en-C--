#include <cstdlib>
#include <iostream>

using namespace std;


#include "CAPTURE.h"

#include "GLOBAL.h"
#include "LISTE.h"
#include "AFFICHAGE.h"
#include "ETHERNET.h"
#include "ARP.h"

//==============================================================================
pcap_if_t* Debut_Liste_Interface=NULL;

#define TIMEOUT_LECTURE 200
//==============================================================================

void Cherche_Liste_Cartes()
{
 char L_Message[PCAP_ERRBUF_SIZE];   
  int L_OK= pcap_findalldevs(&Debut_Liste_Interface,L_Message);
  if (L_OK==-1) {
     cerr<<"ERREUR : "<<L_Message<<endl; 
  } 
}
//------------------------------------------------------------------------------
void Affiche_Liste_Cartes()
{
pcap_if_t* L_Maillon=  Debut_Liste_Interface;
  while (L_Maillon) {
  
      cout<<"Nom : "<<L_Maillon->name<<endl;
      cout<<"Description : "<<L_Maillon->description<<endl;
      cout<<"------------------"<<endl;
      L_Maillon = L_Maillon->next;
  }
}
//---------------------------------------------

char* Donne_Nom_Carte(int P_Numero)
{
 pcap_if_t* L_Maillon=  Debut_Liste_Interface;
 char* L_Nom=NULL;
 int L_Index=0;
 while ((L_Maillon!=NULL) && (L_Index<P_Numero)){
     L_Maillon=L_Maillon->next;   
     L_Index++;
 }
 
 if (L_Index==P_Numero) L_Nom = L_Maillon->name;
 return L_Nom;
    
}


//-------------------------------------------

void Detruire_Liste_Cartes()
{
    pcap_freealldevs(Debut_Liste_Interface);
}

//-------------------------------------------

pcap_t* Ouvre_Carte(int P_Numero)
{
 pcap_t* L_Handle=NULL;
 char L_Message[PCAP_ERRBUF_SIZE];  
  
 char* L_Nom = Donne_Nom_Carte(P_Numero);
 if (L_Nom!=NULL) {
   L_Handle = pcap_open_live(L_Nom,1514,1, TIMEOUT_LECTURE, L_Message);
   if (L_Handle==NULL) 
         cerr<<"ERREUR lors de l'ouverture de la carte : "<<L_Message<<endl;
 }
 return L_Handle;
}
//------------------------------------------------------------------------------

void Ferme_Carte(pcap_t* P_Handle)
{
    pcap_close(P_Handle);
}



//-----------------------------------------------------------------------------
void Capture_Et_Analyse(pcap_t* P_Handle)
{
pcap_pkthdr L_Info_Capture;
 
const unsigned char* L_Dump_Trame = pcap_next(P_Handle,&L_Info_Capture );
  if (L_Dump_Trame != NULL) {
      
      T_TRAME* L_Trame=(T_TRAME*)L_Dump_Trame;
      if (L_Trame->Code_Protocolaire==INVERSE_OCTETS(0x0806)) {
  
          T_MESSAGE_ARP* L_Message_ARP =(T_MESSAGE_ARP*)(L_Dump_Trame+sizeof(T_TRAME)); 
          Affiche_ARP(L_Message_ARP); 
          cout<<endl;
          if (Cherche_Adresse_Existante(L_Message_ARP->Source_Niveau_3)==false) {
              cout <<"=====================> NOUVEAU HOST DETECTE : ";
              Affiche_Adresse_IP(L_Message_ARP->Source_Niveau_3);
              cout<<"/";
              Affiche_MAC(L_Message_ARP->Source_Niveau_2);
              cout<<endl;
             Ajoute_Adresse(L_Message_ARP);
          }
      }
      
  }  
}
