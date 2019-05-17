/* 
 * File:   ETHERNET.h
 * Author: Administrateur
 *
 * Created on 7 février 2013, 15:55
 */

#ifndef ETHERNET_H
#define	ETHERNET_H

#pragma pack(1)
typedef struct {
    unsigned char Mac_Destination[6];
    unsigned char Mac_Source[6];
    unsigned short int Code_Protocolaire;    
}T_TRAME;
#pragma pack()


#endif	/* ETHERNET_H */

