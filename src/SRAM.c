#include <ctime>
#include <cstring>
#include "TCanvas.h"
#include "TH1.h"
#include "TH1F.h"
#include "TColor.h"
#include "TStyle.h"
#include "TFile.h"
#include "TRandom.h"

std::vector<unsigned int> TextLoop(){
    std::vector<unsigned int> StoreHits;
    ifstream in("Pixel_address.txt");

    if(!in){
        cout << "Cannot open input file." << endl;
    }

    char str[255];
    while(in.getline(str, 255)){
        StoreHits.push_back(strtol(str, NULL, 16));
    }

    in.close();

    return StoreHits;
}

void SRAM_Plots(vector<unsigned int>& Hits){

	TFile * fout;
	fout = new TFile("SRAM.root", "Recreate");

	TH1F* h1 = new TH1F("h1","Num Hits per ROC",11,-0.5,10.5);
	h1->SetDirectory(fout);
	h1->GetXaxis()->SetTitle("Hits");

	for(uint32_t j = 0; j < Hits.size(); j++)
		h1->Fill(Hits[ j ]);

	h1->Write();

	TCanvas* Total = new TCanvas("Total","Distribution of Hits",200,10,700,500);
	Total->cd();
	h1->Draw();
	Total->SaveAs("SRAM_Hits.pdf");
}

void Clear_SRAM_FIFO(){
	using namespace uhal;
    disableLogging();
	ConnectionManager manager ("file://settings/GLIB_connections.xml");
	HwInterface hw=manager.getDevice ("GLIB.crate.slot_9");
	HwInterface hw2=manager.getDevice ("GLIB.crate.slot_10");
	HwInterface hw3=manager.getDevice ("GLIB.crate.slot_11");

	hw.getNode("SRAM_FIFO_Clear").write(1);
	hw.dispatch();

	hw.getNode("SRAM_FIFO_Clear").write(0);
	hw.dispatch();

	hw2.getNode("SRAM_FIFO_Clear").write(1);
	hw2.dispatch();

	hw2.getNode("SRAM_FIFO_Clear").write(0);
	hw2.dispatch();

	hw3.getNode("SRAM_FIFO_Clear").write(1);
	hw3.dispatch();

	hw3.getNode("SRAM_FIFO_Clear").write(0);
	hw3.dispatch();	
}

void Partition_Mode( int Partition ){
	using namespace uhal;
    disableLogging();
	ConnectionManager manager ("file://settings/GLIB_connections.xml");
	HwInterface hw=manager.getDevice ("GLIB.crate.slot_9");
	HwInterface hw2=manager.getDevice ("GLIB.crate.slot_10");
	HwInterface hw3=manager.getDevice ("GLIB.crate.slot_11");

    hw.getNode("SRAM_partition").write(Partition);
    hw.dispatch();

    hw2.getNode("SRAM_partition").write(Partition);
    hw2.dispatch();

    hw3.getNode("SRAM_partition").write(Partition);
    hw3.dispatch();

}

void SRAM1_Write_board1(int GLIB){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );

    srand(time(NULL));

	std::vector<unsigned int> WriteData;
    int col = 0, dcol = 0, row = 0, PXL = 0, DCOL = 0, ADC = 0, col_L1 = 0, row_L1 = 0;

    for(uint32_t i = 0; i < 0x1FFFFF; i++){
		dcol = rand() % 26;
		col = rand() % 2;
		col_L1 = rand() % 52;
		row = rand() % 80;
		row_L1 = rand() % 160;
		PXL = New_Row( row, col );
		DCOL = Illegal_Col( dcol );
		ADC = i % 256;
		if(GLIB == 0) WriteData.push_back(PXL << 16 | DCOL << 8 | ADC);
		else WriteData.push_back(row_L1 << 16 | dcol << 8 | ADC);
		
		if( i % 0x10000 == 0) std::cout << "SRAM1_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;
	}

    hw.getNode("sram1_user_logic").write(0);
    hw.dispatch();
        
	hw.getNode("SRAM1_block").writeBlock(WriteData);
    hw.dispatch();
}

void SRAM1_Write_board1(int GLIB, int start, vector<unsigned int>& StoreHits){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );

    srand(time(NULL));

    std::vector<unsigned int> WriteData;
    int col = 0, dcol = 0, row = 0, PXL = 0, DCOL = 0, ADC = 0, col_L1 = 0, row_L1 = 0;

    for(uint32_t i = start; i < (start + 0x1FFFFF); i++){
        PXL = New_Row( StoreHits[i] & 0x1FF0000, StoreHits[i] & 0x3F00);
        DCOL = Illegal_Col( StoreHits[i] & 0x3F00);
        ADC = i % 256;

        if(GLIB == 0) WriteData.push_back(PXL << 16 | DCOL << 8 | ADC);
        else WriteData.push_back(StoreHits[i] | ADC);

    }

    for(int i = 0; i != WriteData.size(); i++) if( i % 0x10000 == 0) std::cout << "SRAM1_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;

    hw.getNode("sram1_user_logic").write(0);
    hw.dispatch();

    hw.getNode("SRAM1_block").writeBlock(WriteData);
    hw.dispatch();
}


void SRAM1_Write_board2(int GLIB){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_10" );
    ValWord<uint32_t> mem;

    srand(time(NULL));

	std::vector<unsigned int> WriteData;
    int col = 0, dcol = 0, row = 0, PXL = 0, DCOL = 0, ADC = 0, col_L1 = 0, row_L1 = 0;

    for(uint32_t i = 0; i < 0x1FFFFF; i++){
		dcol = rand() % 26;
		col = rand() % 2;
		row = rand() % 80;
		col_L1 = rand() % 52;
		row_L1 = rand() % 160;
		PXL = New_Row( row, col );
		DCOL = Illegal_Col( dcol );
		ADC = i % 256;
		if(GLIB == 0) WriteData.push_back(PXL << 16 | DCOL << 8 | ADC);
		else WriteData.push_back(row_L1 << 16 | dcol << 8 | ADC);
		
		if( i % 0x10000 == 0) std::cout << "SRAM1_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;
	}

    hw.getNode("sram1_user_logic").write(0);
    hw.dispatch();

    hw.getNode("SRAM1_block").writeBlock(WriteData);
    hw.dispatch();

	//SRAM_Plots(Hits);
}

void SRAM1_Write_board2(int GLIB, int start, vector<unsigned int>& StoreHits){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_10" );

    srand(time(NULL));

    std::vector<unsigned int> WriteData;
    int col = 0, dcol = 0, row = 0, PXL = 0, DCOL = 0, ADC = 0, col_L1 = 0, row_L1 = 0;

    for(uint32_t i = start; i < (start + 0x1FFFFF); i++){
        PXL = New_Row( StoreHits[i] & 0x1FF0000, StoreHits[i] & 0x3F00);
        DCOL = Illegal_Col( StoreHits[i] & 0x3F00);
        ADC = i % 256;

        if(GLIB == 0) WriteData.push_back(PXL << 16 | DCOL << 8 | ADC);
        else WriteData.push_back(StoreHits[i] | ADC);

        //if( i % 0x10000 == 0) std::cout << "SRAM1_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;
    }

    for(int i = 0; i != WriteData.size(); i++) if( i % 0x10000 == 0) std::cout << "SRAM1_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;

    hw.getNode("sram1_user_logic").write(0);
    hw.dispatch();

    hw.getNode("SRAM1_block").writeBlock(WriteData);
    hw.dispatch();
}

void SRAM1_Write_board3(int GLIB){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem;

    srand(time(NULL));
	std::vector<unsigned int> WriteData;
    int col = 0, dcol = 0, row = 0, PXL = 0, DCOL = 0, ADC = 0, col_L1 = 0, row_L1 = 0;

    for(uint32_t i = 0; i < 0x1FFFFF; i++){
		dcol = rand() % 26;
		col = rand() % 2;
		row = rand() % 80;
		col_L1 = rand() % 52;
		row_L1 = rand() % 160;
		PXL = New_Row( row, col );
		DCOL = Illegal_Col( dcol );
		ADC = i % 256;
		if(GLIB == 0) WriteData.push_back(PXL << 16 | DCOL << 8 | ADC);
		else WriteData.push_back(row_L1 << 16 | dcol << 8 | ADC);
		
		if( i % 0x10000 == 0) std::cout << "SRAM1_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;
	}

    hw.getNode("sram1_user_logic").write(0);
    hw.dispatch();

    hw.getNode("SRAM1_block").writeBlock(WriteData);
    hw.dispatch();

	//SRAM_Plots(Hits);
}

void SRAM1_Write_board3(int GLIB, int start, vector<unsigned int>& StoreHits){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_11" );

    srand(time(NULL));

    std::vector<unsigned int> WriteData;
    int col = 0, dcol = 0, row = 0, PXL = 0, DCOL = 0, ADC = 0, col_L1 = 0, row_L1 = 0;

    for(uint32_t i = start; i < (start + 0x1FFFFF); i++){
        PXL = New_Row( StoreHits[i] & 0x1FF0000, StoreHits[i] & 0x3F00);
        DCOL = Illegal_Col( StoreHits[i] & 0x3F00);
        ADC = i % 256;

        if(GLIB == 0) WriteData.push_back(PXL << 16 | DCOL << 8 | ADC);
        else WriteData.push_back(StoreHits[i] | ADC);

        //if( i % 0x10000 == 0) std::cout << "SRAM1_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;
    }
    
    for(int i = 0; i != WriteData.size(); i++) if( i % 0x10000 == 0) std::cout << "SRAM1_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;

    hw.getNode("sram1_user_logic").write(0);
    hw.dispatch();

    hw.getNode("SRAM1_block").writeBlock(WriteData);
    hw.dispatch();
}


void SRAM2_Write_board1(int GLIB, float Distribution){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
    ValWord<uint32_t> mem;
    int hit[8];

	std::vector<unsigned int> Hits;
	std::vector<unsigned int> WriteData;

    hw.getNode("SRAM_partition").write(0);

    for(uint32_t i = 0; i < 0x1FFFFF; i++){
		for(int j = 0; j < 8; j++){
			hit[j] = gRandom->Poisson(Distribution);
			Hits.push_back(hit[j]);
		}
		
		if(GLIB == 0) WriteData.push_back(hit[0] << 28 | hit[1] << 24 | hit[2] << 20 | hit[3] << 16 | hit[4] << 12 | hit[5] << 8 | hit[6] << 4 | hit[7]);
		if(GLIB == 1) WriteData.push_back(hit[0] << 24 | hit[1] << 16 | hit[2] << 8 | hit[3]);
		if(GLIB == 2) WriteData.push_back(hit[0] << 16 | hit[1]);
		if( i % 0x10000 == 0) std::cout << "SRAM2_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;
	}
    hw.getNode("sram2_user_logic").write(0);
    hw.getNode("flash_select").write(0);
    hw.dispatch();

    hw.getNode("SRAM2_block").writeBlock(WriteData);
    hw.dispatch();

	//SRAM_Plots(Hits);
}

void SRAM2_Write_board1(int GLIB, vector<float>& Distribution, int partition){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
    ValWord<uint32_t> mem;
    int hit[8];

	std::vector<unsigned int> Hits;
	std::vector<unsigned int> WriteData;

    hw.getNode("SRAM_partition").write(partition);

    int divide = 0;
    if( partition == 15) divide = 16;
    else divide = partition;

    uint32_t new_dist = 0x1FFFFF / divide;
    int dist_index = 0;
        for(uint32_t i = 0; i < 0x1FFFFF; i++){
		if(i % new_dist == 0) dist_index++;
		for(int j = 0; j < 8; j++){
			hit[j] = gRandom->Poisson(Distribution[dist_index - 1]);
			Hits.push_back(hit[j]);
		}
		
		if(GLIB == 0) WriteData.push_back(hit[0] << 28 | hit[1] << 24 | hit[2] << 20 | hit[3] << 16 | hit[4] << 12 | hit[5] << 8 | hit[6] << 4 | hit[7]);
		if(GLIB == 1) WriteData.push_back(hit[0] << 24 | hit[1] << 16 | hit[2] << 8 | hit[3]);
		if(GLIB == 2) WriteData.push_back(hit[0] << 16 | hit[1]);
		if( i % 0x10000 == 0) std::cout << "SRAM2_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;
	}
    hw.getNode("sram2_user_logic").write(0);
    hw.getNode("flash_select").write(0);
    hw.dispatch();

    hw.getNode("SRAM2_block").writeBlock(WriteData);
    hw.dispatch();

	//SRAM_Plots(Hits);
}

void SRAM2_Write_board2(int GLIB, float Distribution){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_10" );
    ValWord<uint32_t> mem;

    int hit[8];

	std::vector<unsigned int> Hits;
	std::vector<unsigned int> WriteData;

    hw.getNode("SRAM_partition").write(0);

    for(uint32_t i = 0; i < 0x1FFFFF; i++){
		for(int j = 0; j < 8; j++){
			hit[j] = gRandom->Poisson(Distribution);
			Hits.push_back(hit[j]);
		}
		
		if(GLIB == 0) WriteData.push_back(hit[0] << 28 | hit[1] << 24 | hit[2] << 20 | hit[3] << 16 | hit[4] << 12 | hit[5] << 8 | hit[6] << 4 | hit[7]);
		if(GLIB == 1) WriteData.push_back(hit[0] << 24 | hit[1] << 16 | hit[2] << 8 | hit[3]);
		if(GLIB == 2) WriteData.push_back(hit[0] << 16 | hit[1]);
		if( i % 0x10000 == 0) std::cout << "SRAM2_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;
	}
    hw.getNode("sram2_user_logic").write(0);
    hw.getNode("flash_select").write(0);
    hw.dispatch();

    hw.getNode("SRAM2_block").writeBlock(WriteData);
    hw.dispatch();

	//SRAM_Plots(Hits);
}

void SRAM2_Write_board2(int GLIB, vector<float>& Distribution, int partition){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_10" );
    ValWord<uint32_t> mem;
    int hit[8];

	std::vector<unsigned int> Hits;
	std::vector<unsigned int> WriteData;

    hw.getNode("SRAM_partition").write(partition);

    int divide = 0;
    if( partition == 15) divide = 16;
    else divide = partition;

    uint32_t new_dist = 0x1FFFFF / divide;
    int dist_index = 0;
        for(uint32_t i = 0; i < 0x1FFFFF; i++){
		if(i % new_dist == 0) dist_index++;
		for(int j = 0; j < 8; j++){
			hit[j] = gRandom->Poisson(Distribution[dist_index - 1]);
			Hits.push_back(hit[j]);
		}
		
		if(GLIB == 0) WriteData.push_back(hit[0] << 28 | hit[1] << 24 | hit[2] << 20 | hit[3] << 16 | hit[4] << 12 | hit[5] << 8 | hit[6] << 4 | hit[7]);
		if(GLIB == 1) WriteData.push_back(hit[0] << 24 | hit[1] << 16 | hit[2] << 8 | hit[3]);
		if(GLIB == 2) WriteData.push_back(hit[0] << 16 | hit[1]);
		if( i % 0x10000 == 0) std::cout << "SRAM2_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;
	}
    hw.getNode("sram2_user_logic").write(0);
    hw.getNode("flash_select").write(0);
    hw.dispatch();

    hw.getNode("SRAM2_block").writeBlock(WriteData);
    hw.dispatch();

	//SRAM_Plots(Hits);
}

void SRAM2_Write_board3(int GLIB, float Distribution){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem;

    int hit[8];

	std::vector<unsigned int> Hits;
	std::vector<unsigned int> WriteData;

    hw.getNode("SRAM_partition").write(0);

    for(uint32_t i = 0; i < 0x1FFFFF; i++){
		for(int j = 0; j < 8; j++){
			hit[j] = gRandom->Poisson(Distribution);
			Hits.push_back(hit[j]);
		}
		
		if(GLIB == 0) WriteData.push_back(hit[0] << 28 | hit[1] << 24 | hit[2] << 20 | hit[3] << 16 | hit[4] << 12 | hit[5] << 8 | hit[6] << 4 | hit[7]);
		if(GLIB == 1) WriteData.push_back(hit[0] << 24 | hit[1] << 16 | hit[2] << 8 | hit[3]);
		if(GLIB == 2) WriteData.push_back(hit[0] << 16 | hit[1]);
		if( i % 0x10000 == 0) std::cout << "SRAM2_addr: " << std::hex << i << " write: " << WriteData[i] << std::endl;
	}
    hw.getNode("sram2_user_logic").write(0);
    hw.getNode("flash_select").write(0);
    hw.dispatch();

    hw.getNode("SRAM2_block").writeBlock(WriteData);
    hw.dispatch();

	//SRAM_Plots(Hits);
}

void SRAM2_Write_board3(int GLIB, vector<float>& Distribution, int partition){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem;
    int hit[8];

	std::vector<unsigned int> Hits;
	std::vector<unsigned int> WriteData;

    hw.getNode("SRAM_partition").write(partition);

    int divide = 0;
    if( partition == 15) divide = 16;
    else divide = partition;

    uint32_t new_dist = 0x1FFFFF / divide;
    int dist_index = 0;
        for(uint32_t i = 0; i < 0x1FFFFF; i++){
		if(i % new_dist == 0) dist_index++;
		for(int j = 0; j < 8; j++){
			hit[j] = gRandom->Poisson(Distribution[dist_index - 1]);
			Hits.push_back(hit[j]);
		}
		
		if(GLIB == 0) WriteData.push_back(hit[0] << 28 | hit[1] << 24 | hit[2] << 20 | hit[3] << 16 | hit[4] << 12 | hit[5] << 8 | hit[6] << 4 | hit[7]);
		if(GLIB == 1) WriteData.push_back(hit[0] << 24 | hit[1] << 16 | hit[2] << 8 | hit[3]);
		if(GLIB == 2) WriteData.push_back(hit[0] << 16 | hit[1]);
		if( i % 0x10000 == 0) std::cout << "SRAM2_addr: " << std::hex << i << " write: " << WriteData[i] << " Dist index: " << dist_index - 1 << std::endl;
	}
    hw.getNode("sram2_user_logic").write(0);
    hw.getNode("flash_select").write(0);
    hw.dispatch();

    hw.getNode("SRAM2_block").writeBlock(WriteData);
    hw.dispatch();

	//SRAM_Plots(Hits);
}


