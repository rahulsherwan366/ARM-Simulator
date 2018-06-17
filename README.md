# ARM-Simulator
## COMPILING STATEMENT:
	Open the terminal folder in which assignment is contained and then type "make".
	After this , type "./processor_simulator" to run contained in executable folder.


## POINTS TO NOTE:
	1.	The input file name must be "sample.txt". You take input files from 
		testcases folder u will find many sub-folders in it . Each sub-folder contain a
		"sample.txt" file.Copy it and paste it in the folder containing makefile. Also note
		 that ,sub-folders are named in such to way to depict various hazard taken care.
	2.	While writing the arm code keep in mind that the free memory has 4000 blocks 
		indexed at zero .So, basically memory address that is accessible is upto 4000 only.
		However, there can be any number of lines in the code since vector is used to keep
		the arm instructions.

## Input File :
	1.	Input file must be named "sample.txt".
	2.	Do not open the input file in gedit or vi or vim , since these editors append empty line in the end of any file which will result in an error. Use editors like Sublime to open input files, so that you can know if any emptyline might not have been there as these editors show the line numbers.
	
## ASSUMPTIONS :
	1.	The arm code should not have any empty line otherwise it will report it as error.
	2.	The arm code will not have any comment and also not have any multiple instruction
	    in the same line.
	3.	The arm code should not have any tab in between otherwise it will show error
	 .However there can be any number of spacebars in between.	 



## INSTRUCTIONS WORKING CORRECTLY:(Only these cases of arm instruction are handled keep a note on this fact)

LDR : 3 CASES CONSIDERED IN IT (A. LDR R1,[R2])
							   (B. LDR R1,[R2,#40])
							   (C. LDR R1,=AA) 
LDR R1,=AA  : NOTE THIS INSTRUCTION WILL NOT SHOW ANY ERROR BUT WILL 
              NOT PROCESS ANY THING .
              FOR RUNNING ASSIGNMENT1 ARM CODE WE HAVE SET R3=4180 SINCE =AA IS NOT PROCESSING ANY THING.   
STR : SIMILAR TO THAT OF LDR.

ADD : WORKING PROPERLY

SUB : WORKING PROPERLY

MUL : WORKING PROPERLY

CMP : WORKING PROPERLY

BNE : WORKING PROPERLY

BGE : WORKING PROPERLY

B   : WORKING PROPERLY

BL  : WORKING PROPERLY

MOV : WORKING PROPERLY

## SPECIAL NOTE:

NOTE: IF YOU ARE WRITING MOV PC,LR INSTRUCTION THEN IT MUST BE IN SMALL LETTERS. THIS 
EXCEPTION IS ONLY FOR MOVING PC, LR  INSTRUCTION AND FOR REST ALL CASES MOV INSTRUCTION
WILL WORK FINE.
