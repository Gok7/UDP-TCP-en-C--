

#include <cstdlib>
#include <iostream>
#include <iomanip>
using namespace std;

#include "GLOBAL.h"
#include "ARP.h"
#include "AFFICHAGE.h"

void Affiche_ARP(T_MESSAGE_ARP* P_Message_ARP)
{
    
   cout<<"Type_Niveau_2 : "<<dec<<INVERSE_OCTETS(P_Message_ARP->Type_Niveau_2)<<endl;
   cout<<"Type_Niveau_3 : "<<hex<<INVERSE_OCTETS(P_Message_ARP->Type_Niveau_3)<<endl;
   cout<<"Taille_Adresse_2 : "<<dec<<(short int)P_Message_ARP->Taille_Adresse_2<<endl;
   cout<<"Taille_Adresse_3 : "<<dec<<(short int)P_Message_ARP->Taille_Adresse_3<<endl;
   cout<<"Code_Operation"<<hex<<INVERSE_OCTETS(P_Message_ARP->Code_Operation)<<endl;
   cout<<"Source_Niveau_2 : "; 
   Affiche_MAC(P_Message_ARP->Source_Niveau_2);
   cout<<endl;
   cout<<"Source_Niveau_3 : ";   
   Affiche_Adresse_IP(P_Message_ARP->Source_Niveau_3);
   cout<<endl;
   cout<<"Destination_Niveau_2 : ";   
   Affiche_MAC(P_Message_ARP->Destination_Niveau_2);
   cout<<endl;
   cout<<"Destination_Niveau_3 : ";   
   Affiche_Adresse_IP(P_Message_ARP->Destination_Niveau_3);
   cout<<endl;
}

