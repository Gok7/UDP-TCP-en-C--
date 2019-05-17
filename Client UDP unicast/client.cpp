#include "header1_client_udp_unicast.h"

SOCKET H_SOCKET;
sockaddr_in Process_Distant;

void Init_DLL(){
    WSADATA L_Info_DLL;
    
    if(WSAStartup(MAKEWORD(2,0),&L_Info_DLL)!=0){
        exit;
    }//je veux utiliser la dll a partir 
    //du moment où elle est compatible avec la version 2

#ifdef DEBUG_SOCKET
    cout<<L_Info_DLL.szDescription<<endl;
    cout<<L_Info_DLL.szSystemStatus<<endl;
#endif
}

void Fin_DLL(){
    
    int L_OK=WSACleanup();
    
#ifdef DEBUG_SOCKET
    if(L_OK==0) cout<<"WSACLEANUP : OK"<<endl;
    else cout<<"WSACLEANUP : ERROR"<<endl;
#endif
}

void Creation_Socket(){
    H_SOCKET=socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
    // parameters- 1 :in af adress family. 
    //2 : type UDP. 3 : sous protocol
    
    if(H_SOCKET==INVALID_SOCKET) {
        cout<<"Erreur invalid socket"<<endl;
        Affiche_Erreur();
    }
#ifdef DEBUG_SOCKET
    cout<<"Valeur du socket : "<<H_SOCKET<<endl;
#endif
}

void Affiche_Erreur(){
    DWORD L_Code_Erreur = GetLastError();
    char L_Message[512];
    
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,
            L_Code_Erreur,0,L_Message,511,NULL);
    //1 : flag specifiant l'origine du message. 2 : valeur par defaut 
    //3 : code erreur. 4 : avoir le message en chaine de car
    //5 : adresse bloc memoire du message. 6 : maximum de car
    //7 : info supplementaire dans message
    
    cerr<<"Erreur"<<L_Code_Erreur<<" : "<<L_Message<<endl;
}

void Creation_Info_Adressage(){
    Process_Distant.sin_family = AF_INET;
    Process_Distant.sin_port = htons(9999);
    Process_Distant.sin_addr.S_un.S_addr = inet_addr("192.93.205.255");
    // htons rend le code portable,permet de retourner si on a besoin
    //inet_addr retourne un long. 252 est l'adresse du prof
}

void Test_Emission(){
    
    typedef struct T_Coordonnees{
        int X;
        int Y;
        char z;
    }T_COORDONNEES;
    
    T_Coordonnees L_Position;
    
    L_Position.X=10;
    L_Position.Y=20;
    
    int L_taille=sendto(H_SOCKET,"coucou c'est moi",16,0,(sockaddr*)&Process_Distant, 
            sizeof(Process_Distant));
    if (L_taille==SOCKET_ERROR){
        Affiche_Erreur();
    }
#ifdef DEBUG_SOCKET
    cout<<"Valeur sendto : "<<L_taille<<endl;
#endif
}

void Emission(const void* P_Data,int P_taille){
    
    int L_taille=sendto(H_SOCKET,(char*) P_Data,P_taille,0,
            (sockaddr*)&Process_Distant, 
            sizeof(Process_Distant));
    if (L_taille==SOCKET_ERROR){
        Affiche_Erreur();
    }
#ifdef DEBUG_SOCKET
    cout<<"Valeur sendto : "<<L_taille<<endl;
#endif
}
int main(int argc, char** argv) {

    Init_DLL();
    Creation_Socket();
    Creation_Info_Adressage();
    while(1){
            Emission("je suis ton pere",30);
            Sleep(500);

    }


    Fin_DLL();
    return 0;
}

