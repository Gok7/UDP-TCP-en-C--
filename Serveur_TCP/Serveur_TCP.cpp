
#include <conio.h>
#include <winsock2.h>
#include <cstdlib>
#include <iostream>
#include <windows.h>

#define DEBUG_SOCKET
using namespace std;

void Affiche_Erreur();

// Need to link with Ws2_32.lib
SOCKET SOCKET_Ecoute;
SOCKET SOCKET_Client;
sockaddr_in Process_Distant;
sockaddr_in Process_Local;

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

void Creation_Socket_Ecoute() {
    SOCKET_Ecoute = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (SOCKET_Ecoute == INVALID_SOCKET) {
        cout << "Erreur invalid socket" << endl;
        Affiche_Erreur();
    }
#ifdef DEBUG_SOCKET
    cout << "Valeur du socket : " << SOCKET_Ecoute << endl;
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
    Process_Distant.sin_port = htons(9999);
    Process_Distant.sin_addr.S_un.S_addr = INADDR_ANY;
    
    Process_Local.sin_family = AF_INET;
    Process_Local.sin_port = htons(1024);
    Process_Local.sin_addr.S_un.S_addr = INADDR_ANY;

    // htons rend le code portable,permet de retourner si on a besoin
    //inet_addr retourne un long.
}

void Attacher_Process_Local() {
    int L_OK = bind(SOCKET_Ecoute, (sockaddr*) & Process_Local, sizeof (Process_Local));
    if (L_OK == SOCKET_ERROR) Affiche_Erreur();

#ifdef DEBUG_SOCKET
    cout << "bind : " << L_OK << endl;
#endif
}

void Creation_Liste_Attente() {
    int Nb_Max_Connexion = 5;
    if (listen(SOCKET_Ecoute, Nb_Max_Connexion) == SOCKET_ERROR)
        Affiche_Erreur();

    cout<<"Listening on socket...\n";

}

void Accepter_Connexion() {

    cout << "Waiting for client to connect...\n";
    sockaddr_in L_info_distant;
    int L_taille=sizeof(L_info_distant);
    SOCKET_Client = accept(SOCKET_Ecoute,(sockaddr *)&L_info_distant,&L_taille);
            
    if (SOCKET_Client == INVALID_SOCKET) {
        Affiche_Erreur();
        closesocket(SOCKET_Ecoute);
        WSACleanup();
    } else
        cout << "Client connected.\n";
        cout<<inet_ntoa(L_info_distant.sin_addr)<<endl;
}


void Connexion() {

    int L_OK = connect(SOCKET_Ecoute, (sockaddr*) & Process_Distant, sizeof (Process_Distant));

    if (L_OK == SOCKET_ERROR) Affiche_Erreur();

}

void Emission(const void* P_Data, int P_taille) {

    int L_taille = send(SOCKET_Client, (char*) P_Data, P_taille, 0);
    if (L_taille == SOCKET_ERROR) {
        Affiche_Erreur();
    }
#ifdef DEBUG_SOCKET
    cout << "Valeur sendto : " << L_taille << endl;
#endif
}
int Reception() {
    char L_Data[512];
    sockaddr_in L_Info_Distant;
    int L_Taille_Info = sizeof (L_Info_Distant);
    int L_Taille = recvfrom(SOCKET_Client, (char*)L_Data, sizeof (L_Data) - 1, 0,
            (sockaddr*)&L_Info_Distant,&L_Taille_Info);
    
    if (L_Taille <= 0) Affiche_Erreur();
    else {
        L_Data[L_Taille] = 0;
        cout << L_Data;
    }
#ifdef DEBUG_SOCKET
    cout << "recvfrom - Taille : " << L_Taille << endl;
#endif
    return L_Taille;
}
/*
void Reception_Asynchrone() {
    char L_Data[11];
    int L_Taille;
    long Network_Event;
    WSAEVENT L_Event_Socket;
    WSANETWORKEVENTS L_Resultat_Event;

    L_Event_Socket = WSACreateEvent();

    WSAEventSelect(SOCKET_Ecoute, L_Event_Socket, FD_READ);

    bool L_fin = false;
    do {
        WaitForSingleObject(L_Event_Socket, INFINITE); //attent d'être reveillé

        WSAEnumNetworkEvents(SOCKET_Ecoute, L_Event_Socket, &L_Resultat_Event);
        //si réveillé pourquoi ?

        if ((L_Resultat_Event.lNetworkEvents & FD_READ) == FD_READ) {
            cout << "FD_READ" << endl;
            do {
                L_Taille = recv(SOCKET_Ecoute, L_Data, sizeof (L_Data) - 1, 0);
                if (L_Taille > 0) {
                    L_Data[L_Taille] = 0;
                    cout << L_Data;
                }
            } while (L_Taille > 0);
        }
        if ((L_Resultat_Event.lNetworkEvents & FD_CLOSE) == FD_CLOSE) {
            cout << "FD_CLOSE" << endl;
            L_fin = true;
        }
    } while (L_fin == false);

}
*/
void Deconnexion_Client() {
    shutdown(SOCKET_Client, SD_BOTH);
    closesocket(SOCKET_Client);
}

void Fin_Serveur() {
    closesocket(SOCKET_Ecoute);
}
int main(int argc, char** argv) {

    Init_DLL();
    Creation_Socket_Ecoute();
    Creation_Info_Adressage();
    Attacher_Process_Local();
    Creation_Liste_Attente();
    Accepter_Connexion();
    Reception();
    Deconnexion_Client();
    Fin_Serveur();
    Fin_DLL();
    return 0;
}
