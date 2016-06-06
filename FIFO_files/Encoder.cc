#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <typeinfo>
#include <list>
#include <stdlib.h>
#include <iterator>
#include <sstream>

using namespace std;

template<class T>
void write_vector(const std::vector<T>& vector) 
{
	std::copy(vector.begin(), vector.end(), std::ostream_iterator<T>(std::cout, " "));
	std::cout << '\n';
}

int main(int argc, char* argv[] )
{
    int Size = atoi(argv[1]);
    int Event_CHA[Size];
    int Event_CHB[Size];
    int Interleave[Size*2];
    int Five_Bit[Size*2];
    int Final_Event[Size*2];
    int CHA, CHB;

    for(int i = 0; i < Size; i++){
	cout << "Enter data A: ";
	cin >> std::hex >> CHA;
	Event_CHA[i] = CHA; 
    }

    for(int i = 0; i < Size; i++){
        cout << "Enter data B: ";
        cin >> std::hex >> CHB;
        Event_CHB[i] = CHB;
    }

    int j = 0;

    for(int i = 0; i < Size; i++ )
    {    

//	std::cout << "Event_CHA[" << std::dec << i << "] = " << std::hex << Event_CHA[i] << std::endl;
//	std::cout << "Event_CHB[" << std::dec << i << "] = " << std::hex << Event_CHB[i] << std::endl;

	Interleave[j] = (Event_CHA[i] & 0x8) | ((~(Event_CHB[i]) & 0x8) >> 1) | ((Event_CHA[i] & 0x4) >> 1) | ((~(Event_CHB[i]) & 0x4) >> 2); 
	Interleave[j + 1] = ((Event_CHA[i] & 0x2) << 2) | ((~(Event_CHB[i]) & 0x2) << 1) | ((Event_CHA[i] & 0x1) << 1) | (~(Event_CHB[i]) & 0x1);
	j = j + 2;

    }

    /*for(int i = 0; i < Size*2; i++){
        cout << "Interleave[" << std::dec << i << "] = " << std::hex << Interleave[i] << endl;
    }*/

    for(int i = 0; i < Size*2; i++){
	if(Interleave[i] == 0xA && ( Interleave[i + 1] == 0x1 ) | ( Interleave[i + 1] == 0x4 ) | ( Interleave[i + 1] == 0x5 ) | ( Interleave[i + 1] == 0x6 ) | ( Interleave[i + 1] == 0x7 )){
		Five_Bit[i] = 0x16;
	}
	else if(Interleave[i] == 0xA){
		Five_Bit[i] = 0x10;
	}
	else if(Interleave[i] == 0x0){
                Five_Bit[i] = 0x1E;
        }
	else if(Interleave[i] == 0x1){
                Five_Bit[i] = 0x09;
        }
	else if(Interleave[i] == 0x2){
                Five_Bit[i] = 0x14;
        }
	else if(Interleave[i] == 0x3){
                Five_Bit[i] = 0x15;
        }
	else if(Interleave[i] == 0x4){
                Five_Bit[i] = 0x0A;
        }
	else if(Interleave[i] == 0x5){
                Five_Bit[i] = 0x0B;
        }
	else if(Interleave[i] == 0x6){
                Five_Bit[i] = 0x0E;
        }
	else if(Interleave[i] == 0x7){
                Five_Bit[i] = 0x0F;
        }
	else if(Interleave[i] == 0x8){
                Five_Bit[i] = 0x12;
        }
	else if(Interleave[i] == 0x9){
                Five_Bit[i] = 0x13;
        }
	else if(Interleave[i] == 0xB){
                Five_Bit[i] = 0x17;
        }
	else if(Interleave[i] == 0xC){
                Five_Bit[i] = 0x1A;
        }
	else if(Interleave[i] == 0xD){
                Five_Bit[i] = 0x1B;
        }
	else if(Interleave[i] == 0xE){
                Five_Bit[i] = 0x1C;
        }
	else if(Interleave[i] == 0xF){
                Five_Bit[i] = 0x1D;
        }
    }

    /*for(int i = 0; i < Size*2; i++){
	cout << "Five_Bit[" << std::dec << i << "] = " << std::hex << Five_Bit[i] << endl;
    }*/

    int Buffer[6] = { 0, 0, 0, 0, 0, 0};
    for(int i = 0; i < Size*2; i++){
	//int Buffer[6];
	int k = 0;
	if( (Five_Bit[i] & 0x10) == 0x0 ){
		if(Buffer[5] == (Five_Bit[i] & 0x10) >> 4){
			Buffer[0] = (Five_Bit[i] & 0x10) >> 4;
		}
		else Buffer[0] = Buffer[5];
	}
	else{ 
		if(Buffer[5] == (Five_Bit[i] & 0x10) >> 4){
			Buffer[0] = (~(Five_Bit[i]) & 0x10) >> 4;
		}
		else Buffer[0] = (~Buffer[5]) & 0x1;
	}
	if( (Five_Bit[i] & 0x8) == 0x0){
		Buffer[1] = Buffer[0];
	}
	else Buffer[1] = ~(Buffer[0]) & 0x1;
	if( (Five_Bit[i] & 0x4) == 0x0){
		Buffer[2] = Buffer[1];
	}
	else Buffer[2] = ~(Buffer[1]) & 0x1;
	if( (Five_Bit[i] & 0x2) == 0x0){
                Buffer[3] = Buffer[2];
	}
        else Buffer[3] = ~(Buffer[2]) & 0x1;
	if( (Five_Bit[i] & 0x1) == 0x0){
                Buffer[4] = Buffer[3];
        }
	else Buffer[4] = ~(Buffer[3]) & 0x1;
	
	Buffer[5] = Buffer[4];

	Final_Event[i] = (Buffer[0] << 4) | (Buffer[1] << 3) | (Buffer[2] << 2) | (Buffer[3] << 1) | (Buffer[4]);	
	//cout << std::hex << Buffer[0] << endl;
	//cout << std::hex << Buffer[1] << endl;
	//cout << std::hex << Buffer[2] << endl;
	//cout << std::hex << Buffer[3] << endl;
	//cout << std::hex << Buffer[4] << endl;
    }

    for(int i = 0; i < Size*2; i++){
	cout << std::hex << Final_Event[i] << " ";
    }
	cout << endl;

    exit(0);
}
