#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <functional>
#include <sstream>
#include <string>
using namespace std;
typedef std::vector<std::pair<std::string, uint32_t> > vector_type;

struct QuitNow: public std::exception
  {
  QuitNow() { }
  virtual ~QuitNow() throw() { }
  virtual const char* what() throw()
    {
    return "QuitNow: request normal termination of program.\n"
           "(You should not see this message. Please report it if you do.)";
    }
  };

/* string_to_boolean()
 *
 *   Convert a string into its boolean value.
 *   The string MUST BE one of:
 *     "TRUE" "FALSE" "YES" "NO" "0" "1"
 *   or a numeric value. Case is not significant.
 *
 *   Numeric values evaluate to FALSE if they equal zero; TRUE otherwise.
 *
 *   If the conversion is STRICT then numeric values are limited to 
 *   the strings "0" and "1".
 */

typedef enum
  {
  boolean_false,
  boolean_true,
  boolean_invalid
  }
  string_to_boolean_t;

string_to_boolean_t
string_to_boolean( const std::string& s, bool strict = false )
  {
  const char* falses[] = { "false", "no",  "0" };
  const char* trues [] = { "true",  "yes", "1" };

  unsigned num_falses = sizeof( falses ) / sizeof( falses[ 0 ] );
  unsigned num_trues  = sizeof( trues  ) / sizeof( trues [ 0 ] );

  // Special case
  if (s.empty()) return boolean_invalid;

  // Get a lowercase version of 's'
  std::string s2( s );
  std::transform(
    s2.begin(),
    s2.end(),
    s2.begin(),
    std::ptr_fun <int, int> ( std::tolower )
    );

  // Does the string represent a FALSE?
  for (unsigned n = 0; n < num_falses; n++)
    if (std::string( falses[ n ] ).find( s2 ) == 0)
      return boolean_false;

  // Does the string represent a TRUE?
  for (unsigned n = 0; n < num_trues; n++)
    if (std::string( trues[ n ] ).find( s2 ) == 0)
      return boolean_true;

  // Check for non-zero numbers here
  if (!strict)
    {
    std::istringstream ss( s2 );
    double d;
    if (ss >> d)
      return (d == 0.0)
           ? boolean_false
           : boolean_true;
    }

  // The string was not recognized
  return boolean_invalid;
  }

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

int L1_Col( int col, int row){
	return 0;
	//return (col << 1) | (col % 2);
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
    if(row >= 9 ) PXL_left_int++;
    if(row >= 27) PXL_left_int++;
    if(row >= 45) PXL_left_int++;
    if(row >= 63) PXL_left_int++;

	int PXL_left_buff = (4 - PXL_left_int);

	if( col % 2 == 1)
        new_row = (PXL_left_buff << 6) | (PXL_mid_buff << 3) | ((5 - PXL_right) % 6);
    else
        new_row = (PXL_left_buff << 6) | (PXL_mid_buff << 3) | ((4 - PXL_right) % 6);

    return new_row;

}

int L1_row( int row, int col){
	if(col % 2) return 2*row;
	else return 2*row + 1;
}

int DCOL_Encode( int DCOL_Input){
	return (((DCOL_Input & 0x38) >> 3)*6) + (DCOL_Input & 0x7);
}

int PXL_Encode( int PXL_Input ){
	return (((PXL_Input & 0x1C0) >> 6)*36) + (((PXL_Input & 0x38) >> 3)*6) + (PXL_Input & 0x7);
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

void TBMVersion( int GLIB1_version, int GLIB2_version, int GLIB3_version){
	using namespace uhal;
    disableLogging();
	ConnectionManager manager ("file://settings/GLIB_connections.xml");
	HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
	HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
	HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
	ValWord<uint32_t> mem;
	hw.getNode("TBM_version").write(GLIB1_version);
	mem = hw.getNode("TBM_version").read();
	hw.dispatch();
	hw2.getNode("TBM_version").write(GLIB2_version);
	mem = hw2.getNode("TBM_version").read();
	hw2.dispatch();
	hw3.getNode("TBM_version").write(GLIB3_version);
	mem = hw3.getNode("TBM_version").read();
	hw3.dispatch();
}

void Set_ROCS( int GLIB, int roc_input){
	using namespace uhal;
    disableLogging();
	ConnectionManager manager ("file://settings/GLIB_connections.xml");
	HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
	HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
	HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
	ValWord<uint32_t> mem;

	uint32_t ROC_Clk = 0x9898;
	uint32_t Number_ROCS = 0x88;
	uint32_t inter_num;
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
		
		if(GLIB == 1){
			hw.getNode(roc_num).write( Number_ROCS );
			mem = hw.getNode(roc_num).read();
			hw.dispatch();
			hw.getNode(roc_clock).write( ROC_Clk );
			mem = hw.getNode(roc_clock).read();
			hw.dispatch();
		}
		else if(GLIB == 2){
			hw2.getNode(roc_num).write( Number_ROCS );
			mem = hw2.getNode(roc_num).read();
			hw2.dispatch();
			hw2.getNode(roc_clock).write( ROC_Clk );
			mem = hw2.getNode(roc_clock).read();
			hw2.dispatch();
		}
		else if(GLIB == 3){
			hw3.getNode(roc_num).write( Number_ROCS );
			mem = hw3.getNode(roc_num).read();
			hw3.dispatch();
			hw3.getNode(roc_clock).write( ROC_Clk );
			mem = hw3.getNode(roc_clock).read();
			hw3.dispatch();
		}
	}
}

void ADCAsEvn( int Enable ){
	using namespace uhal;
    disableLogging();
	ConnectionManager manager ("file://settings/GLIB_connections.xml");
	HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
	HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
	HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
	ValWord<uint32_t> mem, mem2, mem3;

	hw.getNode("ADCAsEvn").write(Enable);
	hw.dispatch();
	hw2.getNode("ADCAsEvn").write(Enable);
	hw2.dispatch();
	hw3.getNode("ADCAsEvn").write(Enable);
	hw3.dispatch();
}

void SRAM_Disable(){
	using namespace uhal;
    disableLogging();
	ConnectionManager manager ("file://settings/GLIB_connections.xml");
	HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
	HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
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

void TTC_Reset(){
	using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem, mem2, mem3;
    
    hw2.getNode("TTC_Reset").write(1);
    hw2.dispatch();
    hw2.getNode("TTC_Reset").write(0);
    hw2.dispatch();
  
}

void SEU(int enable, int type, int mode, int Which_Chan){
	using namespace uhal;
    disableLogging();
	ConnectionManager manager ("file://settings/GLIB_connections.xml");
	HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
	HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
	HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
	ValWord<uint32_t> ena, ty, mod;
	string Chan[ 16 ] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
	int Number_Channels = 0;

	if(Which_Chan == 0) Number_Channels = 0x55555555;
	else Number_Channels = Which_Chan;
	
	for(int chan = 0; chan < 16; chan++){
	    string Enable("SEU.Enable_"), Type("SEU.Type_"), Mode("SEU.Mode_");
	    if(Number_Channels & 0x1 == 1){
	    	Enable += Chan[chan];
	    	Type += Chan[chan];
	    	Mode += Chan[chan];
	    	hw.getNode(Enable).write(enable);
	    	hw.getNode(Type).write(type);
	    	hw.getNode(Mode).write(mode);
	    	hw.dispatch();
	    	hw2.getNode(Enable).write(enable);
	    	hw2.getNode(Type).write(type);
	    	hw2.getNode(Mode).write(mode);
	    	hw2.dispatch();
	    	hw3.getNode(Enable).write(enable);
	    	hw3.getNode(Type).write(type);
	    	hw3.getNode(Mode).write(mode);
	    	hw3.dispatch();
	    }
	    Number_Channels = Number_Channels >> 2;
	}
}

void GLIBL1ACounter(){
	using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem, mem2, mem3;

	mem = hw.getNode("L1A_count").read();
	hw.dispatch();
	mem2 = hw2.getNode("L1A_count").read();
	hw2.dispatch();
	mem3 = hw3.getNode("L1A_count").read();
	hw3.dispatch();

	cout << "GLIB1 L1A count: " << mem.value() << endl;
	cout << "GLIB2 L1A count: " << mem2.value() << endl;
	cout << "GLIB3 L1A count: " << mem3.value() << endl;
}

std::vector<std::pair<std::string, uint32_t> > GLIBHeaderErrorCountDump( ){
	using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem, mem2, mem3;
	string Chan[ 16 ] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
    std::vector<std::pair<std::string, uint32_t> > Counter_reg;
	string GLIB1("GLIB1 ");
	string GLIB2("GLIB2 ");
	string GLIB3("GLIB3 ");
	for(int chan = 0; chan < 16; chan++){
		string headerID("TBMHeaderError.tbm_ch_");
		headerID += Chan[chan];
		mem = hw.getNode(headerID).read();
		hw.dispatch();
		mem2 = hw2.getNode(headerID).read();
		hw2.dispatch();
		mem3 = hw3.getNode(headerID).read();
        hw3.dispatch();
		Counter_reg.push_back({GLIB1 + headerID , mem  });
        Counter_reg.push_back({GLIB2 + headerID , mem2 });
        Counter_reg.push_back({GLIB3 + headerID , mem3 });
	}

    return Counter_reg;
}

std::vector<std::pair<std::string, uint32_t> > GLIBTrailerErrorCountDump( ){
	using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem, mem2, mem3;
	string Chan[ 16 ] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
    std::vector<std::pair<std::string, uint32_t> > Counter_reg;
	string GLIB1("GLIB1 ");
	string GLIB2("GLIB2 ");
	string GLIB3("GLIB3 ");
	for(int chan = 0; chan < 16; chan++){
		string trailerID("TBMTrailerError.tbm_ch_");
		trailerID += Chan[chan];
		mem = hw.getNode(trailerID).read();
		hw.dispatch();
		mem2 = hw2.getNode(trailerID).read();
		hw2.dispatch();
		mem3 = hw3.getNode(trailerID).read();
        hw3.dispatch();
		Counter_reg.push_back({GLIB1 + trailerID , mem  });
        Counter_reg.push_back({GLIB2 + trailerID , mem2 });
        Counter_reg.push_back({GLIB3 + trailerID , mem3 });
	}

    return Counter_reg;
}

std::vector<std::pair<std::string, uint32_t> > GLIBHeaderCountDump( ){
	using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem, mem2, mem3;
	string Chan[ 16 ] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
    std::vector<std::pair<std::string, uint32_t> > Counter_reg;
	string GLIB1("GLIB1 ");
	string GLIB2("GLIB2 ");
	string GLIB3("GLIB3 ");
	for(int chan = 0; chan < 16; chan++){
		string headerID("HeaderID_tbm_ch_");
		headerID += Chan[chan];
		mem = hw.getNode(headerID).read();
		hw.dispatch();
		mem2 = hw2.getNode(headerID).read();
		hw2.dispatch();
		mem3 = hw3.getNode(headerID).read();
        hw3.dispatch();
		Counter_reg.push_back({GLIB1 + headerID , mem  });
        Counter_reg.push_back({GLIB2 + headerID , mem2 });
        Counter_reg.push_back({GLIB3 + headerID , mem3 });
	}

    return Counter_reg;
}

std::vector<std::pair<std::string, uint32_t> > GLIBTrailerCountDump( ){
	using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
    HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );
    ValWord<uint32_t> mem, mem2, mem3;
	string Chan[ 16 ] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
    std::vector<std::pair<std::string, uint32_t> > Counter_reg;
	string GLIB1("GLIB1 ");
	string GLIB2("GLIB2 ");
	string GLIB3("GLIB3 ");
	for(int chan = 0; chan < 16; chan++){
		string trailerID("TrailerID_tbm_ch_");
		trailerID += Chan[chan];
		mem = hw.getNode(trailerID).read();
		hw.dispatch();
		mem2 = hw2.getNode(trailerID).read();
		hw2.dispatch();
		mem3 = hw3.getNode(trailerID).read();
		hw3.dispatch();
        Counter_reg.push_back({GLIB1 + trailerID , mem  });
        Counter_reg.push_back({GLIB2 + trailerID , mem2 });
        Counter_reg.push_back({GLIB3 + trailerID , mem3 });
	}

    return Counter_reg;
}

void Test_Write(){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
    ValWord<uint32_t> mem, mem2, mem3;

	hw.getNode("Test_write").write(0x1010);
}

void Test_Read(){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
    ValWord<uint32_t> mem, mem2, mem3;

	mem = hw.getNode("Test_write").read();
	hw.dispatch();
	cout << "Test: " << mem.value() << endl;
}

void SRAM_Mode(){
    using namespace uhal;
    disableLogging();
    ConnectionManager manager ("file://settings/GLIB_connections.xml");
    HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );
    HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );
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


