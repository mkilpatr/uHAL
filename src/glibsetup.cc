#include "uhal/uhal.hpp"
#include "uhal/tests/tools.hpp"
#include <boost/filesystem.hpp>
#include "uhal/HwInterface.hpp"
#include <ctime>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <typeinfo>
#include <list>
#include <stdlib.h>
#include <iterator>
#include "GLIBFunctions.c"
#include <stdexcept>

int main(){
	using namespace uhal;
	using namespace std;
	std::vector<std::pair<std::string, uint32_t> > Counter_reg_1, Counter_reg_2, Counter_reg_3, Counter_reg_4;
	typedef std::vector<std::pair<std::string, uint32_t> > vector_type;

	//Make Sure the initial setting of the GLIB are set
	PKAM_Enable(1, 20);
	Delay_Time(1);
	SRAM_Disable();
	SEU(0, 1, 1, 0x55555555);
	Marker_Error(0x55555555, 0, 0x7F8, 1);
	Marker_Error_Idle(0x55555555, 0, 0x7F8, 1, 1);
	usleep(10);
	GLIBVersion();
	int GLIB1_version = 0;
	int GLIB2_version = 0;
	int GLIB3_version = 0;

	cout << "Do you want to Read(0) registers or Write(1) registers?" << endl;
	int ReadWrite;
	cin >> ReadWrite;
	if(ReadWrite){

		cout << "How many ROCs in a channel? Default: 8" << endl;
		int Num_ROCs;
		cin >> Num_ROCs;
		if(GLIB1_version == 0 && GLIB2_version == 0 && GLIB3_version == 0){
			if(Num_ROCs == 0) Set_ROCS(8);
			else Set_ROCS(Num_ROCs);
		}
		else{
			if     (GLIB1_version == 0) Set_ROCS(1, 8);
			else if(GLIB1_version == 1) Set_ROCS(1, 4);
			else if(GLIB1_version == 2) Set_ROCS(1, 2);
			if     (GLIB2_version == 0) Set_ROCS(2, 8);
			else if(GLIB2_version == 1) Set_ROCS(2, 4);
			else if(GLIB2_version == 2) Set_ROCS(2, 2);
			if     (GLIB3_version == 0) Set_ROCS(3, 8);
			else if(GLIB3_version == 1) Set_ROCS(3, 4);
			else if(GLIB3_version == 2) Set_ROCS(3, 2);
		}

		cout << "Do you want to run in SRAM Mode? 1: Yes 0: No" << endl; 
		int SRAM_Enable;
		cin >> SRAM_Enable;
		if(SRAM_Enable == 1){ 
			cout << "Have you loaded the SRAMs of each board? 1: Yes 0: No" << endl;
			int LoadSRAM;
			cin >> LoadSRAM;
			if(LoadSRAM == 0){
				cout << "What Poisson distribution do you want?" << endl;
				int Distribution;
				cin >> Distribution;
                cout << "Do you want to add PKAM events?" << endl;
                int PKAM;
                cin >> PKAM;
				SRAM1_Write_board1(GLIB1_version);
				SRAM1_Write_board2(GLIB2_version);
				SRAM1_Write_board3(GLIB3_version);
				SRAM2_Write_board1(GLIB1_version, Distribution);
				SRAM2_Write_board2(GLIB2_version, Distribution);
				SRAM2_Write_board3(GLIB3_version, Distribution);
			}
		

		SRAM_Mode();
		}
		else{
			SRAM_Disable();
		
			cout << "How many hits in each ROC? Default: 0" << endl;
			int Num_Hits;
			cin >> Num_Hits;
			Set_Num_Hits(Num_Hits);

			cout << "Do you want ADC value as Event Number? 1: Yes 0: No" << endl;
			int ADCasEvn;
			cin >> ADCasEvn;
			if(ADCasEvn == 1) ADCAsEvn(1);
			else ADCAsEvn(0);
		
			for(int i = 0; i < 8; i++){
				int col = rand() % 26;
				int row = rand() % 80;
				int even = rand() % 2;
				int ADC = rand() % 256;
				
				int DCOL = Illegal_Col(col);
				int PXL  = New_Row(row, even);
				int Hit_A = (PXL << 16) | (DCOL << 8) | ADC;
				int Hit_B = (PXL << 16) | (DCOL << 8) | ADC;

				Write_ROCs(i, Hit_A, Hit_B);						
			}
		}
	}
	else{
		GLIBL1ACounter();
		Counter_reg_1 = GLIBHeaderCountDump();
		Counter_reg_2 = GLIBTrailerCountDump();
		Counter_reg_3 = GLIBHeaderErrorCountDump();
		Counter_reg_4 = GLIBTrailerErrorCountDump();

		for(vector_type::const_iterator pos = Counter_reg_1.begin(); pos != Counter_reg_1.end(); ++pos)
			cout << pos->first << " " << pos->second << endl;
		for(vector_type::const_iterator pos = Counter_reg_2.begin(); pos != Counter_reg_2.end(); ++pos)
			cout << pos->first << " " << pos->second << endl;
		for(vector_type::const_iterator pos = Counter_reg_3.begin(); pos != Counter_reg_3.end(); ++pos)
			cout << pos->first << " " << pos->second << endl;
		for(vector_type::const_iterator pos = Counter_reg_4.begin(); pos != Counter_reg_4.end(); ++pos)
			cout << pos->first << " " << pos->second << endl;


	}
}
