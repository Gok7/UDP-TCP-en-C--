/* 
 * File:   LISTE.h
 * Author: Administrateur
 *
 * Created on 7 février 2013, 15:44
 */

#ifndef LISTE_H
#define	LISTE_H



#include "ARP.h"


typedef struct T_MAILLON
{
    unsigned char Adresse_IP[4];
    unsigned char Adresse_MAC[6];  
    T_MAILLON* Suivant;
}T_MAILLON;
//------------------------------------------------------------------------------

bool Cherche_Adresse_Existante(unsigned char* P_Adresse_IP);
void Ajoute_Adresse(T_MESSAGE_ARP* P_Message_ARP);
void Affiche_Liste_Adresses();
void Detruire_Liste_Adresses();

#endif	/* LISTE_H */

