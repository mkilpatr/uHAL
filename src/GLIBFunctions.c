#include "Gray.c"
#include "SRAM.c"

int Marker_Read(){
    using namespace uhal;
    using namespace std;
    disableLogging();
    
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    ValWord<uint32_t> mem[48];
    
    string Chan[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    string fiber ("Fiber_");    
    string Marker_Type_chA      ("Marker_Error.chA_Type"     ); 
    string Marker_Type_chB      ("Marker_Error.chB_Type"     );
    string MarkerErrorTypechA;
    string MarkerErrorTypechB;
    int MarkerCount = 0;
    
    for (int i = 0; i < 8; i++){
         MarkerErrorTypechA  = fiber + Chan[i] + "." + Marker_Type_chA;
         MarkerErrorTypechB  = fiber + Chan[i] + "." + Marker_Type_chB;
         
         mem[i*6 + 0] = hw.getNode(MarkerErrorTypechA  ).read();
         mem[i*6 + 1] = hw.getNode(MarkerErrorTypechB  ).read();
         hw.dispatch();

         mem[i*6 + 2] = hw2.getNode(MarkerErrorTypechA ).read();
         mem[i*6 + 3] = hw2.getNode(MarkerErrorTypechB ).read();
         hw2.dispatch();
         
         mem[i*6 + 4] = hw3.getNode(MarkerErrorTypechA ).read();
         mem[i*6 + 5] = hw3.getNode(MarkerErrorTypechB ).read();
         hw3.dispatch();
    }
    
    for(int i = 0; i < 48; i++){
        if(mem[i].value() != 0) MarkerCount++;
    }
    return MarkerCount;
}

int Marker_Idle_Read(){
    using namespace uhal;
    using namespace std;
    disableLogging();
    
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    ValWord<uint32_t> mem[48];
    
    string Chan[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    string fiber ("Fiber_");    
    string Marker_Type_chA      ("Marker_Error_Custom.chA_Type"     ); 
    string Marker_Type_chB      ("Marker_Error_Custom.chB_Type"     );
    string MarkerErrorTypechA;
    string MarkerErrorTypechB;
    int MarkerCount = 0;
    
    for (int i = 0; i < 8; i++){
         MarkerErrorTypechA  = fiber + Chan[i] + "." + Marker_Type_chA;
         MarkerErrorTypechB  = fiber + Chan[i] + "." + Marker_Type_chB;
         
         mem[i*6 + 0] = hw.getNode(MarkerErrorTypechA  ).read();
         mem[i*6 + 1] = hw.getNode(MarkerErrorTypechB  ).read();
         hw.dispatch();

         mem[i*6 + 2] = hw2.getNode(MarkerErrorTypechA ).read();
         mem[i*6 + 3] = hw2.getNode(MarkerErrorTypechB ).read();
         hw2.dispatch();
         
         mem[i*6 + 4] = hw3.getNode(MarkerErrorTypechA ).read();
         mem[i*6 + 5] = hw3.getNode(MarkerErrorTypechB ).read();
         hw3.dispatch();
    }
    
    for(int i = 0; i < 48; i++){
        if(mem[i].value() != 0) MarkerCount++;
    }
    return MarkerCount;
}

int Marker_Idle_Counter_Read(){
    using namespace uhal;
    using namespace std;
    disableLogging();
    
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    ValWord<uint32_t> mem[48];
    
    string Chan[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    string fiber ("Fiber_");    
    string Marker_Clock_chA      ("Marker_Error_chA_Clock_Custom_Idle"     ); 
    string Marker_Clock_chB      ("Marker_Error_chB_Clock_Custom_Idle"     );
    string MarkerErrorClockchA;
    string MarkerErrorClockchB;
    int MarkerCount = 0;
    
    for (int i = 0; i < 8; i++){
         MarkerErrorClockchA  = fiber + Chan[i] + "." + Marker_Clock_chA;
         MarkerErrorClockchB  = fiber + Chan[i] + "." + Marker_Clock_chB;
         
         mem[i*6 + 0] = hw.getNode(MarkerErrorClockchA  ).read();
         mem[i*6 + 1] = hw.getNode(MarkerErrorClockchB  ).read();
         hw.dispatch();

         mem[i*6 + 2] = hw2.getNode(MarkerErrorClockchA ).read();
         mem[i*6 + 3] = hw2.getNode(MarkerErrorClockchB ).read();
         hw2.dispatch();
         
         mem[i*6 + 4] = hw3.getNode(MarkerErrorClockchA ).read();
         mem[i*6 + 5] = hw3.getNode(MarkerErrorClockchB ).read();
         hw3.dispatch();
    }
    
    int MaxClock = 0;
    for(int i = 0; i < 47; i++){
        MaxClock = std::max(mem[i].value(), mem[i+1].value());
    }

    cout << "Marker Idle BX: " << std::dec << MaxClock << endl;

    return MaxClock;
}

int SEU_Read(){
    using namespace uhal;
    using namespace std;
    disableLogging();
    
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    ValWord<uint32_t> mem[48];
    
    string Chan[ 16 ] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
    string SEU_Enable      ("SEU.Enable_"     ); 
    string SEU;
    int SEUCount = 0;
    
    for (int i = 0; i < 16; i++){
         SEU  = SEU_Enable + Chan[i];
         
         mem[i*3 + 0] = hw.getNode( SEU ).read();
         hw.dispatch();
         
         mem[i*3 + 1] = hw.getNode( SEU ).read();
         hw.dispatch();
         
         mem[i*3 + 2] = hw.getNode( SEU ).read();
         hw.dispatch();
    }
    
    for(int i = 0; i < 48; i++){
        if(mem[i].value() != 0) SEUCount++;
    }
    return SEUCount;
}

void GLIBVersion(){
    using namespace uhal;
    using namespace std;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem,mem2,mem3,mem4;
    char rice1,rice2,rice3,rice4;
    
    mem4 = hw.getNode("user_rice_ascii_04.char4").read();
    mem  = hw.getNode("user_rice_fw_ver_year.ver_major").read();
    mem2 = hw.getNode("user_rice_fw_ver_year.ver_minor").read();
    mem3 = hw.getNode("user_rice_fw_ver_year.ver_build").read();
    hw.dispatch();
    
    if(mem4.value() == 0x31) cout << "RICE FED TESTER TBM08" << endl;
    else if(mem4.value() == 0x32) cout << "RICE FED TESTER TBM09/10" << endl;
    cout << "FW Version: " << std::dec << mem.value() << "." << mem2.value() << "." << mem3.value() << endl;
}

int GLIBVersion( int glibId ){
    using namespace uhal;
    using namespace std;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw = manager.getDevice ( "GLIB.crate.slot_9" );
    if(glibId == 1) hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    if(glibId == 2) hw=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    if(glibId == 3) hw=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem4;
    
    mem4 = hw.getNode("user_rice_ascii_04.char4").read();
    hw.dispatch();
    
    return mem4.value();
}

uint32_t PKAMRead(){
    using namespace uhal;
    using namespace std;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    ValWord<uint32_t> PKAM_1, PKAM_2;
    
    PKAM_1 = hw.getNode("Fiber_0.PKAM_Reset.chA").read();
    PKAM_2 = hw.getNode("Fiber_0.PKAM_Constant.chA").read();
    hw.dispatch();
    
    return 256*PKAM_1 + PKAM_2;
}

void PKAM_Enable( int PKAM_init, int PKAM_Const ){

    using namespace uhal;
    using namespace std;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    
    uint32_t PKAM_Constant = PKAM_Const;
    uint32_t PKAM_Enable = 0x3;
    
    std::string index[8] = {"0","1","2","3","4","5","6","7"};
    std::string fiber = "Fiber_";
    std::string Value_chA = "PKAM_Reset.chA";
    std::string Value_chB = "PKAM_Reset.chB";
    std::string constant_chA = "PKAM_Constant.chA";
    std::string constant_chB = "PKAM_Constant.chB";
    std::string Enable = "PKAM_Enable";
    std::string PKAM_Value_chA;
    std::string PKAM_Value_chB;
    std::string PKAM_Constant_chA;
    std::string PKAM_Constant_chB;
    std::string Enable_string;
    
    for (int i = 0; i < 8; i++) {
    
        PKAM_Value_chA = fiber + index[i] + "." + Value_chA;
        PKAM_Value_chB = fiber + index[i] + "." + Value_chB;
        PKAM_Constant_chA = fiber + index[i] + "." + constant_chA;
        PKAM_Constant_chB = fiber + index[i] + "." + constant_chB;
        Enable_string = fiber + index[i] + "." + Enable;
            
        hw.getNode(PKAM_Value_chA).write( PKAM_init );
        hw.getNode(PKAM_Constant_chA).write(PKAM_Constant);
        hw.getNode(PKAM_Value_chB).write( PKAM_init );
        hw.getNode(PKAM_Constant_chB).write(PKAM_Constant);
        hw.dispatch();
        
        hw.getNode(Enable_string).write( PKAM_Enable );
        hw.dispatch();
        
        hw2.getNode(PKAM_Value_chA).write( PKAM_init );
        hw2.getNode(PKAM_Constant_chA).write(PKAM_Constant);
        hw2.getNode(PKAM_Value_chB).write( PKAM_init );
        hw2.getNode(PKAM_Constant_chB).write(PKAM_Constant);
        hw2.dispatch();
        
        hw2.getNode(Enable_string).write( PKAM_Enable );
        hw2.dispatch();
        
        hw3.getNode(PKAM_Value_chA).write( PKAM_init );
        hw3.getNode(PKAM_Constant_chA).write(PKAM_Constant);
        hw3.getNode(PKAM_Value_chB).write( PKAM_init );
        hw3.getNode(PKAM_Constant_chB).write(PKAM_Constant);
        hw3.dispatch();
        
        hw3.getNode(Enable_string).write( PKAM_Enable );
        hw3.dispatch();
    } 
}

void PKAM_Enable( int PKAM_init, int PKAM_Const, int Which_Chan ){

    using namespace uhal;
    using namespace std;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    
    uint32_t PKAM_Constant = PKAM_Const;
    uint32_t PKAM_Enable = 0x3;
    
    std::string index[8] = {"0","1","2","3","4","5","6","7"};
    std::string fiber = "Fiber_";
    std::string Value_chA = "PKAM_Reset.chA";
    std::string Value_chB = "PKAM_Reset.chB";
    std::string constant_chA = "PKAM_Constant.chA";
    std::string constant_chB = "PKAM_Constant.chB";
    std::string Enable = "PKAM_Enable";
    std::string PKAM_Value_chA;
    std::string PKAM_Value_chB;
    std::string PKAM_Constant_chA;
    std::string PKAM_Constant_chB;
    std::string Enable_string;
    int Number_Channels = 0;
    
    if(Which_Chan == 0) Number_Channels = 0x55555555;
    else Number_Channels = Which_Chan;
    
    for (int i = 0; i < 8; i++) {
    
        PKAM_Value_chA = fiber + index[i] + "." + Value_chA;
        PKAM_Value_chB = fiber + index[i] + "." + Value_chB;
        PKAM_Constant_chA = fiber + index[i] + "." + constant_chA;
        PKAM_Constant_chB = fiber + index[i] + "." + constant_chB;
        Enable_string = fiber + index[i] + "." + Enable;
        
        if((Number_Channels & 0x1) == 1){
            hw.getNode(PKAM_Value_chA).write( PKAM_init );
            hw.getNode(PKAM_Constant_chA).write(PKAM_Constant);
            hw.dispatch();
            hw2.getNode(PKAM_Value_chA).write( PKAM_init );
            hw2.getNode(PKAM_Constant_chA).write(PKAM_Constant);
            hw2.dispatch();
            hw3.getNode(PKAM_Value_chA).write( PKAM_init );
            hw3.getNode(PKAM_Constant_chA).write(PKAM_Constant);
            hw3.dispatch();
        }
        Number_Channels = Number_Channels >> 2;
        if((Number_Channels & 0x1) == 1){
            hw.getNode(PKAM_Value_chB).write( PKAM_init );
            hw.getNode(PKAM_Constant_chB).write(PKAM_Constant);
            hw.dispatch();
            hw2.getNode(PKAM_Value_chB).write( PKAM_init );
            hw2.getNode(PKAM_Constant_chB).write(PKAM_Constant);
            hw2.dispatch();
            hw3.getNode(PKAM_Value_chB).write( PKAM_init );
            hw3.getNode(PKAM_Constant_chB).write(PKAM_Constant);
            hw3.dispatch();
        }
        Number_Channels = Number_Channels >> 2;
        
        hw.getNode(Enable_string).write( PKAM_Enable );
        hw.dispatch();
        hw2.getNode(Enable_string).write( PKAM_Enable );
        hw2.dispatch();
        hw3.getNode(Enable_string).write( PKAM_Enable );
        hw3.dispatch();
    } 
}

void PKAM_Disable(){

    using namespace uhal;
    using namespace std;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    
    uint32_t PKAM_Enable = 0x0;
    
    std::string index[8] = {"0","1","2","3","4","5","6","7"};
    std::string fiber = "Fiber_";
    std::string Enable = "PKAM_Enable";
    std::string Enable_string;
    
    for (int i = 0; i < 8; i++) {
    
        Enable_string = fiber + index[i] + "." + Enable;
        
        hw.getNode(Enable_string).write( PKAM_Enable );
        hw.dispatch();
        hw2.getNode(Enable_string).write( PKAM_Enable );
        hw2.dispatch();
        hw3.getNode(Enable_string).write( PKAM_Enable );
        hw3.dispatch();
    } 
}

void Delay_Time( int Delay_init ){

    using namespace uhal;
    using namespace std;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    ValWord<uint32_t> mem;
    
    hw.getNode("Delay_Reset").write( 1 );
    hw.dispatch();
    
    hw2.getNode("Delay_Reset").write( 1 );
    hw2.dispatch();
    
    hw3.getNode("Delay_Reset").write( 1 );
    hw3.dispatch();
    
    
    hw.getNode("Delay_Signal").write( Delay_init );
    mem = hw.getNode ( "Delay_Signal" ).read();
    hw.dispatch();
    std::cout << "Delay Signal = " << std::dec << mem.value() << std::endl;
    
    hw2.getNode("Delay_Signal").write( Delay_init );
    hw2.dispatch();
    
    hw3.getNode("Delay_Signal").write( Delay_init );
    hw3.dispatch();
    
    
    hw.getNode("Delay_Reset").write( 0 );
    hw.dispatch();
    
    hw2.getNode("Delay_Reset").write( 0 );
    hw2.dispatch();
    
    hw3.getNode("Delay_Reset").write( 0 );
    hw3.dispatch();

}

void Marker_Rate(){
    using namespace uhal;
    using namespace std;
    disableLogging();
    
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    ValWord<uint32_t> mem, mem2, mem3, mem4, mem5, mem6;
    
    string Chan[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    string fiber ("Fiber_");
    string Marker_Clock_chA     ("Marker_Error_chA_Clock"    );
    string Marker_Clock_chB     ("Marker_Error_chB_Clock"    );
    string MarkerErrorClock; 

	for( int i = 0; i < 8; i++){
		MarkerErrorClock = fiber + Chan[i] + "." + Marker_Clock_chA;
		mem = hw.getNode(MarkerErrorClock).read();
		mem2 = hw2.getNode(MarkerErrorClock).read();
		mem3 = hw3.getNode(MarkerErrorClock).read();
		hw.dispatch();
		hw2.dispatch();
		hw3.dispatch();
		cout << "GLIB1 Marker Rate fiber " << std::dec << i << ": " << mem.value() << endl;
		cout << "GLIB2 Marker Rate fiber " << i << ": " << mem2.value() << endl;
		cout << "GLIB3 Marker Rate fiber " << i << ": " << mem3.value() << endl;
	}
	mem4 = hw.getNode("Marker_dump_1").read();
	mem5 = hw.getNode("Marker_dump_2").read();
	mem6 = hw.getNode("Marker_dump_3").read();
	hw.dispatch();
	cout << "dump Marker Regs: " << std::hex << mem4.value() << " " << mem5.value() << " " << mem6.value() << endl;
}

//Used to change a header of trailer to any ouput desired by the user (12 bits)
//Can send it at any 8 bit frequency and to any channel '0' is all channels
void Marker_Error( int Which_Chan, int Marker_Type, int Marker_Value, int Marker_Rate){
    using namespace uhal;
    using namespace std;
    disableLogging();

    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    ValWord<uint32_t> mem, mem2, mem3;
    
    int Number_Channels = 0;
    string Chan[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    string fiber ("Fiber_");
    string Marker_Value_chA     ("Marker_Error.chA_Value"    );
    string Marker_Value_chB     ("Marker_Error.chB_Value"    );
    string Marker_Type_chA      ("Marker_Error.chA_Type"     );
    string Marker_Type_chB      ("Marker_Error.chB_Type"     );
    string Marker_Clock_chA     ("Marker_Error_chA_Clock"    );
    string Marker_Clock_chB     ("Marker_Error_chB_Clock"    );
    string Marker_ROC_Value_chA ("Marker_Error_ROC_Count_chA");
    string Marker_ROC_Value_chB ("Marker_Error_ROC_Count_chB");
    string MarkerErrorValue;
    string MarkerErrorType; 
    string MarkerErrorClock; 
    string MarkerErrorRoc; 
    
    if(Which_Chan == 0)
        Number_Channels = 0x55555555;
    else
        Number_Channels = Which_Chan;
    for (int i = 0; i < 8; i++){
        int Which_ROC = rand() % 8;
        if((Number_Channels & 0x1) == 1){
            MarkerErrorValue = fiber + Chan[i] + "." + Marker_Value_chA;
            MarkerErrorType  = fiber + Chan[i] + "." + Marker_Type_chA;
            MarkerErrorClock = fiber + Chan[i] + "." + Marker_Clock_chA;
            MarkerErrorRoc   = fiber + Chan[i] + "." + Marker_ROC_Value_chA;
    
            hw.getNode(MarkerErrorClock).write( Marker_Rate  );
            hw.getNode(MarkerErrorValue).write( Marker_Value );
            hw.getNode(MarkerErrorType ).write( Marker_Type  );
            hw.getNode(MarkerErrorRoc  ).write( Which_ROC    );
            hw.dispatch();
    
            hw2.getNode(MarkerErrorClock).write( Marker_Rate  );
            hw2.getNode(MarkerErrorValue).write( Marker_Value );
            hw2.getNode(MarkerErrorType ).write( Marker_Type  );
            hw2.getNode(MarkerErrorRoc  ).write( Which_ROC    );
            hw2.dispatch();
    
            hw3.getNode(MarkerErrorClock).write( Marker_Rate  );
            hw3.getNode(MarkerErrorValue).write( Marker_Value );
            hw3.getNode(MarkerErrorType ).write( Marker_Type  );
            hw3.getNode(MarkerErrorRoc  ).write( Which_ROC    );
            hw3.dispatch();
        }
        Number_Channels = Number_Channels >> 2;
        
        if((Number_Channels & 0x1) == 1){
            MarkerErrorValue = fiber + Chan[i] + "." + Marker_Value_chB;
            MarkerErrorType  = fiber + Chan[i] + "." + Marker_Type_chB;
            MarkerErrorClock = fiber + Chan[i] + "." + Marker_Clock_chB;
            MarkerErrorRoc   = fiber + Chan[i] + "." + Marker_ROC_Value_chB;
    
            hw.getNode(MarkerErrorClock).write( Marker_Rate  );
            hw.getNode(MarkerErrorValue).write( Marker_Value );
            hw.getNode(MarkerErrorType ).write( Marker_Type  );
            hw.getNode(MarkerErrorRoc  ).write( Which_ROC    );
            hw.dispatch();
    
            hw2.getNode(MarkerErrorClock).write( Marker_Rate  );
            hw2.getNode(MarkerErrorValue).write( Marker_Value );
            hw2.getNode(MarkerErrorType ).write( Marker_Type  );
            hw2.getNode(MarkerErrorRoc  ).write( Which_ROC    );
            hw2.dispatch();
    
            hw3.getNode(MarkerErrorClock).write( Marker_Rate  );
            hw3.getNode(MarkerErrorValue).write( Marker_Value );
            hw3.getNode(MarkerErrorType ).write( Marker_Type  );
            hw3.getNode(MarkerErrorRoc  ).write( Which_ROC    );
            hw3.dispatch();
         }
         Number_Channels = Number_Channels >> 2;
    }
}

//Used to change the idle pattern after x BX's from the event trailer to any ouput desired by the user (12 bits)
//Can send it at any 8 bit frequency and to any channel '0' is all channels
void Marker_Error_Idle( int Which_Chan, int Marker_Type, int Marker_Value, int Marker_Rate, int Marker_Rate_Idle){
    using namespace uhal;
    using namespace std;
    disableLogging();

    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    ValWord<uint32_t> mem, mem2, mem3;
    
    int Number_Channels = 0;
    string Chan[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    string fiber ("Fiber_");
    string Marker_Value_chA      ("Marker_Error_Custom.chA_Value"         );
    string Marker_Value_chB      ("Marker_Error_Custom.chB_Value"         );
    string Marker_Type_chA       ("Marker_Error_Custom.chA_Type"          );
    string Marker_Type_chB       ("Marker_Error_Custom.chB_Type"          );
    string Marker_Clock_chA      ("Marker_Error_chA_Clock_Custom"         );
    string Marker_Clock_chB      ("Marker_Error_chB_Clock_Custom"         );
    string Marker_Clock_Idle_chA ("Marker_Error_chA_Clock_Custom_Idle"    );
    string Marker_Clock_Idle_chB ("Marker_Error_chB_Clock_Custom_Idle"    );
    string MarkerErrorValue;
    string MarkerErrorType; 
    string MarkerErrorClock; 
    string MarkerErrorClockIdle; 
    
    if(Which_Chan == 0)
        Number_Channels = 0x55555555;
    else
        Number_Channels = Which_Chan;
    for (int i = 0; i < 8; i++){
        if((Number_Channels & 0x1) == 1){
            MarkerErrorValue     = fiber + Chan[i] + "." + Marker_Value_chA;
            MarkerErrorType      = fiber + Chan[i] + "." + Marker_Type_chA;
            MarkerErrorClock     = fiber + Chan[i] + "." + Marker_Clock_chA;
            MarkerErrorClockIdle = fiber + Chan[i] + "." + Marker_Clock_Idle_chA;
    
            hw.getNode(MarkerErrorClock    ).write( Marker_Rate       );
            hw.getNode(MarkerErrorClockIdle).write( Marker_Rate_Idle  );
            hw.getNode(MarkerErrorValue    ).write( Marker_Value      );
            hw.getNode(MarkerErrorType     ).write( Marker_Type       );
            hw.dispatch();
    
            hw2.getNode(MarkerErrorClock    ).write( Marker_Rate       );
            hw2.getNode(MarkerErrorClockIdle).write( Marker_Rate_Idle  );
            hw2.getNode(MarkerErrorValue    ).write( Marker_Value      );
            hw2.getNode(MarkerErrorType     ).write( Marker_Type       );
            hw2.dispatch();
    
            hw3.getNode(MarkerErrorClock    ).write( Marker_Rate       );
            hw3.getNode(MarkerErrorClockIdle).write( Marker_Rate_Idle  );
            hw3.getNode(MarkerErrorValue    ).write( Marker_Value      );
            hw3.getNode(MarkerErrorType     ).write( Marker_Type       );
            hw3.dispatch();
    
        }
        Number_Channels = Number_Channels >> 2;
        
        if((Number_Channels & 0x1) == 1){
            MarkerErrorValue     = fiber + Chan[i] + "." + Marker_Value_chB;
            MarkerErrorType      = fiber + Chan[i] + "." + Marker_Type_chB;
            MarkerErrorClock     = fiber + Chan[i] + "." + Marker_Clock_chB;
            MarkerErrorClockIdle = fiber + Chan[i] + "." + Marker_Clock_Idle_chB;
    
            hw.getNode(MarkerErrorClock    ).write( Marker_Rate       );
            hw.getNode(MarkerErrorClockIdle).write( Marker_Rate_Idle  );
            hw.getNode(MarkerErrorValue    ).write( Marker_Value      );
            hw.getNode(MarkerErrorType     ).write( Marker_Type       );
            hw.dispatch();
    
            hw2.getNode(MarkerErrorClock    ).write( Marker_Rate       );
            hw2.getNode(MarkerErrorClockIdle).write( Marker_Rate_Idle  );
            hw2.getNode(MarkerErrorValue    ).write( Marker_Value      );
            hw2.getNode(MarkerErrorType     ).write( Marker_Type       );
            hw2.dispatch();
    
            hw3.getNode(MarkerErrorClock    ).write( Marker_Rate       );
            hw3.getNode(MarkerErrorClockIdle).write( Marker_Rate_Idle  );
            hw3.getNode(MarkerErrorValue    ).write( Marker_Value      );
            hw3.getNode(MarkerErrorType     ).write( Marker_Type       );
            hw3.dispatch();
    
        }
        Number_Channels = Number_Channels >> 2;
    }
}

void writeTTSState( int State ){
    using namespace uhal;
    disableLogging();
    
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    
    hw.getNode("TTSState").write(State);
    hw.dispatch();
    hw2.getNode("TTSState").write(State);
    hw2.dispatch();
    hw3.getNode("TTSState").write(State);
    hw3.dispatch();
} 

void writeCSRMode( int State ){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    ValWord<uint32_t> mem;

    hw.getNode("CSR_Mode").write(State);
    mem = hw.getNode("CSR_Mode").read();
    hw.dispatch();
    cout << "CSR Mode: " << std::dec << mem.value() << endl;
    hw2.getNode("CSR_Mode").write(State);
    hw2.dispatch();
    hw3.getNode("CSR_Mode").write(State);
    hw3.dispatch();
} 

void Set_ROCS(int roc_input){
    using namespace uhal;
    disableLogging();
    
    uint32_t Number_ROCS = 0x88;
    uint32_t inter_num;
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    
    uint32_t ROC_Clk = 0x9898;  //Puts a value of 160 clk cycles in channel A & B
    
    if (roc_input == 0) { }
    else if (roc_input <= 8) {
        inter_num = (roc_input << 4);
        Number_ROCS = inter_num | roc_input;
    }
    
    std::string index[8] = {"0","1","2","3","4","5","6","7"};
    std::string fiber ("Fiber_");
    
    for (int i = 0; i<8; i++) {
        std::string roc_num = "ROC_Number";
        roc_num = fiber + index[i] + "." + roc_num;
        
        std::string roc_clock = "ROC_CLK";
        roc_clock = fiber + index[i] + "." + roc_clock;
        
        hw.getNode(roc_num).write( Number_ROCS );
        hw.getNode(roc_clock).write( ROC_Clk );
        hw.dispatch();
        hw2.getNode(roc_num).write( Number_ROCS );
        hw2.getNode(roc_clock).write( ROC_Clk );
        hw2.dispatch();
        hw3.getNode(roc_num).write( Number_ROCS );
        hw3.getNode(roc_clock).write( ROC_Clk );
        hw3.dispatch();
    }
    
    writeTTSState(8);
}

void Set_Num_Hits(int hit_input){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem,mem2,mem3;
    uint32_t Hits1 = 0x00000000;
    uint32_t Hits2 = 0x00000000;
    uint32_t inter_hits[4];
    if (hit_input == 0) { }
    else {
        for (int i = 0; i < 4; i++) {
            inter_hits[i] = hit_input << (8*(3-i));
        }
        Hits1 = inter_hits[0] | inter_hits[1] | inter_hits[2] | inter_hits[3];
        Hits2 = Hits1;
    }
    std::string index[8] = {"0","1","2","3","4","5","6","7"};
    std::string fiber ("Fiber_");
    for (int i = 0; i < 8; i++) {
        std::string cha_hits1 = "CHA_Hits_1";
        std::string cha_hits2 = "CHA_Hits_2";
        cha_hits1 = fiber + index[i] + "." + cha_hits1;
        cha_hits2 = fiber + index[i] + "." + cha_hits2;
        std::string chb_hits1 = "CHB_Hits_1";
        std::string chb_hits2 = "CHB_Hits_2";
        chb_hits1 = fiber + index[i] + "." + chb_hits1;
        chb_hits2 = fiber + index[i] + "." + chb_hits2;
        hw.getNode(cha_hits1).write( Hits1 );
        hw.getNode(chb_hits1).write( Hits1 );
        mem = hw.getNode ( cha_hits1 ).read();
        hw.dispatch();
        std::cout << cha_hits1 << ": " << mem.value() << std::endl;
        hw2.getNode(cha_hits1).write( Hits1 );
        hw2.getNode(chb_hits1).write( Hits1 );
        hw2.dispatch();
        hw3.getNode(cha_hits1).write( Hits1 );
        hw3.getNode(chb_hits1).write( Hits1 );
        hw3.dispatch();
        hw.getNode(cha_hits2).write( Hits2 );
        hw.getNode(chb_hits2).write( Hits2 );
        mem = hw.getNode ( cha_hits2 ).read();
        hw.dispatch();
        std::cout << cha_hits2 << ": " << mem.value() << std::endl;
        hw2.getNode(cha_hits2).write( Hits2 );
        hw2.getNode(chb_hits2).write( Hits2 );
        hw2.dispatch();
        hw3.getNode(cha_hits2).write( Hits2 );
        hw3.getNode(chb_hits2).write( Hits2 );
        hw3.dispatch();
    }
}

void Set_Num_Hits(int hit_input_GLIB1_chA, int hit_input_GLIB1_chB, int hit_input_GLIB2_chA, int hit_input_GLIB2_chB, int hit_input_GLIB3_chA, int hit_input_GLIB3_chB, int Which_Channels){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem,mem_1,mem2,mem2_1,mem3,mem3_1;
    
    uint32_t Hits1_GLIB1_chA = 0x00000000;
    uint32_t Hits2_GLIB1_chA = 0x00000000;
    uint32_t Hits1_GLIB1_chB = 0x00000000;
    uint32_t Hits2_GLIB1_chB = 0x00000000;
    uint32_t Hits1_GLIB2_chA = 0x00000000;
    uint32_t Hits2_GLIB2_chA = 0x00000000;
    uint32_t Hits1_GLIB2_chB = 0x00000000;
    uint32_t Hits2_GLIB2_chB = 0x00000000;
    uint32_t Hits1_GLIB3_chA = 0x00000000;
    uint32_t Hits2_GLIB3_chA = 0x00000000;
    uint32_t Hits1_GLIB3_chB = 0x00000000;
    uint32_t Hits2_GLIB3_chB = 0x00000000;
    uint32_t inter_hits_GLIB1_chA[4];
    uint32_t inter_hits_GLIB1_chB[4];
    uint32_t inter_hits_GLIB2_chA[4];
    uint32_t inter_hits_GLIB2_chB[4];
    uint32_t inter_hits_GLIB3_chA[4];
    uint32_t inter_hits_GLIB3_chB[4];
    int Number_Channels = 0;
    
    for (int i = 0; i < 4; i++) {
        inter_hits_GLIB1_chA[i] = hit_input_GLIB1_chA << (8*(3-i));
        inter_hits_GLIB1_chB[i] = hit_input_GLIB1_chB << (8*(3-i));
        inter_hits_GLIB2_chA[i] = hit_input_GLIB2_chA << (8*(3-i));
        inter_hits_GLIB2_chB[i] = hit_input_GLIB2_chB << (8*(3-i));
        inter_hits_GLIB3_chA[i] = hit_input_GLIB3_chA << (8*(3-i));
        inter_hits_GLIB3_chB[i] = hit_input_GLIB3_chB << (8*(3-i));
    }
    Hits1_GLIB1_chA = hit_input_GLIB1_chA;//inter_hits_GLIB1_chA[0] | inter_hits_GLIB1_chA[1] | inter_hits_GLIB1_chA[2] | inter_hits_GLIB1_chA[3];
    Hits2_GLIB1_chA = hit_input_GLIB1_chA;
    Hits1_GLIB2_chA = hit_input_GLIB2_chA;//inter_hits_GLIB2_chA[0] | inter_hits_GLIB2_chA[1] | inter_hits_GLIB2_chA[2] | inter_hits_GLIB2_chA[3];
    Hits2_GLIB2_chA = hit_input_GLIB2_chA;
    Hits1_GLIB3_chA = hit_input_GLIB3_chA;//inter_hits_GLIB3_chA[0] | inter_hits_GLIB3_chA[1] | inter_hits_GLIB3_chA[2] | inter_hits_GLIB3_chA[3];
    Hits2_GLIB3_chA = hit_input_GLIB3_chA;
    
    Hits1_GLIB1_chB = hit_input_GLIB1_chB;//inter_hits_GLIB1_chB[0] | inter_hits_GLIB1_chB[1] | inter_hits_GLIB1_chB[2] | inter_hits_GLIB1_chB[3];
    Hits2_GLIB1_chB = hit_input_GLIB1_chB;
    Hits1_GLIB2_chB = hit_input_GLIB2_chB;//inter_hits_GLIB2_chB[0] | inter_hits_GLIB2_chB[1] | inter_hits_GLIB2_chB[2] | inter_hits_GLIB2_chB[3];
    Hits2_GLIB2_chB = hit_input_GLIB2_chB;
    Hits1_GLIB3_chB = hit_input_GLIB3_chB;//inter_hits_GLIB3_chB[0] | inter_hits_GLIB3_chB[1] | inter_hits_GLIB3_chB[2] | inter_hits_GLIB3_chB[3];
    Hits2_GLIB3_chB = hit_input_GLIB3_chB;
    
    std::string index[8] = {"0","1","2","3","4","5","6","7"};
    std::string fiber ("Fiber_");
    
    if (Which_Channels == 0) Number_Channels = 0x55555555;
    else Number_Channels = Which_Channels;
    
    for (int i = 0; i < 8; i++) {
    
        std::string cha_hits1 = "CHA_Hits_1";
        std::string cha_hits2 = "CHA_Hits_2";
        cha_hits1 = fiber + index[i] + "." + cha_hits1;
        cha_hits2 = fiber + index[i] + "." + cha_hits2;
        
        std::string chb_hits1 = "CHB_Hits_1";
        std::string chb_hits2 = "CHB_Hits_2";
        chb_hits1 = fiber + index[i] + "." + chb_hits1;
        chb_hits2 = fiber + index[i] + "." + chb_hits2;
        
        if((Number_Channels & 0x1) == 1){
            hw.getNode(cha_hits1).write( Hits1_GLIB1_chA );
            hw.getNode(cha_hits2).write( Hits2_GLIB1_chA );
            hw.dispatch();
            hw2.getNode(cha_hits1).write( Hits1_GLIB2_chA );
            hw2.getNode(cha_hits2).write( Hits2_GLIB2_chA );
            hw2.dispatch();
            hw3.getNode(cha_hits1).write( Hits1_GLIB3_chA );
            hw3.getNode(cha_hits2).write( Hits1_GLIB3_chA );
            hw3.dispatch();
        }
        Number_Channels = Number_Channels >> 2;
        
        if((Number_Channels & 0x1) == 1){
            hw.getNode(chb_hits1).write( Hits1_GLIB1_chB );
            hw.getNode(chb_hits2).write( Hits2_GLIB1_chB );
            hw.dispatch();
            hw2.getNode(chb_hits1).write( Hits1_GLIB2_chB );
            hw2.getNode(chb_hits2).write( Hits2_GLIB2_chB );
            hw2.dispatch();
            hw3.getNode(chb_hits1).write( Hits1_GLIB3_chB );
            hw3.getNode(chb_hits2).write( Hits1_GLIB3_chB );
            hw3.dispatch();
        }
        Number_Channels = Number_Channels >> 2;
    }

}

void Set_Num_Hits(int hit1_input_GLIB1_chA, int hit2_input_GLIB1_chA, int hit1_input_GLIB1_chB, int hit2_input_GLIB1_chB, int hit1_input_GLIB2_chA, int hit2_input_GLIB2_chA, int hit1_input_GLIB2_chB, int hit2_input_GLIB2_chB, int hit1_input_GLIB3_chA, int hit2_input_GLIB3_chA, int hit1_input_GLIB3_chB, int hit2_input_GLIB3_chB, int Which_Channels){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem,mem_1,mem2,mem2_1,mem3,mem3_1;
    
    uint32_t Hits1_GLIB1_chA = 0x00000000;
    uint32_t Hits2_GLIB1_chA = 0x00000000;
    uint32_t Hits1_GLIB1_chB = 0x00000000;
    uint32_t Hits2_GLIB1_chB = 0x00000000;
    uint32_t Hits1_GLIB2_chA = 0x00000000;
    uint32_t Hits2_GLIB2_chA = 0x00000000;
    uint32_t Hits1_GLIB2_chB = 0x00000000;
    uint32_t Hits2_GLIB2_chB = 0x00000000;
    uint32_t Hits1_GLIB3_chA = 0x00000000;
    uint32_t Hits2_GLIB3_chA = 0x00000000;
    uint32_t Hits1_GLIB3_chB = 0x00000000;
    uint32_t Hits2_GLIB3_chB = 0x00000000;
    int Number_Channels = 0;
    
    Hits1_GLIB1_chA = hit1_input_GLIB1_chA;
    Hits2_GLIB1_chA = hit2_input_GLIB1_chA;
    Hits1_GLIB2_chA = hit1_input_GLIB2_chA;
    Hits2_GLIB2_chA = hit2_input_GLIB2_chA;
    Hits1_GLIB3_chA = hit1_input_GLIB3_chA;
    Hits2_GLIB3_chA = hit2_input_GLIB3_chA;
    
    Hits1_GLIB1_chB = hit1_input_GLIB1_chB;
    Hits2_GLIB1_chB = hit2_input_GLIB1_chB;
    Hits1_GLIB2_chB = hit1_input_GLIB2_chB;
    Hits2_GLIB2_chB = hit2_input_GLIB2_chB;
    Hits1_GLIB3_chB = hit1_input_GLIB3_chB;
    Hits2_GLIB3_chB = hit2_input_GLIB3_chB;
    
    std::string index[8] = {"0","1","2","3","4","5","6","7"};
    std::string fiber ("Fiber_");
    
    if (Which_Channels == 0) Number_Channels = 0x55555555;
    else Number_Channels = Which_Channels;
    
    for (int i = 0; i < 8; i++) {
    
        std::string cha_hits1 = "CHA_Hits_1";
        std::string cha_hits2 = "CHA_Hits_2";
        cha_hits1 = fiber + index[i] + "." + cha_hits1;
        cha_hits2 = fiber + index[i] + "." + cha_hits2;
        
        std::string chb_hits1 = "CHB_Hits_1";
        std::string chb_hits2 = "CHB_Hits_2";
        chb_hits1 = fiber + index[i] + "." + chb_hits1;
        chb_hits2 = fiber + index[i] + "." + chb_hits2;
        
        if((Number_Channels & 0x1) == 1){
            hw.getNode(cha_hits1).write( Hits1_GLIB1_chA );
            hw.getNode(cha_hits2).write( Hits2_GLIB1_chA );
            hw.dispatch();
            hw2.getNode(cha_hits1).write( Hits1_GLIB2_chA );
            hw2.getNode(cha_hits2).write( Hits2_GLIB2_chA );
            hw2.dispatch();
            hw3.getNode(cha_hits1).write( Hits1_GLIB3_chA );
            hw3.getNode(cha_hits2).write( Hits1_GLIB3_chA );
            hw3.dispatch();
        }
        Number_Channels = Number_Channels >> 2;
        
        if((Number_Channels & 0x1) == 1){
            hw.getNode(chb_hits1).write( Hits1_GLIB1_chB );
            hw.getNode(chb_hits2).write( Hits2_GLIB1_chB );
            hw.dispatch();
            hw2.getNode(chb_hits1).write( Hits1_GLIB2_chB );
            hw2.getNode(chb_hits2).write( Hits2_GLIB2_chB );
            hw2.dispatch();
            hw3.getNode(chb_hits1).write( Hits1_GLIB3_chB );
            hw3.getNode(chb_hits2).write( Hits1_GLIB3_chB );
            hw3.dispatch();
        }
        Number_Channels = Number_Channels >> 2;
    }

}

int Write_ROCs(int i, int Hit_infoA, int Hit_infoB){
    using namespace std;
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    
    std::string Chan[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    std::string fiber ("Fiber_");
    
    for(int j = 0; j < 8; j++){
        string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
        string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"};
        
        ROCSA[i] = fiber + Chan[j] + "." + ROCSA[i];
        ROCSB[i] = fiber + Chan[j] + "." + ROCSB[i];
        hw.getNode( ROCSA[i] ).write(Hit_infoA);
        ValWord<uint32_t> mem = hw.getNode(ROCSA[i]).read();
        hw.dispatch();
        std::cout << ROCSA[i] << " = " << mem.value() << std::endl;
        hw.getNode( ROCSB[i] ).write(Hit_infoB);
        mem = hw.getNode(ROCSB[i]).read();
        hw.dispatch();
        hw2.getNode( ROCSA[i] ).write(Hit_infoA);
        hw2.getNode( ROCSB[i] ).write(Hit_infoB);
        hw2.dispatch();
        hw3.getNode( ROCSA[i] ).write(Hit_infoA);
        hw3.getNode( ROCSB[i] ).write(Hit_infoB);
        hw3.dispatch();
        ROCSA[j] = ROCSA[j].substr(0,9);
        ROCSB[j] = ROCSB[j].substr(0,9);
    }
}

std::vector<int> readTTSState(){
    using namespace uhal;
    disableLogging();
           
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );  //this is the glib with the debug card
    ValWord<uint32_t> mem,mem2,mem3;
    std::vector<int> cWord;
    mem = hw.getNode("TTSState").read();
    hw.dispatch();
    mem2 = hw2.getNode("TTSState").read();
    hw2.dispatch();
    mem3 = hw3.getNode("TTSState").read();
    hw3.dispatch();
            
    cWord.push_back(mem.value());
    cWord.push_back(mem2.value());
    cWord.push_back(mem3.value());
            
    std::cout << "GLIB TTS State: ";
    switch(mem.value()){
        case 8: 
            std::cout << "RDY"  << std::endl;
            break;
        case 4:
            std::cout << "BSY"  << std::endl;
            break;
        case 2:
            std::cout << "OOS"  << std::endl;
            break;
        case 1:
            std::cout << "OVF"  << std::endl;
            break;
        case 0:
            std::cout << "DIC"  << std::endl;
            break;
        case 12:
            std::cout << "ERR"  << std::endl;
            break;
    }       
    
    return cWord;
}

void Write_Data_Header( int Header_Value ){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    
    int New_Header_data_A = Header_Value;
    int New_Header_data_B = Header_Value;
    string Chan[8] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    string fiber ("Fiber_");       
    
    for (int i = 0; i < 8; i++){
        string Header_A ("Header_Flag_A");
        string Header_B ("Header_Flag_B");
        Header_A = fiber + Chan[i] + "." + Header_A;
        Header_B = fiber + Chan[i] + "." + Header_B;
        
        hw.getNode(Header_A).write( New_Header_data_A );
        hw.dispatch();
        hw.getNode(Header_B).write( New_Header_data_B );
        hw.dispatch();
        
        hw2.getNode(Header_A).write( New_Header_data_A );
        hw2.dispatch();
        hw2.getNode(Header_B).write( New_Header_data_B );
        hw2.dispatch();
        
        
        hw3.getNode(Header_A).write( New_Header_data_A );
        hw3.dispatch();
        hw3.getNode(Header_B).write( New_Header_data_B );
        hw3.dispatch();
    }       
}   
