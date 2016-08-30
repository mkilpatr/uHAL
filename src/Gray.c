#include <iostream>
#include <string>
#include <vector>
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


