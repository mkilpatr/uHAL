#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cstdlib>
using namespace std;

//Change COL to DCOL
int Illegal_Col( int col){
        int new_col = 0;
        int col_buff = 0;
        int col_buff_6 = 0;
        
        col_buff = col % 6; 
        col_buff_6 = (col - col_buff)/6;
        new_col = (col_buff_6 << 3) | col_buff;
        
        return new_col;
}

//Change Row to PXL address
int New_Row( int row, int col){
        int new_row;
        int PXL_mid_right = (row*2) % 36;
        int PXL_left = ((row*2) - PXL_mid_right)/36;
        int PXL_right = PXL_mid_right % 6;
        int PXL_mid = (PXL_mid_right - PXL_right)/6;
        int PXL_buff = 0;
        int PXL_mid_buff = (((2 - PXL_mid) + 6) % 6);
        int PXL_left_int = 0;
        if(row >= 9 )
                PXL_left_int++;
        if(row >= 27 )
                PXL_left_int++;
        if(row >= 45)
                PXL_left_int++;
        if(row >= 63)
		PXL_left_int++;
	int PXL_left_buff = (4 - PXL_left_int);

	if( col % 2 == 1)
                new_row = (PXL_left_buff << 6) | (PXL_mid_buff << 3) | ((5 - PXL_right) % 6);
        else
                new_row = (PXL_left_buff << 6) | (PXL_mid_buff << 3) | ((4 - PXL_right) % 6);

        return new_row;

}

int DCOL_Encode( int DCOL_Input){
	int DCOL = (((DCOL_Input & 0x38) >> 3)*6) + (DCOL_Input & 0x7);
	return DCOL;
}

int PXL_Encode( int PXL_Input ){
	int PXL = (((PXL_Input & 0x1C0) >> 6)*36) + (((PXL_Input & 0x38) >> 3)*6) + (PXL_Input & 0x7);
	return PXL;
}

int Random_Marker( int Which_marker ){
	int TBMH = 0x7FC;
        int ROCH = 0x7F8;
        int TBMT = 0x7FE;
        int Random_Marker = rand() % 12;
        int Marker_Buffer = 1 << Random_Marker;
	int New_Marker = 0;

	switch( Which_marker ){
		case 1: New_Marker = TBMH ^ Marker_Buffer;
		case 2: New_Marker = TBMT ^ Marker_Buffer;
		case 3: New_Marker = ROCH ^ Marker_Buffer;
	}

	return New_Marker;
}

int Random_DCOL(){
	int Random_DCOL = rand() % 26;
	int DCOL = Illegal_Col( Random_DCOL );

	return DCOL;
}

int Random_PXL( int DCOL ){
	int Random_Row = rand() % 80;
	int PXL = New_Row( Random_Row, DCOL);

	return PXL;
}

void SRAM_Disable(){
	using namespace uhal;
        ConnectionManager manager ("file://test/dummy_connections_multi_chan.xml");
        HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );
        HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_9" );
        HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
        ValWord<uint32_t> mem, mem2, mem3;

        hw.getNode("Wea").write(0);
        mem = hw.getNode("Wea").read();
        hw.dispatch();

	hw2.getNode("Wea").write(0);
        mem2 = hw2.getNode("Wea").read();
        hw2.dispatch();
	
	hw3.getNode("Wea").write(0);
        mem3 = hw3.getNode("Wea").read();
        hw3.dispatch();
}

void SRAM_Mode(){
        using namespace uhal;
        ConnectionManager manager ("file://test/dummy_connections_multi_chan.xml");
        HwInterface hw=manager.getDevice ( "GLIB.crate.slot_3" );
        HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_9" );
        HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
        ValWord<uint32_t> mem, mem2, mem3;

        hw.getNode("Wea").write(1);
        mem = hw.getNode("Wea").read();
        hw.dispatch();

        hw.getNode("sys_regs").write(0x10001);
        mem = hw.getNode("sys_regs").read();
        hw.dispatch();
        std::cout << "sys_regs: " << std::hex << mem.value() << std::endl;

        hw.getNode("SRAM2_WData").write(0x0);
        mem = hw.getNode("SRAM2_WData").read();
        hw.dispatch();

        hw.getNode("Wea2_cs").write(1);
        mem = hw.getNode("Wea2_cs").read();
        hw.dispatch();

        hw.getNode("Wea2_en").write(0);
        mem = hw.getNode("Wea2_en").read();
        hw.dispatch();
        usleep(10);

        hw2.getNode("Wea").write(1);
        mem2 = hw2.getNode("Wea").read();
        hw2.dispatch();

        hw2.getNode("sys_regs").write(0x10001);
        mem2 = hw2.getNode("sys_regs").read();
        hw2.dispatch();
        std::cout << "sys_regs: " << std::hex << mem2.value() << std::endl;

        hw2.getNode("SRAM2_WData").write(0x0);
        mem2 = hw2.getNode("SRAM2_WData").read();
        hw2.dispatch();

        hw2.getNode("Wea2_cs").write(1);
        mem2 = hw2.getNode("Wea2_cs").read();
        hw2.dispatch();

        hw2.getNode("Wea2_en").write(0);
        mem2 = hw2.getNode("Wea2_en").read();
        hw2.dispatch();
        usleep(10);

        hw3.getNode("Wea").write(1);
        mem3 = hw3.getNode("Wea").read();
        hw3.dispatch();

        hw3.getNode("sys_regs").write(0x10001);
        mem3 = hw3.getNode("sys_regs").read();
        hw3.dispatch();
        std::cout << "sys_regs: " << std::hex << mem3.value() << std::endl;
        hw3.getNode("SRAM2_WData").write(0x0);
        mem3 = hw3.getNode("SRAM2_WData").read();
        hw3.dispatch();

        hw3.getNode("Wea2_cs").write(1);
        mem3 = hw3.getNode("Wea2_cs").read();
        hw3.dispatch();

        hw3.getNode("Wea2_en").write(0);
        mem3 = hw3.getNode("Wea2_en").read();
        hw3.dispatch();
        usleep(10);
}

//Create gray code but isn't actually used in the program
std::vector< string > generateGrayArr(){
	vector<string> arr;
        vector<unsigned int> GrayCode;

	arr.push_back("0");
        arr.push_back("1");

        int n = 7;

	int i, j;
        for (i = 2; i < (1<<n); i = i<<1){
                for (j = i - 1; j >= 0; j--)
                        arr.push_back(arr[j]);
                for (j = 0; j < i; j++)
                        arr[j] = "0" + arr[j];
                for (j = i; j < 2*i; j++)
                        arr[j] = "1" + arr[j];
        }

	return arr;
}


