/* 
 * File:   ARP.h
 * Author: Administrateur
 *
 * Created on 7 février 2013, 15:53
 */

#ifndef ARP_H
#define	ARP_H
#pragma pack(1)
typedef struct {
   unsigned short int Type_Niveau_2;
   unsigned short int Type_Niveau_3;
   unsigned char Taille_Adresse_2;
   unsigned char Taille_Adresse_3;
   unsigned short int Code_Operation;
   unsigned char Source_Niveau_2[6];   // 6 pour ethernet
   unsigned char Source_Niveau_3[4];   // 4 pour IP
   unsigned char Destination_Niveau_2[6];   // 6 pour ethernet
   unsigned char Destination_Niveau_3[4];   // 4 pour IP
}T_MESSAGE_ARP;
#pragma pack()


void Affiche_ARP(T_MESSAGE_ARP* P_Message_ARP);



#endif	/* ARP_H */

