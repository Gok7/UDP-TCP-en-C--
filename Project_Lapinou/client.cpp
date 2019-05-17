#include "header1_client_TCP.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>

SOCKET H_SOCKET;
sockaddr_in Process_Distant;


DWORD rot_tete_droite = 0x01010064;
DWORD rot_tete_gauche = 0x0101FF9C;
DWORD rot_corp_droite = 0x020100B4;
DWORD rot_corp_gauche = 0x0201FF4C;
DWORD Inclinaison_tete_droite = 0x01020064;
DWORD Inclinaison_tete_gauche = 0x01020000;
DWORD Inclinaison_corp_droite = 0x0202005A;
DWORD Inclinaison_corp_gauche = 0x0202FFA6;
DWORD oreille_gauche_max = 0x03010064;
DWORD oreille_gauche_min = 0x03010000;
DWORD oreille_droite_max = 0x03020064;
DWORD oreille_droite_min = 0x03020000;

void Init_DLL() {
    WSADATA L_Info_DLL;

    if (WSAStartup(MAKEWORD(2, 0), &L_Info_DLL) != 0) {
        exit;
    }//je veux utiliser la dll a partir 
    //du moment où elle est compatible avec la version 2

#ifdef DEBUG_SOCKET
    cout << L_Info_DLL.szDescription << endl;
    cout << L_Info_DLL.szSystemStatus << endl;
#endif
}

void Fin_DLL() {

    int L_OK = WSACleanup();

#ifdef DEBUG_SOCKET
    if (L_OK == 0) cout << "WSACLEANUP : OK" << endl;
    else cout << "WSACLEANUP : ERROR" << endl;
#endif
}

void Creation_Socket() {
    H_SOCKET = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (H_SOCKET == INVALID_SOCKET) {
        cout << "Erreur invalid socket" << endl;
        Affiche_Erreur();
    }
#ifdef DEBUG_SOCKET
    cout << "Valeur du socket : " << H_SOCKET << endl;
#endif
}

void Affiche_Erreur() {
    DWORD L_Code_Erreur = GetLastError();
    char L_Message[512];

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
            L_Code_Erreur, 0, L_Message, 511, NULL);
    //1 : flag specifiant l'origine du message. 2 : valeur par defaut 
    //3 : code erreur. 4 : avoir le message en chaine de car
    //5 : adresse bloc memoire du message. 6 : maximum de car
    //7 : info supplementaire dans message

    cerr << "Erreur" << L_Code_Erreur << " : " << L_Message << endl;
}

void Creation_Info_Adressage() {

    Process_Distant.sin_family = AF_INET;
    Process_Distant.sin_port = htons(901);
    Process_Distant.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    // htons rend le code portable,permet de retourner si on a besoin
    //inet_addr retourne un long.
}

void Emission(const void* P_Data, int P_taille) {

    int L_taille = send(H_SOCKET, (char*) P_Data, P_taille, 0);
    if (L_taille == SOCKET_ERROR) {
        Affiche_Erreur();
    }
#ifdef DEBUG_SOCKET
    cout << "Valeur sendto : " << L_taille << endl;
#endif
}

void Connexion() {

    int L_OK = connect(H_SOCKET, (sockaddr*) & Process_Distant, sizeof (Process_Distant));

    if (L_OK == SOCKET_ERROR) Affiche_Erreur();

}

void Deconnexion() {

    shutdown(H_SOCKET, SD_BOTH);
    closesocket(H_SOCKET);
}

void Envoi(DWORD P_commande) {
/*
 rot tete de -100 a 100
 * inclinaison tet 0 à 100
 * rot corp de -180 a 180
 * Inclinaison corp de -90 a 90
 * oreille gauche de 0 à 100
 * oreille droite 0 à 100
 */
    cout <<hex<< P_commande << endl;
    
    Emission(&P_commande, sizeof (P_commande));
}

void Gestion_Clavier(unsigned char P_Touche) {
    switch (P_Touche) {
        case 'a': Envoi(rot_tete_droite);
            break;
        case 'z': Envoi(rot_tete_gauche);
            break;
        case 'e': Envoi(Inclinaison_tete_droite);
            break;
        case 'r': Envoi(Inclinaison_tete_gauche);
            break;
        case 't': Envoi(rot_corp_droite);
            break;
        case 'y': Envoi(rot_corp_gauche);
            break;
        case 'u': Envoi(Inclinaison_corp_droite);
            break;
        case 'i': Envoi(Inclinaison_corp_gauche);
            break;
        case 'o': Envoi(oreille_droite_max);
            break;
        case 'p': Envoi(oreille_droite_min);
            break;
        case 'q': Envoi(oreille_gauche_max);
            break;
        case 's': Envoi(oreille_gauche_min);
            break;
    };

}

int main(int argc, char** argv) {

    Init_DLL();
    Creation_Socket();
    Creation_Info_Adressage();
    Connexion();

    //setsockopt

    char Touche_Entree;
    while (1) {
        //fflush(stdin);
        Touche_Entree = getch();
        Gestion_Clavier(Touche_Entree);
        
    };


    getch();
    Deconnexion();
    Fin_DLL();
    return 0;
}
