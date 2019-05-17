#include "header1_client_TCP.h"
#include <conio.h>
SOCKET H_SOCKET;
sockaddr_in Process_Distant;

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

void Creation_Info_Adressage(char* P_addresse_IP,char *  P_Port) {
    
    //hostent* L_Info_Host=gethostbyname(P_Nom_Host);
   // long L_Adresse;
    //int L_Port;
    
    //if(L_Info_Host) L_Adresse=*(u_long *)L_Info_Host->h_addr_list[0];
   // else 
        //L_Adresse = inet_addr("127.0.0.1");
    
    //servent* L_Info_Service=getservbyname(P_Nom_Service,"TCP");
    
    
    //if(L_Info_Service) L_Port = L_Info_Service->s_port;
    //else L_Port=htons(80);
    
    Process_Distant.sin_family = AF_INET;
    Process_Distant.sin_port = htons(9999);
    Process_Distant.sin_addr.S_un.S_addr = inet_addr(P_addresse_IP);
    
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

int Reception() {
    char L_Data[10];
    sockaddr_in L_Info_Distant;
    int L_Taille_Info = sizeof (L_Info_Distant);

    int L_Taille = recv(H_SOCKET, L_Data, sizeof (L_Data) - 1, 0);
    //si aucune erreur : retourne nb octer reçu

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

void Reception_Asynchrone() {
    char L_Data[11];
    int L_Taille;
    long Network_Event;
    WSAEVENT L_Event_Socket;
    WSANETWORKEVENTS L_Resultat_Event;

    L_Event_Socket = WSACreateEvent();

    WSAEventSelect(H_SOCKET, L_Event_Socket,FD_READ);
    
    bool L_fin=false;
    do {
        WaitForSingleObject(L_Event_Socket, INFINITE);//attent d'être reveillé

        WSAEnumNetworkEvents(H_SOCKET, L_Event_Socket, &L_Resultat_Event);
        //si réveillé pourquoi ?
        
        if((L_Resultat_Event.lNetworkEvents & FD_READ) == FD_READ){
            cout<<"FD_READ"<<endl;
            do{
                L_Taille = recv(H_SOCKET,L_Data, sizeof (L_Data) - 1, 0);
                if(L_Taille>0){
                    L_Data[L_Taille] = 0;
                    cout << L_Data;
                }
            }while(L_Taille>0);
        }
        if((L_Resultat_Event.lNetworkEvents & FD_CLOSE) == FD_CLOSE){
            cout<<"FD_CLOSE"<<endl;
            L_fin=true;
        }
    }while(L_fin==false);

}

int main(int argc, char** argv) {

    Init_DLL();
    Creation_Socket();
    Creation_Info_Adressage("10.1.0.107","80");
    Connexion();
    const char L_Message[] = "hellomoto";

    Emission(L_Message, strlen(L_Message));
    int L_OK; //quand la taille = 0, la personne s'est déconnecté
//    do {
//        L_OK = Reception();
//
//    } while (L_OK > 0);
    Reception_Asynchrone();
    getch();
    Deconnexion();
    Fin_DLL();
    return 0;
}

//gethostbyname

//