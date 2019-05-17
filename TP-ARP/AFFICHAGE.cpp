#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

void Affiche_Dump(const unsigned char* P_Dump, int P_Taille)
{  
    for (int L_Index=0; L_Index<P_Taille; L_Index++){       
        cout<<hex<<setw(2)<<setfill('0')<<(int)P_Dump[L_Index]<<" ";
        if ( (L_Index%16) == 15 ) cout<<endl;
    }
    cout<<endl;
  
}
//-----------------------------------------------------------------------------
void Affiche_Adresse_IP(const unsigned char* P_Dump)
{      
  cout<<dec<<(int)P_Dump[0]<<"."<<(int)P_Dump[1]<<"."<<(int)P_Dump[2]<<"."<<(int)P_Dump[3];
 }
//-----------------------------------------------------------------------------
void Affiche_MAC(const unsigned char* P_Dump)
{  
    for (int L_Index=0; L_Index<6; L_Index++){       
        cout<<hex<<setw(2)<<setfill('0')<<(int)P_Dump[L_Index];
        if (L_Index<5) cout<<"-";
    }
}


