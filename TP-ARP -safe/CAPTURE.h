/* 
 * File:   CAPTURE.h
 * Author: Administrateur
 *
 * Created on 7 février 2013, 16:01
 */

#ifndef CAPTURE_H
#define	CAPTURE_H

#include "pcap.h"

void Cherche_Liste_Cartes();
void Affiche_Liste_Cartes();
pcap_t* Ouvre_Carte(int P_Numero);
void Capture_Et_Analyse(pcap_t* P_Handle);
void Ferme_Carte(pcap_t* P_Handle);
void Detruire_Liste_Cartes();

#endif	/* CAPTURE_H */

