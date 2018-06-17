//HEADERS REQUIRED 
#include <iostream>
#include <string.h>
#include <vector>
#include <stdlib.h>

using namespace std;

class enhanced_armsimulator {
	private: string words,key;   //REGS=REGISTERS, WORDS TAKES THE WHOLE LINE AT ONCE AND KEY CONTAINS THE INSTRUCTION OR KEYWORD
			 int clock,link,str_key,instruction_count,latency[13];
			 string str_value;
			 vector<string> pipeline_register[4];
			 vector<string> label; //FOR STORING VARIOUS LABELS
			 vector<int> index;		// FOR MAINTAING THE INDEX OF LABELS SO THAT WE KNOW WHERE TO JUMP 
			 vector<string> instruction;// FOR STORING INPUT FILE INSTRUCTIONS
			 int regs[16],r1,r2,r3,carry,num,memory[4000];
	public:
		// THIS IS A DEFAULT CONSTRUCTOR USED TO PROVIDE INITIAL VALUES TO THE DATA MEMBERS.
 		enhanced_armsimulator();

		// THIS FUNCTION IS FOR PROVIDING VALUES TO THE DATA MEMBERS FROM MAIN
		void getdata(vector<string> l,vector<int> in,vector<string> s,int x[]);

		//THIS IS THE MOST IMPORTANT FUNCTION OF THE CLASS IT WILL CALL ALL THE FUNCTION IN THE 
		//CLASS ONE BY ONE IN A SEQUENCE.
		int instruction_section();

		//-------------------------------------------------------------------------------------------------//
		//------------------------FOR HANDLING ERRORS------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------//


		//THIS FUNCTION WILL PARSE THE INSTRUCTION AND CHECK ITS VALIDITY
		//THIS FUNCTION WILL CALL SUBCOMPONENTS FOR VARIOUS POSSIBILITIES.
		int error_detection_unit(int i); 

		// ERROR DETECTION UNIT WILL CALL FOLLOWING FUNCTIONS(SUBCOMPONENTS):


		// THIS FUNCTION EXPLOITS THE  BRANCH INSTRUCTION WITH THREE ALPHABETS SUCH AS BLE,BNE ETC.
		int possibility3(int i);
		// THIS FUNCTION EXPLOITS THE  BRANCH INSTRUCTION WITH TWO ALPHABETS SUCH AS BL.
		int possibility4(int i);
		// THIS FUNCTION EXPLOITS THE  BRANCH INSTRUCTION WITH SINGLE ALPHABETS SUCH AS B.
		int possibility5(int i);
		// THIS FUNCTION CHECKS ALL BRANCH INSTRUCTION WHENEVER CALLED UPON  AND RETURNS THE VALUES WHICH
		// IT RECIEVES FROM POSSIBILITY 3,4,5 FUNCTION, ONE AT A TIME.        
		int checkerror2(int i);

		// THIS FUNCTION CHECKS WHETHER THE KEY WORD IS VALID OR NOT
		void checkkeyword(int i);


		// THIS FUNCTION CHECKS ALL POSSIBILITIES OF MOV AND CMP COMMAND
		int possibility1(int i);     // this function is for exploiting all error possibilities which can take place in mov or cmp command

		// THIS FUNCTION CHECKS ALL POSSIBILITIES OF ADD,SUB AND MUL COMMAND.
		int possibility2(int i);     // this function is for exploiting all error possibilities which can take place in add or sub or mul command

		// THIS FUNCTION CHECKS ALL THE POSSIBILITIES OF LOAD AND STORE COMMAND , FOR SIMPLIFICATION WE HAVE 3 POSSIBLE VARIATIONS OF LDR AND STR
		int possibilitymemory(int i);

		// THIS FUNCTION RETURNS AN INTEGER CONSTANTS AFTER CHECKING ERROR  IN ANY #INTEGER VALUES.
		int possibilityhash(char i,int r);

		// THIS FUNCTION CHECKS ERROR AND RETURNS THE VALUES AFTER DEALING WITH VARIOUS POSSIBILITIES.
		int checkerror(int i);

		// END OF ERROR DETECTION SUBCOMPONENTS

		//------------------------------------------------------------------------------------------------------------------------------------------------//
		//----------------------------------END OF ERROR HANDLING UNIT------------------------------------------------------------------------------------//
		//------------------------------------------------------------------------------------------------------------------------------------------------//

		//------------------------------------------------------------------------------------------------------------------------------------------------//
		//----------------------------------VARIOUS STAGES OF ARMSIM--------------------------------------------------------------------------------------//
		//------------------------------------------------------------------------------------------------------------------------------------------------//

		int IF_STAGE(int i);
	 	int ID_STAGE();
	    int EXE_STAGE();
	    // THIS FUNCTION PERFORMS ARTHEMATIC OPERATIONS BETWEEN REGISTERS ONLY
		void performoperation(int i);

		// THIS FUNCTION PERFORMS ARTHEMATIC OPERATIONS BETWEEN REGISTERS AND A CONSTANT
		void performoperation2(int i);

		// THIS FUNCTION PERFORM BRANCH OPERATIONS WHENEVER CALLED UPON WITHOUT ANY CONSTANT COMPARISON I.E. COMPARISIONS MUST 
		// BE BETWEEN REGISTERS
		int performoperation3();

		// THIS FUNCTION PERFORM BRANCH OPERATIONS WHENEVER CALLED UPON WITH CONSTANT COMPARISON I.E. COMPARISIONS MUST 
		// BE BETWEEN REGISTER AND A CONSTANT BEFORE BRANCH STATEMENT
		int performoperation4();
	 	
	    int MEM_STAGE();
	    int latency_check();
	    string WB_STAGE();


	 	//------------------------------------------------------------------------------------------------------------------------------------------------//
		//----------------------------------END OF ARM STAGES---------------------------------------------------------------------------------------------//
		//------------------------------------------------------------------------------------------------------------------------------------------------//


		//------------------------------------------------------------------------------------------------------------------------------------------------//
		//----------------------------------SOME OUTPUT FUNCTIONS-----------------------------------------------------------------------------------------//
		//------------------------------------------------------------------------------------------------------------------------------------------------//
		void clock_number();
	 	void output_IF();
	 	void output_ID();
	 	void output_EXE();
	 	void output_MEM();
	 	void output_WB(string r);
	 	void output();
		//------------------------------------------------------------------------------------------------------------------------------------------------//
		//----------------------------------END OF OUTPUT FUNCTIONS---------------------------------------------------------------------------------------//
		//------------------------------------------------------------------------------------------------------------------------------------------------//

    	   // THIS FUNCTION IS CALLED AT THE END TO SHOW THE TOTAL NO. OF INSTRUCTION COUNT FROM MAIN.
	       int instructioncount();

	    	//memory funcion to show contents of memory
	 	    void memoryadd();


};