#include <cstdlib>
#include <iostream>
#include <memory.h>
using namespace std;


#include "LISTE.h"
#include "AFFICHAGE.h"

//------------------------------------------------------------------------------
T_MAILLON* Debut_Liste=NULL;

//------------------------------------------------------------------------------
bool Cherche_Adresse_Existante(unsigned char* P_Adresse_IP)
{
  T_MAILLON* L_Maillon=Debut_Liste;
  bool L_Trouve=false;
   while ((L_Maillon)&&(L_Trouve==false)) {
       L_Trouve = (memcmp(L_Maillon->Adresse_IP, P_Adresse_IP, 4)==0);
       L_Maillon= L_Maillon->Suivant;  
 }   
  return L_Trouve;
}
//-----------------------------------------------------------------------------
void Ajoute_Adresse(T_MESSAGE_ARP* P_Message_ARP)
{

    if (Cherche_Adresse_Existante(P_Message_ARP->Source_Niveau_3)==false) {
        T_MAILLON* L_Maillon= new T_MAILLON;

        memcpy(L_Maillon->Adresse_IP,P_Message_ARP->Source_Niveau_3,4);
        memcpy(L_Maillon->Adresse_MAC,P_Message_ARP->Source_Niveau_2,6);

        L_Maillon->Suivant=Debut_Liste;
        Debut_Liste= L_Maillon;
    }
}
//------------------------------------------------------------------------------

void Affiche_Liste_Adresses()
{
 T_MAILLON* L_Maillon=Debut_Liste;
 while (L_Maillon) {
     cout <<" Hosts : ";
     Affiche_Adresse_IP(L_Maillon->Adresse_IP);
     cout <<" ====> ";
     Affiche_MAC(L_Maillon->Adresse_MAC);
     
     cout<<endl;
     L_Maillon= L_Maillon->Suivant;
 }
}
//------------------------------------------------------------------------------
void Detruire_Liste_Adresses()
{
    T_MAILLON* L_Maillon;
    while (Debut_Liste) {
        L_Maillon= Debut_Liste->Suivant;
        delete Debut_Liste;
        Debut_Liste=L_Maillon;
    }
}