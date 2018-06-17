//HEADERS REQUIRED 
#include <sstream>
#include <iostream>
#include <string.h>
#include <vector>

//HEADERS INCLUDED
#include "enhanced_armsimulator.h"

// IN PUBLIC MODE WE HAVE DEFINED MEMBER FUNCTIONS WHICH AN OBJECT OF enhanced_armsimulator TYPE CAN ACCESS IT


// THIS IS A DEFAULT CONSTRUCTOR USED TO PROVIDE INITIAL VALUES TO THE DATA MEMBERS.
      enhanced_armsimulator::enhanced_armsimulator()
         {
          for(int i=0;i<16;i++){
            regs[i]=0;
          }
          for(int i=0;i<4000;i++){
            memory[i]=0;
          }
          regs[13]=24576;  // PROVIDING INITIAL VALUES
          regs[15]=4096;    // INITIAL STATE OF PROGRAM COUNTER
          regs[3]=0;     // ASSUMPTION SIR LDR R1,=AA , = IS NOT WORKING PROPERLY THATS WHY WE HAVE PROVIDED THE VALUE DIRECTLY TO REGISTER3 ACC. TO ASSIGNMENT 1
          carry=0;          // TO HANDEL COMPARE
          num=0;             // TO HANDLE CONSTANTS
          clock=0;
          instruction_count=0;
          for(int i=0;i<5;i++)
          {
          pipeline_register[0].push_back("");
          pipeline_register[1].push_back("");
          pipeline_register[2].push_back("");
          pipeline_register[3].push_back("");
      	  }
         for(int h=0;h<13;h++)
          {
             latency[h] = 0;
          }
        }

void enhanced_armsimulator::getdata(vector<string> l,vector<int> in,vector<string> s,int x[])
{
	 for(int i=0;i<l.size();i++)
          {
            label.push_back(l[i]);   //coping labels to label data member.
            index.push_back(in[i]);  //coping line index where labels are found to index data member.
           
          }
    for(int i=0;i<s.size();i++)
    	{
    		instruction.push_back(s[i]);  // coping instructions of input file to instruction data member.
	    }
    for(int h=0;h<13;h++)
    {
      latency[h]=x[h];
    }    
}

int enhanced_armsimulator::instruction_section()
{
	int temp,state,flagx=0;
	 for(int i=0;i<instruction.size();i++)
	 {	words=instruction[i];
		temp=error_detection_unit(i+1);
		if((temp==0)&&(flagx==0))
			flagx=1;
	 }
	 if(flagx==1)
	 	return 0;
	 cout<<"ERROR FREE CODE\n";
	 string s;
	//cout<<"rahul";
	 for(int i=0;i<4;i++)
	 {
	 	instruction.push_back("end");
	 }
	 //regs[15]=4096;
	 for(int j=0;j<instruction.size();j++)
	 {
    if((instruction[j].find("terminate")!=-1)||(instruction[j].find(".")!=-1)||(instruction[j].find("swi")!=-1)||(instruction[j].find("end")!=-1)||(instruction[j].find(":")!=-1))
    {
      instruction_count=instruction_count;
    }
    else
    {
      instruction_count+=1;
    }

	 	if(instruction[j].find("=")!=-1)
		regs[3]=4180;
	  
    s=WB_STAGE();
	  temp=MEM_STAGE();
		string k,lexeme;
		if(j>=2)
		lexeme = instruction[j-2];    // GETTING THE LINE.
		else
			lexeme=instruction[j];
      	 for(int p=0;p<lexeme.length();p++){ //REMOVIN THE SPACES
           if(lexeme[p]==' ')
           continue;
           else
           k+=lexeme[p];         
           }
           string keys;
      	  // HERE WE ARE CHECKING BRANCH KEYWORD
           if(((k[0]=='b')||(k[0]=='B'))&&((k[1]=='l')||(k[1]=='L'))) 
           {
            if((k[1]=='l')||(k[1]=='L'))
            {
              if((k[2]=='e')||(k[2]=='E'))
              {
                   keys+=k[0];
                   keys+=k[1];
                   keys+=k[2];
              }
              else 
              {
                 keys+=k[0];
                 keys+=k[1];
              }
            }
            else{
                   keys+=k[0];
                }
           }
          else{  // IF NOT BRANCH THEN IT MUST BE A KEYWORD OF 3 ALPHABET
           keys+=k[0];
           keys+=k[1];
           keys+=k[2];
              }
    if((keys=="bl")||(keys=="BL")||(keys=="Bl"))  // BL CONDITION
    {
      link=j+1;   // THIS LINK STORES THE LINE NUMBER FROM WHERE THE PC J SHOULD START AGAIN 
    }
	    temp=EXE_STAGE();
	    state=temp;
	    if(state==-142)  // IF IT RETURNS -142 THEN IT IS A "mov pc,lr" INSTRUCTION
   		 {
      		j=link;        // SO UPDATE THE VALUE OF J WITH LINK VALUE WHICH WE STORED ABOVE WHILE EXECUTING BL INSTRUCTION

    	 }
    	
	    if(state==-137)  // IF IT RETURNS -137 THEN IT IS A BRANCH CONDITION , WE HAVE TO UPDATE THE JTH LINE.
      { //IF 3 ALPHABET INSTRUCTION
      	cout<<keys<<" "<<endl;
        if((keys=="bne")||(keys=="BNE")||(keys=="Bne")||(keys=="bge")||(keys=="BGE")||(keys=="Bge")||(keys=="ble")||(keys=="BLE")||(keys=="Ble"))
        { //cout<<"rahulbyas sherwan";
          string t=k.substr(3);  
          for(int i=0;i<label.size();i++){
            if(t==label[i])
              j=index[i]; 
          }
        }
        //IF 2 ALPHABET INSTRUCTION 
        else if((keys=="bl")||(keys=="Bl")||(keys=="BL"))
        {  string t=k.substr(2);  
          for(int i=0;i<label.size();i++){
            if(t==label[i])
              j=index[i];
          }

        }
        // IF 1 ALPHABET INSTRUCTION i.e. B
        else{ 
               string t=k.substr(1);  
          for(int i=0;i<label.size();i++){
            if(t==label[i])
              j=index[i];

             }
           }
      }
		temp=ID_STAGE();
		if(temp==-150)
			j--;
		temp=IF_STAGE(j);
		if((state==-137)||(state==-142))
    	{
    		for(int i=0;i<5;i++)
          {
          pipeline_register[0][i]="";
          pipeline_register[1][i]="";
      	  }
    	}	 	
	 	clock+=1;
    temp=latency_check();
	 	 clock_number();
	 	 output_IF();
	 	 output_ID();
	 	 output_EXE();
	 	 output_MEM();
	 	 output_WB(s);
	 	 output();
	 }
   int r;
   memoryadd(); // FINAL STATUS OF FREE MEMORY 
   r=instructioncount()-1;
	 cout<<"\nTOTAL NO OF INSTRUCTION EXECUTED : "<<r<<endl;
   cout<<"\nTOTAL NO OF CLOCK CYCLES : "<<clock<<endl;
   cout<<"\nAVERAGE NO OF INSTRUCTION PER CYCLE (IPC) : "<<(float)((float)r/(float)clock)<<endl;

	return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------ERROR HANDLING BEGINS----------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//




int enhanced_armsimulator::error_detection_unit(int i)
{
	    int flagl=1;       
            
             for(int e=0;e<words.length();e++)  // TO HANDLE WITH LABELS AND .EQU
              {
                if((words[e]==':')||(words[e]=='.'))
                flagl=0;
              }
              string o;
              for(int p=0;p<words.length();p++)
               {         
                        if(words[p]==' ')
                          continue;
                        else
                          o+=words[p];         
                } 
              o+=words[0];
              o+=words[1];
              o+=words[2];
              if((o[0]=='s')&&(o[1]=='w')&&(o[2]=='i'))  //TO HANDLE WITH SWI
                {flagl=0;
                }              
              if(flagl==1)     // IT MEANS NO LABELS,EQU SWI CAN ENTER 
              {  //instruction+=1;
               // regs[15]+=4;
                 string k;
                 int y;
                 //char s=words[0];
                 string t;
                 int j,flag=0;
                  
                    for(j=0;j<words.length();j++)  // ANALYSING KEYWORD
                     { 
                        if(words[j]!=' ') // ELEMINATING INITIAL SPACES i.e ___LDR R1,[R2] WILL BECOME LDR R1,[R2]
                        flag=1;                   
            
                        if(flag==1)  //ANALYSING KEYWORDS
                        {
                            if((words[j]=='B')||(words[j]=='b'))
                              {
                                 if(((words[j+1]=='l')||(words[j+1]=='L'))&&(words[j+2]==' '))
                                  {
                                     t+=words[j];
                                     t+=words[j+1];
                                     flag=2;
                                  }
                                   else if(((words[j+1]=='l')||(words[j+1]=='L')||(words[j+1]=='n')||(words[j+1]=='N')||(words[j+1]=='G')||(words[j+1]=='g'))&&(words[j+2]!=' '))
                                  {
                                      t+=words[j];
                                      t+=words[j+1];
                                      t+=words[j+2];
                                      flag=2; 
                                  }
                                  else
                                 {
                                  t+=words[j];
                                  flag=2;
                                  } 
                              }   
                               else
                              {
                                  t+=words[j];
                                   t+=words[j+1];
                                  t+=words[j+2];
                                  flag=2;
                              } 
                       }
                    
                      if(flag==2)
                      break;
                    }           
              key=t;   // FINALLY KEYWORD AFTER ANALYSIS IS BEING COPIED TO KEY DATA MEMBER WHICH WE ACCESS FROM ANY FUNCTION OF THIS CLASS
            
             if((t[0]!='b')&&(t[0]!='B'))   // IF IT IS NOT A BRACH INSTRUCTION THEN TRUE
             {
            
               for(int p=0;p<words.length();p++) // THIS LOOP IS BASICALLY REMOVING THE SPACES i.e IF IT IS ADD R1,   R2,R3  THEN IT WILL BECOME ADDR1,R2,R3
               {         
                        if(words[p]==' ')
                          continue;
                        else
                          k+=words[p];         
                } 
         
             checkkeyword(i); // CHECKING WHETHER IT IS A VALID KEYWORD OR NOT
            
              if((k!="movpc,lr")) // THIS IS SPECIFICALLY OF HANDLING BL STATEMENTS  
              { if((key=="ldr")){ 
                  i=i;}
                  else if(key=="str"){
                    i=i;
                  }
                 else{                                // to check the first keyword
                 for(int l=0;l<words.length();l++)  // for checking the possibility of r__0 which is an error
                 {              
                  if(((words[l]=='r')||(words[l]=='R'))&&((words[l+1]<'0')||(words[l+1]>'9')))
                   { flag=3;}
                 }
               }
              }
                    
              if(flag==3)
               {cout<<"error in line :"<<i<<endl;            
                 y=0;
                 goto x;    //to go to x label witout executing the following statement                         
                }
            
           words=k; // WORDS IS BEING UPDATED , NOW IT HAS INSTRUCTIONS WITHOUT ANY SPACES
           
             if((words!="movpc,lr"))  
              { 
                y=checkerror(i);  // CHECKING THE SYNTAX ERROR
                //if(y==1)
                 //performoperation(i);  // IF IT IS TRUE THEN WE ARE ALLOWED TO PERFORM OPERATIONS
              
              //if((y!=1)&&(y!=0))  
                //{
                 // performoperation2(y); // IF IT CONTAINS ANY CONSTANTS THEN THIS FUNCTION WILL HANDL IT
                //}
              }
     //      else {  // THIS ELSE OVER HERE HANDLES BL RESUMING CONDITION i.e MOV PC,LR
     //                 if(regs[14]==0)  //IF REG[14]== 0 MEANS NO LINK IS THERE WHICH WILL BE AN ERROR
     // /*DOUBT TENSION*/                 { cout<<"error in line:"<<i<<endl;
     //                     y=0;
     //                   }
     //                 else{ // ELSE UPDATE THE PROGRAM COUNTER AND MAKE REG[14] =0
     //                      regs[15]=regs[14];
     //                      regs[14]=0;
     //                      y=-142;
     //                     }
     //           }
      
             x:i=i;   // GOTO LABEL IS HERE
             return y; 
           } 
       
       else {  // THIS ELSE IS FOR HANDLING BRANCH OPERATIONS
               for(int p=0;p<words.length();p++)// REMOVING SPACES IF ANY
               {         
                if(words[p]==' ')
                  continue;
                else
                   k+=words[p];         
                }

                words=k;   // UPDATING THE WORDS
                checkkeyword(i); // CHECKING FOR ANY SYNTAX ERROR FOR VARIOUS BRANCH INSTRUCTION
                int x=checkerror2(i);
                
                //if(x!=0){ // IF NOT AN ERROR
                //          x=performoperation3(); //THEN PERFORM OPERATION
                  //        if(x==1)
              //            x=performoperation4();// FUNTION FOR HANDLING CONSTANTS
                    //    }
            //     output(); // PRINTING THE OUTPUT AFTER PERFORMING OPERATION
                return x;
            }
            
            }  
       else{ // THIS ELSE IS FOR LABEL ,SWI,.EQU WHICH DOESN'T NEED TO BE PROCESSED
              return 1;  
            }
} 



// THIS FUNCTION EXPLOITS THE  BRANCH INSTRUCTION WITH THREE ALPHABETS SUCH AS BLE,BNE ETC.
        int enhanced_armsimulator::possibility3(int i)
        {
          string temp;
          temp=words.substr(3);
           for(int k=0;k<label.size();k++)
           {
            if(temp==label[k])
              return 1;
          }
          cout<<"error in line :"<<i<<endl;
          return 0;
        } 



         
// THIS FUNCTION EXPLOITS THE  BRANCH INSTRUCTION WITH TWO ALPHABETS SUCH AS BL.
         int enhanced_armsimulator::possibility4(int i)
        {
          string temp;
          temp=words.substr(2);
           for(int k=0;k<label.size();k++)
           {
            if(temp==label[k])
              return 1;
          }
          cout<<"error in line :"<<i<<endl;
          return 0;
        } 



// THIS FUNCTION EXPLOITS THE  BRANCH INSTRUCTION WITH SINGLE ALPHABETS SUCH AS B.
         int enhanced_armsimulator::possibility5(int i)
        {
          string temp;
          temp=words.substr(1);
           for(int k=0;k<label.size();k++)
           {
            if(temp==label[k])
              return 1;
          }
          cout<<"error in line :"<<i<<endl;
          return 0;
        }



// THIS FUNCTION CHECKS ALL BRANCH INSTRUCTION WHENEVER CALLED UPON  AND RETURNS THE VALUES WHICH
// IT RECIEVES FROM POSSIBILITY 3,4,5 FUNCTION, ONE AT A TIME.        
        int enhanced_armsimulator::checkerror2(int i)
        {
           string temp;           
          temp+=words[0];
          if((words[1]=='n')||(words[1]=='N')||(words[1]=='l')||(words[1]=='L')||(words[1]=='g')||(words[1]=='G'))
          temp+=words[1];
          if((words[2]=='e')||(words[2]=='E'))
          temp+=words[2];
          if((temp=="bne")||(temp=="Bne")||(temp=="BNE")||(temp=="ble")||(temp=="BLE")||(temp=="Ble")||(temp=="BGE")||(temp=="bge")||(temp=="Bge"))  
          {
            return(possibility3(i));
          }
          else if((temp=="bl")||(temp=="Bl")||(temp=="BL"))  
          {
            return(possibility4(i));
          } 
          else
          {
            return(possibility5(i));
          }

        }




// THIS FUNCTION CHECKS WHETHER THE KEY WORD IS VALID OR NOT

        void enhanced_armsimulator::checkkeyword(int i)
        {
            if((key=="mov")||(key=="MOV")||(key=="Mov")||(key=="cmp")||(key=="CMP")||(key=="Cmp")||(key=="add")||(key=="ADD")||(key=="Add"))
              {
              	if((key=="mov")||(key=="MOV")||(key=="Mov"))
              	{
              		key="mov";
              	}
              	if((key=="cmp")||(key=="CMP")||(key=="Cmp"))
              	{
              		key="cmp";
              	}
              	if((key=="add")||(key=="ADD")||(key=="Add"))
              	{
              		key="add";
              	}	
              }
            else if((key=="sub")||(key=="SUB")||(key=="Sub")||(key=="mul")||(key=="MUL")||(key=="Mul")||(key=="ldr")||(key=="LDR")||(key=="Ldr"))
              {
              		if((key=="sub")||(key=="SUB")||(key=="Sub")){
              			key="sub";
              		}
              		if((key=="mul")||(key=="MUL")||(key=="Mul")){
              			key="mul";
              		}	
              		if((key=="ldr")||(key=="Ldr")||(key=="LDR")){
                  		key="ldr";
                	}
              }
            else if((key=="str")||(key=="STR")||(key=="Str")||(key=="bne")||(key=="BNE")||(key=="Bne")||(key=="bge")||(key=="BGE")||(key=="Bge")||(key=="ble")||(key=="BLE")||(key=="Ble"))
              {	
                 if((key=="bne")||(key=="BNE")||(key=="Bne"))
                 {
                 	key="bne";
                 }
                 if((key=="bge")||(key=="BGE")||(key=="Bge"))
                 {
                 	key="bge";
                 }	
                 if((key=="ble")||(key=="BLE")||(key=="Ble"))
                 {
                 	key="ble";
                 }
                 if((key=="str")||(key=="Str")||(key=="STR"))
                 {
                  key="str";
                 }
              }
            else if((key=="B")||(key=="b")||(key=="bl")||(key=="BL")||(key=="Bl"))
             {
             	if((key=="B")||(key=="b"))
             	{
             		key="b";
             	}
             	if((key=="bl")||(key=="BL")||(key=="Bl"))
             	{
             		key="bl";
             	}	
             }
        } 




// THIS FUNCTION CHECKS ALL POSSIBILITIES OF MOV AND CMP COMMAND
        int enhanced_armsimulator::possibility1(int i)     // this function is for exploiting all error possibilities which can take place in mov or cmp command
        {
          if(((words[3]=='r')||(words[3]=='R'))&&((words[4]>='0')&&(words[4]<='9'))&&((words[5]==',')))
          {    r1=words[4]-48;
              if(((words[6]=='r')||(words[6]=='R'))&&((words[7]>='0')&&(words[7]<='9')))
              { 
                r2=words[7]-48;  
                return 1;
              }
              else if(words[6]=='#')
              {
                return (possibilityhash('m',i));
              }  
              else {cout<<"error in line :"<<i<<endl;
                      return 0;
                    }
          }
          else 
          {
            cout<<"error in line :"<<i<<endl;
            return 0;
          }
        }



// THIS FUNCTION CHECKS ALL POSSIBILITIES OF ADD,SUB AND MUL COMMAND.
      int enhanced_armsimulator::possibility2(int i)     // this function is for exploiting all error possibilities which can take place in add or sub or mul command
        {
          if(((words[3]=='r')||(words[3]=='R'))&&((words[4]>='0')&&(words[4]<='9'))&&((words[5]==','))&&((words[6]=='r')||(words[6]=='R'))&&((words[7]>='0')&&(words[7]<='9')&&(words[8]==',')))
          {    r1=words[4]-48;
               r2=words[7]-48;
              if(((words[9]=='r')||(words[9]=='R'))&&((words[10]>='0')&&(words[10]<='9')))
              { r3=words[10]-48;                  
                return 1;
              }
              else if(words[9]=='#')
              {
                return (possibilityhash('a',i));
              }  
              else {cout<<"error in line :"<<i<<endl;
                      return 0;
                    }
          }
          else 
             {
               cout<<"error in line :"<<i<<endl;
               return 0;
             }
        }




// THIS FUNCTION CHECKS ALL THE POSSIBILITIES OF LOAD AND STORE COMMAND , FOR SIMPLIFICATION WE HAVE 3 POSSIBLE VARIATIONS OF LDR AND STR
          int enhanced_armsimulator::possibilitymemory(int i)    
        { 
          if(((words[3]=='r')||(words[3]=='R'))&&((words[4]>='0')&&(words[4]<='9'))&&((words[5]==','))&&(words[6]=='[')&&((words[7]=='r')||(words[7]=='R'))&&((words[8]>='0')&&(words[8]<='9')))
          {    r1=words[4]-48;
               r2=words[8]-48;
              if(words[9]==']')
              {                   
                return 1;
              }
              else if((words[9]==',')&&(words[10]=='#')&&(words[words.length()-1]==']'))
              {
                return (possibilityhash('l',i));
              }  
              else {cout<<"error in line :"<<i<<endl;
                      return 0;
                    }
          }
          else if(((words[3]=='r')||(words[3]=='R'))&&((words[4]>='0')&&(words[4]<='9'))&&((words[5]==','))&&(words[6]=='='))
          {     string t=words.substr(7);
                regs[3]=4180;
                  for(int r=0;r<label.size();r++)
                  { 
                     if(t==label[r])
                      return 1;
                  }
                  cout<<"error in line :"<<i<<endl;
                      return 0;

          }
          else 
             {
               cout<<"error in line :"<<i<<endl;
               return 0;
             }
        }




// THIS FUNCTION RETURNS AN INTEGER CONSTANTS AFTER CHECKING ERROR  IN ANY #INTEGER VALUES.
        int enhanced_armsimulator::possibilityhash(char i,int r)
        {
            if((i=='m')||(i=='a')||(i=='l'))
                      {  int start; 
                        if(i=='m')
                         start=7;
                        if(i=='a')
                          start=10;
                        if(i=='l')
                          start=11;
                         int flag=0;
                         if((start==7)||(start==10))
                         {
                         	for(int j=start;j<words.length();j++)
                            {
                              if((words[j]<'0')||(words[j]>'9'))
                              flag=1;
                            }
                         }
                         if(start==11)
             			 {
                         	for(int j=start;j<(words.length()-1);j++)
                            {
                              if((words[j]<'0')||(words[j]>'9'))
                              flag=1;
                            }
                            if(words[words.length()-1]!=']')
                               flag=1;
                         }
                         if(flag==1)
                         { cout<<"error in line :"<<r<<endl;
                           return 0;
                         }
                         else{
                              string s;
                             for(int i=start;i<words.length();i++)
                             {
                              s+=words[i];
                             }
                             stringstream geek(s);
                             int x = 0;
                             geek >> x;
                             // if((key=="ldr")||(key=="LDR")||(key=="Ldr")||(key=="str")||(key=="Str")||(key=="STR"))
                             //  x=x/4;
                             if(x==1)
                                return(-11);
                             else if(x==0)
                                return(-17);
                             else                              
                                return(x);
                         }   
                      }
                else {
                        cout<<"error in line :"<<r<<endl;
                        return 0;
                    }
        }




// THIS FUNCTION CHECKS ERROR AND RETURNS THE VALUES AFTER DEALING WITH VARIOUS POSSIBILITIES.
        int enhanced_armsimulator::checkerror(int i){
          string temp;
          temp+=words[0];
          temp+=words[1];
          temp+=words[2];
          if(((temp=="mov")||(temp=="MOV")||(temp=="Mov"))||((temp=="cmp")||(temp=="CMP")||(temp=="Cmp")))
          {
            return(possibility1(i));
          }

          else if(((temp=="add")||(temp=="ADD")||(temp=="Add"))||((temp=="sub")||(temp=="SUB")||(temp=="Sub"))||((temp=="MUL")||(temp=="Mul")||(temp=="mul")))
          { 
            return(possibility2(i));
          }

          else if(((temp=="ldr")||(temp=="LDR")||(temp=="Ldr"))||((temp=="str")||(temp=="STR")||(temp=="Str")))
          {
            return(possibilitymemory(i));
          }

          else{
            cout<<"error in line :"<<i<<endl;
            return 0;
          }
        }




//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------ERROR HANDLING ENDS ----------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------IF-STAGE----------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
        int enhanced_armsimulator::IF_STAGE(int i)
        {	regs[15]+=4;
        	if(instruction[i]=="end")
        		regs[15]-=4;
        	pipeline_register[0][0]=instruction[i];
        	 int flagl=1;       
            
             for(int e=0;e<instruction[i].length();e++)  // TO HANDLE WITH LABELS AND .EQU
              {
                if((instruction[i][e]==':')||(instruction[i][e]=='.'))
                flagl=0;
              }
             if(flagl==0)
             {
             	regs[15]-=4;
             	pipeline_register[0][0]="label_detected";
             } 
			   
			return 0;     	
        }
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------IF_STAGE ENDS-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------ID_STAGE----------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
         int enhanced_armsimulator::ID_STAGE()
         {
         	string k,temp=pipeline_register[0][0];
         	if(pipeline_register[0][0]=="")
         		{	pipeline_register[1][0]="";
         		for(int i=1;i<pipeline_register[1].size();i++)
         		{	
         			pipeline_register[1][i]="";
         		}
         		return 0;
         	}
         	if(pipeline_register[0][0]=="end")
         	{	pipeline_register[1][0]="end";
         		for(int i=1;i<pipeline_register[1].size();i++)
         		{	
         			pipeline_register[1][i]="";
         		}
         		return 0;
         	}
         	if(pipeline_register[0][0]=="label_detected")
         	{	pipeline_register[1][0]="label_detected";
         		for(int i=1;i<pipeline_register[1].size();i++)
         		{	
         			pipeline_register[1][i]="";
         		}
         		return 0;
         	}
         	string u;
         	for(int p=0;p<pipeline_register[0][0].length();p++)// REMOVING SPACES IF ANY
               {         
                if(pipeline_register[0][0][p]==' ')
                  continue;
                else{
                	if((pipeline_register[0][0][p]>='A')&&(pipeline_register[0][0][p]<='Z'))
                		pipeline_register[0][0][p]+=32;
                   u+=pipeline_register[0][0][p];         
                }
                }
            string o;
            if(u.length()>4)
            o=u.substr(5);
        	else o="";
        	// cout<<o<<endl;
         	if((pipeline_register[1][0]=="ldr")&&(o.find("r"+pipeline_register[1][2])!=-1))
         	{
         		pipeline_register[1][0]="";
         		for(int i=1;i<pipeline_register[1].size();i++)
         		{	
         			pipeline_register[1][i]="";
         		}
         		return -150;	
         	}
         	if((pipeline_register[1][0]=="ldr")&&(o.find(pipeline_register[1][1])!=-1))
         	{
         		pipeline_register[1][0]="";
         		for(int i=1;i<pipeline_register[1].size();i++)
         		{	
         			pipeline_register[1][i]="";
         		}
         		return -150;	
         	}	
         	// for(int i=0;i<pipeline_register[1].size();i++)
         	// 	{
         	// 		pipeline_register[1].push_back("");
         	// 	}
         	 for(int p=0;p<temp.length();p++) // THIS LOOP IS BASICALLY REMOVING THE SPACES i.e IF IT IS ADD R1,   R2,R3  THEN IT WILL BECOME ADDR1,R2,R3
               {         
                        if(temp[p]==' ')
                          continue;
                        else
                          k+=temp[p];         
                }
                temp=k;
             if(((temp[0]=='B')||(temp[0]=='b'))&&((temp[1]=='l')||(temp[1]=='L'))&&((temp[2]=='R')||(temp[2]=='r')))
             {
             	key="bl";
             }
       		 else if(((temp[0]=='B')||(temp[0]=='b'))&&((temp[1]=='r')||(temp[1]=='R')))
             {
             	key="b";
             }
             else if(((temp[0]=='B')||(temp[0]=='b'))&&((temp[1]=='l')||(temp[1]=='L'))&&((temp[2]=='e')||(temp[2]=='E')))
             {
             	key="ble";
             }
             else if((temp[0]=='B')||(temp[0]=='b'))
             {
             	if((temp[1]=='N')||(temp[1]=='n'))
             		key="bne";
             	else
             		key="bge";
             } 
              else if((temp[0]=='A')||(temp[0]=='a'))
             {
             	key="add";
             }
              else if((temp[0]=='s')||(temp[0]=='S'))
          	 {
          	 	if((temp[1]=='u')||(temp[1]=='U'))
          	 	key="sub";
          	 	else
          	 		key="str";
          	 }
          	 else if((temp[0]=='m')||(temp[0]=='M'))
          	 {	
          	 	if((temp[1]=='u')||(temp[1]=='U'))
          	 	key="mul";
          	 	else
          	 		key="mov";
          	 }
          	else if((temp[0]=='l')||(temp[0]=='L'))
          	 {
          	 	key="ldr";
          	 }
          	 else if((temp[0]=='c')||(temp[0]=='C'))
          	 {
          	 	key="cmp";
          	 }

          	 words=temp;
          	 if((key=="add")||(key=="mul")||(key=="sub"))
          	 {	
          	 if((temp[9]=='r')||(temp[9]=='R'))
             	{	int x;
             		string f,l;
             		x=possibility2(0);
             		pipeline_register[1][0]=key;
             		f=r1+48;
             		l="r";
             		l=l+f;
             		pipeline_register[1][1]=l;
             		f=r2+48;
             		pipeline_register[1][2]=f;
             		f=r3+48;
             		pipeline_register[1][3]=f;	
             		pipeline_register[1][4]="type-R";
             	}
             	else
             	{
             		int x;
             		string f,l;
             		x=possibility2(0);
             		pipeline_register[1][0]=key;
             		f=r1+48;
             		l="r";
             		l=l+f;
             		pipeline_register[1][1]=l;
             		f=r2+48;
             		pipeline_register[1][2]=f;
             		if(x==-11)
             			l=49;
             		else if(x==-17)
             			l=48;
             		else
             			l=to_string(x);
             		pipeline_register[1][3]=l;
             		pipeline_register[1][4]="type-I";

             	}
          	 }
          	 else if((key=="mov")||(key=="cmp"))
          	 {
          	 	if((temp[6]=='r')||(temp[6]=='R'))
          	 	{	int x;
             		string f,l;
             		x=possibility1(0);
             		pipeline_register[1][0]=key;
             		f=r1+48;
             		l="r";
             		l=l+f;
             		pipeline_register[1][1]=l;
             		f=r2+48;
             		pipeline_register[1][2]=f;
             		pipeline_register[1][3]="type-R";
             		pipeline_register[1][4]="";
             	}
             	else
             	{
             		int x;
             		string f,l;
             		x=possibility1(0);
             		pipeline_register[1][0]=key;
             		f=r1+48;
             		l="r";
             		l=l+f;
             		pipeline_register[1][1]=l;
             		if(x==-11)
             			l=49;
             		else if(x==-17)
             			l=48;
             		else
             			l=to_string(x);
             		pipeline_register[1][2]=l;
             		pipeline_register[1][3]="type-I";
             		pipeline_register[1][4]="";
             	}
          	 }
          	 else if((key=="ldr")||(key=="str"))
          	 {
          	 	if((temp[9]==']')&&(temp[6]!='='))
          	 	{
          	 		int x;
             		string f,l;
             		x=possibilitymemory(0);
             		pipeline_register[1][0]=key;
             		f=r1+48;
             		l="r";
             		l=l+f;
             		pipeline_register[1][1]=l;
             		f=r2+48;
             		pipeline_register[1][2]=f;	
             		pipeline_register[1][3]="type-R";
          	 		pipeline_register[1][4]="";
          	 	}
          	 	else if((temp[10]=='#')&&(temp[6]!='='))
          	 	{
          	 		int x;
             		string f,l;
             		x=possibilitymemory(0);
             		pipeline_register[1][0]=key;
             		f=r1+48;
             		l="r";
             		l=l+f;
             		pipeline_register[1][1]=l;
             		f=r2+48;
             		pipeline_register[1][2]=f;
             		if(x==-11)
             			l=49;
             		else if(x==-17)
             			l=48;
             		else
             			l=to_string(x);
             		pipeline_register[1][3]=l;
             		pipeline_register[1][4]="type-I";	
          	 	}
          	 	//ASSUMING THAT WE DON'T HAVE ANY INSTRUCTIONS LIKE ldr r1,=label
          	 }
          	 else if((key=="bne")||(key=="bge")||(key=="ble"))
          	 {
          	 	pipeline_register[1][0]=key;
          	 	pipeline_register[1][1]=words;
          	 	pipeline_register[1][2]="";
          	 	pipeline_register[1][3]="";
          	 	pipeline_register[1][4]="";
          	 }
          	 else if(key=="bl")
          	 {
          	 	pipeline_register[1][0]=key;
          	 	pipeline_register[1][1]=words;
          	 	pipeline_register[1][2]="";
          	 	pipeline_register[1][3]="";
          	 	pipeline_register[1][4]="";
          	 }
          	 else if(key=="b")
          	 {
          	 	pipeline_register[1][0]=key;
          	 	pipeline_register[1][1]=words;
          	 	pipeline_register[1][2]="";
          	 	pipeline_register[1][3]="";
          	 	pipeline_register[1][4]="";
          	 }
          	 return 0;
         }
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------ID_STAGE ENDS-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------EXE_STAGE----------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
          int enhanced_armsimulator::EXE_STAGE()
          {		string k;
         		if(pipeline_register[1][0]=="")
         		{
         			pipeline_register[2][0]="";
         			for(int i=1;i<pipeline_register[2].size();i++)
         			{	
         			pipeline_register[2][i]="";
         			}

         		return 0;
         		}
         		if(pipeline_register[1][0]=="end")
         		{	pipeline_register[2][0]="end";
         			for(int i=1;i<pipeline_register[2].size();i++)
         			{	
         			pipeline_register[2][i]="";
         			}
         			return 0;
         		}
         		if(pipeline_register[1][0]=="label_detected")
         		{	pipeline_register[2][0]="label_detected";
         			for(int i=1;i<pipeline_register[2].size();i++)
         			{	
         			pipeline_register[2][i]="";
         			}
         			return 0;
         		}
         		//forwarding begins here for various cases
         		//hazard detection conditions.
         		//# mov and cmp statements 
         		//1a.EX/MEM.RegisterRd=ID/EX.RegisterRs or 1b.EX/MEM.RegisterRd=ID/EX.RegisterRt
         		//2a.MEM/WB.RegisterRd=ID/EX.RegisterRs or 2b.MEM/WB.RegisterRd=ID/EX.RegisterRt
         		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         		//-------------------------------------R-TYPE_FORWARDING--------------------------------------------------------------------------------------//
         		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


         		if(((pipeline_register[1][0]=="mov"))&&(pipeline_register[1][3]=="type-R")&&(("r"+pipeline_register[1][2]==pipeline_register[2][1])))
         		{	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			performoperation(-6);
          			return 0;
         		}
         		
         		if(((pipeline_register[1][0]=="mov"))&&(pipeline_register[1][3]=="type-R")&&(("r"+pipeline_register[1][2]==pipeline_register[3][0])))
         		{
         			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			performoperation(-7);
          			return 0;
         		}

         		if(((pipeline_register[1][0]=="cmp"))&&(pipeline_register[1][3]=="type-R")&&(("r"+pipeline_register[1][2]==pipeline_register[2][1])))
         		{	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			if(pipeline_register[1][1]==pipeline_register[3][0])
          			{
          				performoperation(-8);
          			}
          			else if(pipeline_register[1][1]==pipeline_register[2][1])
          			{
          				performoperation(-9);
          			}
          			else{string t=pipeline_register[1][1].substr(1);
          					r1=t[0]-48;
          					r2=pipeline_register[1][2][0]-48;
          				pipeline_register[2][1]=pipeline_register[1][1];
          				performoperation(-6);	
          			}
          			
          			return 0;
         		}
         		//2a.MEM/WB.RegisterRd=ID/EX.RegisterRs or 2b.MEM/WB.RegisterRd=ID/EX.RegisterRt
         		if(((pipeline_register[1][0]=="cmp"))&&(pipeline_register[1][3]=="type-R")&&(("r"+pipeline_register[1][2]==pipeline_register[3][0])))
         		{
         			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[3][0];
          			if(pipeline_register[1][1]==pipeline_register[3][0])
          			{
          				performoperation(-10);
          			}
          			else if(pipeline_register[1][1]==pipeline_register[2][1])
          			{
          				performoperation(-11);
          			}
          			else{string t=pipeline_register[1][1].substr(1);
          					r1=t[0]-48;
          					r2=pipeline_register[1][2][0]-48;
          				pipeline_register[2][1]=pipeline_register[1][1];
          				performoperation(-7);
          			}
          			return 0;
         		}

         		if(((pipeline_register[1][0]=="cmp"))&&(pipeline_register[1][3]=="type-R")&&((pipeline_register[1][1]==pipeline_register[2][1])))
         		{	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			if(pipeline_register[1][2]==pipeline_register[3][0])
          			{
          				performoperation(-12);
          			}
          			else if(pipeline_register[1][2]==pipeline_register[2][1])
          			{
          				performoperation(-13);
          			}
          			else{string t=pipeline_register[1][1].substr(1);
          					r1=t[0]-48;
          					r2=pipeline_register[1][2][0]-48;
          				pipeline_register[2][1]=pipeline_register[1][2];
          				performoperation(-14);	
          			}
          			
          			return 0;
         		}
         		//2a.MEM/WB.RegisterRd=ID/EX.RegisterRs or 2b.MEM/WB.RegisterRd=ID/EX.RegisterRt
         		if(((pipeline_register[1][0]=="cmp"))&&(pipeline_register[1][3]=="type-R")&&((pipeline_register[1][1]==pipeline_register[3][0])))
         		{
         			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[3][0];
          			if(pipeline_register[1][2]==pipeline_register[3][0])
          			{
          				performoperation(-15);
          			}
          			else if(pipeline_register[1][2]==pipeline_register[2][1])
          			{
          				performoperation(-16);
          			}
          			else{
          				string t=pipeline_register[1][1].substr(1);
          					r1=t[0]-48;
          					r2=pipeline_register[1][2][0]-48;
          				pipeline_register[2][1]=pipeline_register[1][2];
          				performoperation(-17);
          			}
          			return 0;
         		}

         		//# add,sub and mul statements 
         		//1a.EX/MEM.RegisterRd=ID/EX.RegisterRs or 1b.EX/MEM.RegisterRd=ID/EX.RegisterRt
         		if(((pipeline_register[1][0]=="add")||(pipeline_register[1][0]=="mul")||(pipeline_register[1][0]=="sub"))&&(pipeline_register[1][4]=="type-R")&&(("r"+pipeline_register[1][2]==pipeline_register[2][1])))
         		{	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			if("r"+pipeline_register[1][3]==pipeline_register[3][0])
          			{	r3=pipeline_register[1][3][0]-48;
          				performoperation(-8);
          			}
          			else if("r"+pipeline_register[1][3]==pipeline_register[2][1])
          			{	r3=pipeline_register[1][3][0]-48;
          				performoperation(-9);
          			}
          			else{
          				r3=pipeline_register[1][3][0]-48;
          				performoperation(-6);	
          			}
          			
          			return 0;
         		}
         		//2a.MEM/WB.RegisterRd=ID/EX.RegisterRs or 2b.MEM/WB.RegisterRd=ID/EX.RegisterRt
         		if(((pipeline_register[1][0]=="add")||(pipeline_register[1][0]=="mul")||(pipeline_register[1][0]=="sub"))&&(pipeline_register[1][4]=="type-R")&&(("r"+pipeline_register[1][2]==pipeline_register[3][0])))
         		{
         			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			if("r"+pipeline_register[1][3]==pipeline_register[3][0])
          			{	r3=pipeline_register[1][3][0]-48;
          				performoperation(-10);//cout<<"asdasdassdasdasd";
          			}
          			else if("r"+pipeline_register[1][3]==pipeline_register[2][1])
          			{	r3=pipeline_register[1][3][0]-48;
          				performoperation(-11); //cout<<"asdasdassdasdasd";
          			}
          			else{
          				r3=pipeline_register[1][3][0]-48;
          				performoperation(-7);//cout<<"sdff";
          			}
          			return 0;
         		}

         		if(((pipeline_register[1][0]=="add")||(pipeline_register[1][0]=="mul")||(pipeline_register[1][0]=="sub"))&&(pipeline_register[1][4]=="type-R")&&(("r"+pipeline_register[1][3]==pipeline_register[2][1])))
         		{	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			if("r"+pipeline_register[1][2]==pipeline_register[3][0])
          			{	r2=pipeline_register[1][2][0]-48;
          				performoperation(-12);
          			}
          			else if("r"+pipeline_register[1][2]==pipeline_register[2][1])
          			{	r2=pipeline_register[1][2][0]-48;
          				performoperation(-13);
          			}
          			else{
          				r2=pipeline_register[1][2][0]-48;
          				performoperation(-14);	
          			}
          			
          			return 0;
         		}
         		//2a.MEM/WB.RegisterRd=ID/EX.RegisterRs or 2b.MEM/WB.RegisterRd=ID/EX.RegisterRt
         		if(((pipeline_register[1][0]=="add")||(pipeline_register[1][0]=="mul")||(pipeline_register[1][0]=="sub"))&&(pipeline_register[1][4]=="type-R")&&(("r"+pipeline_register[1][3]==pipeline_register[3][0])))
         		{
         			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			if("r"+pipeline_register[1][2]==pipeline_register[3][0])
          			{	r2=pipeline_register[1][2][0]-48;
          				performoperation(-15);//cout<<"asdasdassdasdasd";
          			}
          			else if("r"+pipeline_register[1][2]==pipeline_register[2][1])
          			{	r2=pipeline_register[1][2][0]-48;
          				performoperation(-16);// cout<<"asdasdassdasdasd";
          			}
          			else{
          				r2=pipeline_register[1][2][0]-48;
          				performoperation(-17);//cout<<"sdff";
          			}
          			return 0;
         		}

         		if((pipeline_register[1][0]=="ldr")&&(pipeline_register[1][3]=="type-R")&&(("r"+pipeline_register[1][2]==pipeline_register[2][1])))
         		{
         			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			performoperation(-6);
          			return 0;
         		}	

         		if((pipeline_register[1][0]=="ldr")&&(pipeline_register[1][3]=="type-R")&&(("r"+pipeline_register[1][2]==pipeline_register[3][0])))
         		{
         			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			performoperation(-7);
          			return 0;
         		}	

         		if((pipeline_register[1][0]=="str")&&(pipeline_register[1][3]=="type-R")&&((pipeline_register[1][1]==pipeline_register[2][1])))
         		{
         			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[2][1];str_key=1;str_value=pipeline_register[2][2];
          			performoperation(0);
          			return 0;
         		}

         		if((pipeline_register[1][0]=="str")&&(pipeline_register[1][3]=="type-R")&&(("r"+pipeline_register[1][2]==pipeline_register[3][0])))
         		{
         			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[3][0];str_key=1;str_value=pipeline_register[3][1];
          			performoperation(0);
          			return 0;
         		}
         		//hazard_forwarding ends here.
         		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         		//-----------------------------------R_TYPE-FORWARDING ENDS------------------------------------------------------------------------------------------//
         		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

         		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         		//-----------------------------------I_TYPE-FORWARDING BEGINS------------------------------------------------------------------------------------------//
         		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				if(((pipeline_register[1][0]=="cmp"))&&(pipeline_register[1][3]=="type-I")&&((pipeline_register[1][1]==pipeline_register[2][1])))
         		{	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			stringstream geek(pipeline_register[1][2]);
          				     int i=0;
          				     geek >> i;num=-6;//cout<<"india"<<" "<<i<<endl;
          				     performoperation2(i);cout<<carry<<endl;
          			return 0;
         		}  

         		if(((pipeline_register[1][0]=="cmp"))&&(pipeline_register[1][3]=="type-I")&&((pipeline_register[1][1]==pipeline_register[3][0])))
         		{	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[3][0];
          			stringstream geek(pipeline_register[1][2]);
          				     int i=0;
          				     geek >> i;num=-7; 
          				     performoperation2(i);
          			return 0;
         		}  

         		if(((pipeline_register[1][0]=="add")||(pipeline_register[1][0]=="mul")||(pipeline_register[1][0]=="sub"))&&(pipeline_register[1][4]=="type-I")&&(("r"+pipeline_register[1][2]==pipeline_register[2][1])))
				 {	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
				 	stringstream geek(pipeline_register[1][3]);
          				     int i=0;
          				     geek >> i; num=-6;
          				       r2=pipeline_register[1][2][0]-48;
          				     performoperation2(i);
          		    return 0;
				 }

				 if(((pipeline_register[1][0]=="add")||(pipeline_register[1][0]=="mul")||(pipeline_register[1][0]=="sub"))&&(pipeline_register[1][4]=="type-I")&&(("r"+pipeline_register[1][2]==pipeline_register[3][0])))
				 {	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
				 	stringstream geek(pipeline_register[1][3]);
          				     int i=0;
          				     geek >> i;  num=-7;
          				       r2=pipeline_register[1][2][0]-48;
          				     performoperation2(i);
          		    return 0;
				 }

				 if((pipeline_register[1][0]=="ldr")&&(pipeline_register[1][4]=="type-I")&&(("r"+pipeline_register[1][2]==pipeline_register[2][1])))
     			 {
     			 	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			stringstream geek(pipeline_register[1][3]);
          				     int i=0;
          				     geek >> i;  num=-6;
          				     performoperation2(i);
          		    return 0;
     			 }

     			  if((pipeline_register[1][0]=="ldr")&&(pipeline_register[1][4]=="type-I")&&(("r"+pipeline_register[1][2]==pipeline_register[3][0])))
     			 {
     			 	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			stringstream geek(pipeline_register[1][3]);
          				     int i=0;
          				     geek >> i;  num=-7;
          				     performoperation2(i);
          		    return 0;
     			 }

     			if((pipeline_register[1][0]=="str")&&(pipeline_register[1][4]=="type-I")&&((pipeline_register[1][1]==pipeline_register[2][1])))
         		{
         			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[2][1];str_key=1;str_value=pipeline_register[2][2];
          			stringstream geek(pipeline_register[1][3]);
          				     int i=0;
          				     geek >> i;  
          				     performoperation2(i);
          			return 0;
         		}

         		if((pipeline_register[1][0]=="str")&&(pipeline_register[1][3]=="type-R")&&(("r"+pipeline_register[1][2]==pipeline_register[3][0])))
         		{
         			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[3][0];str_key=1;str_value=pipeline_register[3][1];
          			stringstream geek(pipeline_register[1][3]);
          				     int i=0;
          				     geek >> i; 
          				     performoperation2(i);
          			return 0;
         		}

         		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         		//-----------------------------------I_TYPE-FORWARDING ENDS------------------------------------------------------------------------------------------//
         		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

          		if((pipeline_register[1][0]=="add")||(pipeline_register[1][0]=="sub")||(pipeline_register[1][0]=="mul"))
          		{	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			if(pipeline_register[1][4]=="type-R")
          			{
          					r2=pipeline_register[1][2][0]-48;
          					r3=pipeline_register[1][3][0]-48;
          					performoperation(0);          					
          			}
          			else if(pipeline_register[1][4]=="type-I")
          			{		
          				     r2=pipeline_register[1][2][0]-48;
          				     stringstream geek(pipeline_register[1][3]);
          				     int i=0;
          				     geek >> i;
          				     performoperation2(i);
           			}
          		}
          		else if((pipeline_register[1][0]=="mov")||(pipeline_register[1][0]=="cmp"))
          		{	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			if(pipeline_register[1][3]=="type-R")
          			{		string t=pipeline_register[1][1].substr(1);
          					r1=t[0]-48;
          					r2=pipeline_register[1][2][0]-48;
          					performoperation(0);
          			}
          			else if(pipeline_register[1][3]=="type-I")
          			{		 string t=pipeline_register[1][1].substr(1);
          					 r1=t[0]-48;
          					 stringstream geek(pipeline_register[1][2]);
          				     int i=0;
          				     geek >> i;
          				     performoperation2(i);	
          			}
          		}
          		else if((pipeline_register[1][0]=="ldr")||(pipeline_register[1][0]=="str"))
          		{	key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			if(pipeline_register[1][3]=="type-R")
          			{		
          					r2=pipeline_register[1][2][0]-48;
          					performoperation(0);
          			}
          			else if(pipeline_register[1][4]=="type-I")
          			{		
          					 stringstream geek(pipeline_register[1][3]);
          				     int i=0;
          				     geek >> i;
          				     r2=pipeline_register[1][2][0]-48;
          				     performoperation2(i);	
          			}
          		}
          		else if((pipeline_register[1][0]=="bl")||(pipeline_register[1][0]=="b")||(pipeline_register[1][0]=="ble")||(pipeline_register[1][0]=="bne")||(pipeline_register[1][0]=="bge"))
          		{	int x;
          			key=pipeline_register[1][0];
          			pipeline_register[2][0]=pipeline_register[1][0];
          			pipeline_register[2][1]=pipeline_register[1][1];
          			x=performoperation3();
          			if(x==1)
          			x=performoperation4();
          			pipeline_register[2][2]="";//cout<<x<<"asdfasdf"<<endl;
          			return x ; 
          		}
          		return 0;
          }
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------EXE_STAGE ENDS-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------MEM_STAGE----------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
          int enhanced_armsimulator::MEM_STAGE()
          {
         		if(pipeline_register[2][0]=="")
         		{
         			pipeline_register[3][0]="";
         			for(int i=1;i<pipeline_register[3].size();i++)
       		  		{	
         				pipeline_register[3][i]="";
         			}

         		return 0;	
         		}
         			
         		if(pipeline_register[2][0]=="end")
         		{	pipeline_register[3][0]="end";
         			for(int i=1;i<pipeline_register[3].size();i++)
       		  		{	
         				pipeline_register[3][i]="";
         			}
         			return 0;
         		}	
         		if(pipeline_register[2][0]=="label_detected")
         		{	pipeline_register[3][0]="label_detected";
         			for(int i=1;i<pipeline_register[3].size();i++)
       		  		{	
         				pipeline_register[3][i]="";
         			}
         			return 0;
         		}
         		if(pipeline_register[2][0]=="str")
         		{	int r;
         			 stringstream geek(pipeline_register[2][2]);
          				     int i=0;
          				     geek >> i;
          			 string t;
          			 t=pipeline_register[2][1].substr(1);
          			 r=t[0]-48;
         			 memory[i]=regs[r];
         			 if(str_key==1)
         			 {	stringstream ss(str_value);
          				     int k=0;
          				     ss >> k;
         			 	memory[i]=k;
         			 	str_key=0;
         			 }
         			 pipeline_register[3][0]="";
         			 pipeline_register[3][1]="";
         		}
         		else if(pipeline_register[2][0]=="ldr")
         		{
         			 stringstream geek(pipeline_register[2][2]);//cout<<pipeline_register[2][2]<<"sdf";
          				     int i=0;
          				     geek >> i;
          			 pipeline_register[3][1]=to_string(memory[i]);
          			 pipeline_register[3][0]=pipeline_register[2][1];
         		}
         		else if((pipeline_register[2][0]=="add")||(pipeline_register[2][0]=="sub")||(pipeline_register[2][0]=="mul")||(pipeline_register[2][0]=="mov"))
         		{
         			pipeline_register[3][0]=pipeline_register[2][1];
         			pipeline_register[3][1]=pipeline_register[2][2];
         		}
         		else
         		{
         			pipeline_register[3][0]="";
         			pipeline_register[3][1]="";
         		}
         		return 0;
          }
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------MEM_STAGE ENDS-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------WB_STAGE----------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
          string enhanced_armsimulator::WB_STAGE()
          {		int r;
          		if(pipeline_register[3][0]=="")
         		return "empty";
         		else if(pipeline_register[3][0]=="end")
         		{ return "empty";
         		}
         		else if(pipeline_register[3][0]=="label_detected")
         		{ return "empty";
         		}	
         		else
         		{	
         			 stringstream geek(pipeline_register[3][1]);
          				     int i=0;
          				     geek >> i;
         			 string t=pipeline_register[3][0].substr(1);
          					 r=t[0]-48;
          					 regs[r]=i;	
         		}
         		return pipeline_register[3][0];
          }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------WB_STAGE ENDS-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
 
//------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------SOME OUTPUT FUNCTIONS-----------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------//
		void enhanced_armsimulator::clock_number()
		{
			cout<<"CLOCK CYCLE COUNT :\t"<<clock<<endl;
		}
	 	void enhanced_armsimulator::output_IF()
	 	{
	 	     cout<<"\n(IF STAGE)\nIF/ID PIPELINE REGISTER :\t"<<pipeline_register[0][0]<<endl;
	 	}
	 	void enhanced_armsimulator::output_ID()
	 	{
	 		 cout<<"\n(ID STAGE)\nID/EXE PIPELINE REGISTER :\t";
	 		 for(int i=0;i<pipeline_register[1].size();i++)
	 		 {
	 		 	cout<<pipeline_register[1][i]<<"     ";
	 		 }	
	 		 cout<<endl;
	 	}
	 	void enhanced_armsimulator::output_EXE()
	 	{
	 		cout<<"\n(EXE STAGE)\nEXE/MEM PIPELINE REGISTER :\t";
	 		 for(int i=0;i<pipeline_register[1].size();i++)
	 		 {
	 		 	cout<<pipeline_register[2][i]<<"     ";
	 		 }	
	 		 cout<<endl;
	 	}
	 	void enhanced_armsimulator::output_MEM()
	 	{
	 		cout<<"\n(MEM STAGE)\nMEM/WB PIPELINE REGISTER :\t";
	 		 for(int i=0;i<pipeline_register[1].size();i++)
	 		 {
	 		 	cout<<pipeline_register[3][i]<<"     ";
	 		 }	
	 		 cout<<endl;
	 	}
	 	void enhanced_armsimulator::output_WB(string r)
	 	{	if(r!="empty")
	 		cout<<"\n(WRITE_BACK STAGE)\nWRITING IN REGISTER : "<<r<<endl;
	 		else
	 			cout<<"\n(WRITE_BACK STAGE)\n NO VALUE TO WRITE BACK TO THE REGISTER"<<endl;
	 	}
	 	void enhanced_armsimulator::output() // OUTPUT FUNCTION WHICH SHOWS REGISTER VALUE AFTER EACH INSTRUCTION
       {
        for(int i=0;i<16;i++){
          if(i==10)
          cout<<"R"<<i<<"(sl):"<<regs[i]<<endl;
          else if(i==11)
          cout<<"R"<<i<<"(fp):"<<regs[i]<<endl; 
          else if(i==12)
          cout<<"R"<<i<<"(ip):"<<regs[i]<<endl;
          else if(i==13)
          cout<<"R"<<i<<"(sp):"<<regs[i]<<endl;
          else if(i==14)
          cout<<"R"<<i<<"(lr):"<<regs[i]<<endl;
          else if(i==15)
          cout<<"R"<<i<<"(pc):"<<regs[i]<<endl;
          else  
          cout<<"R"<<i<<"     :"<<regs[i]<<endl;
        }
        cout<<"--------------------------"<<endl;
       }
//------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------END OF OUTPUT FUNCTIONS---------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------FUNCTIONS FOR PERFORMING ARTHEMATIC OPERATIONS-----------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------//

// THIS FUNCTION PERFORMS ARTHEMATIC OPERATIONS BETWEEN REGISTERS ONLY
        void enhanced_armsimulator::performoperation(int i){
          stringstream geek(pipeline_register[2][2]);
          				     int t=0;
          				     geek >> t;
          stringstream ss(pipeline_register[3][1]);
          				     int p=0;
          				     ss >> p;
          if((key=="mov")||(key=="MOV")||(key=="Mov"))
          {
              if(i==-6)
              {
              	pipeline_register[2][2]=pipeline_register[2][2];
              }
              else if(i==-7)
               {
               	 pipeline_register[2][2]=pipeline_register[3][1];	
               }
              else
              	pipeline_register[2][2]=to_string(regs[r2]);
          }
          else  if((key=="cmp")||(key=="CMP")||(key=="Cmp"))
          {	  if(i==-6)
          	  {
          	  	if(regs[r1]==t)
                {
                carry=1;
                }
                else if(regs[r1]>t)
                {
                	carry=2;
                }
                else if(regs[r1]<t)
                {
                	carry=3;
                }
          	  }
          	  else if(i==-8)
          	  {
          	  	if(p==t)
                {
                carry=1;
                }
                else if(p>t)
                {
                	carry=2;
                }
                else if(p<t)
                {
                	carry=3;
                }
          	  }
          	  else if(i==-9)
          	  {
                carry=1;
                
          	  }
          	  else if(i==-7)
          	  {
          	  	if(regs[r1]==p)
                {
                carry=1;
                }
                else if(regs[r1]>p)
                {
                	carry=2;
                }
                else if(regs[r1]<p)
                {
                	carry=3;
                }	
          	  }
          	 else if(i==-10)
          	  {  	  	
                
                carry=1;
                               	
          	  }
          	   else if(i==-11)
          	  {
          	  	if(t==p)
                {
                carry=1;
                }
                else if(t>p)
                {
                	carry=2;
                }
                else if(t<p)
                {
                	carry=3;
                }	
          	  }
          	 else if(i==-14)
          	  {
          	  	if(t==regs[r2])
                {
                carry=1;
                }
                else if(t>regs[r2])
                {
                	carry=2;
                }
                else if(t<regs[r2])
                {
                	carry=3;
                }
          	  }
          	  else if(i==-12)
          	  {
          	  	if(t==p)
                {
                carry=1;
                }
                else if(t>p)
                {
                	carry=2;
                }
                else if(t<p)
                {
                	carry=3;
                }
          	  }
          	  else if(i==-13)
          	  {
                carry=1;
                
          	  }
          	  else if(i==-17)
          	  {
          	  	if(p==regs[r2])
                {
                carry=1;
                }
                else if(p>regs[r2])
                {
                	carry=2;
                }
                else if(p<regs[r2])
                {
                	carry=3;
                }	
          	  }
          	 else if(i==-15)
          	  {  	  	
                
                carry=1;
                               	
          	  }
          	   else if(i==-16)
          	  {
          	  	if(p==t)
                {
                carry=1;
                }
                else if(p>t)
                {
                	carry=2;
                }
                else if(p<t)
                {
                	carry=3;
                }	
          	  }
          	  else{
              if(regs[r1]==regs[r2])
                {
                carry=1;
                }
                else if(regs[r1]>regs[r2])
                {
                	carry=2;
                }
                else if(regs[r1]<regs[r2])
                {
                	carry=3;
                }
            	}

          }
          else  if((key=="add")||(key=="ADD")||(key=="Add"))
          {	   if(i==-6)
          		{    
          			pipeline_register[2][2]=to_string(t+regs[r3]);
          		}
          		else if(i==-8)
          		{    
          			pipeline_register[2][2]=to_string(t+p);
          		}
          		else if(i==-9)
          		{    
          			pipeline_register[2][2]=to_string(t+t);
          		}
          		else if(i==-7)
          		{    
          			pipeline_register[2][2]=to_string(p+regs[r3]);
          		}
          		else if(i==-10)
          		{    
          			pipeline_register[2][2]=to_string(p+p);
          		}
          		else if(i==-11)
          		{    
          			pipeline_register[2][2]=to_string(p+t);cout<<"asdfasdfsdf";
          		}
          		else if(i==-14)
          		{    
          			pipeline_register[2][2]=to_string(regs[r2]+t);
          		}
          		else if(i==-12)
          		{    
          			pipeline_register[2][2]=to_string(p+t);
          		}
          		else if(i==-13)
          		{    
          			pipeline_register[2][2]=to_string(t+t);
          		}
          		else if(i==-17)
          		{    
          			pipeline_register[2][2]=to_string(regs[r2]+p);
          		}
          		else if(i==-15)
          		{    
          			pipeline_register[2][2]=to_string(p+p);
          		}
          		else if(i==-16)
          		{    
          			pipeline_register[2][2]=to_string(t+p);
          		}
          	  else{
          	  	pipeline_register[2][2]=to_string(regs[r2]+regs[r3]);	
          	  }
              
          }
          else   if((key=="sub")||(key=="Sub")||(key=="SUB"))
          {		
                if(i==-6)
          		{    
          			pipeline_register[2][2]=to_string(t-regs[r3]);
          		}
          		else if(i==-8)
          		{    
          			pipeline_register[2][2]=to_string(t-p);
          		}
          		else if(i==-9)
          		{    
          			pipeline_register[2][2]=to_string(t-t);
          		}
          		else if(i==-7)
          		{    
          			pipeline_register[2][2]=to_string(p-regs[r3]);
          		}
          		else if(i==-10)
          		{    
          			pipeline_register[2][2]=to_string(p-p);
          		}
          		else if(i==-11)
          		{    
          			pipeline_register[2][2]=to_string(p-t);
          		}
          		else if(i==-14)
          		{    
          			pipeline_register[2][2]=to_string(regs[r2]-t);
          		}
          		else if(i==-12)
          		{    
          			pipeline_register[2][2]=to_string(p-t);
          		}
          		else if(i==-13)
          		{    
          			pipeline_register[2][2]=to_string(t-t);
          		}
          		else if(i==-17)
          		{    
          			pipeline_register[2][2]=to_string(regs[r2]-p);
          		}
          		else if(i==-15)
          		{    
          			pipeline_register[2][2]=to_string(p-p);
          		}
          		else if(i==-16)
          		{    
          			pipeline_register[2][2]=to_string(t-p);
          		}
          	  else{
          	  	pipeline_register[2][2]=to_string(regs[r2]-regs[r3]);	
          	  }
          }
          else  if((key=="mul")||(key=="mul")||(key=="mul"))
          {
                if(i==-6)
          		{    
          			pipeline_register[2][2]=to_string(t*regs[r3]);
          		}
          		else if(i==-8)
          		{    
          			pipeline_register[2][2]=to_string(t*p);
          		}
          		else if(i==-9)
          		{    
          			pipeline_register[2][2]=to_string(t*t);
          		}
          		else if(i==-7)
          		{    
          			pipeline_register[2][2]=to_string(p*regs[r3]);
          		}
          		else if(i==-10)
          		{    
          			pipeline_register[2][2]=to_string(p*p);
          		}
          		else if(i==-11)
          		{    
          			pipeline_register[2][2]=to_string(p*t);
          		}
          		else if(i==-14)
          		{    
          			pipeline_register[2][2]=to_string(regs[r2]*t);
          		}
          		else if(i==-12)
          		{    
          			pipeline_register[2][2]=to_string(p*t);
          		}
          		else if(i==-13)
          		{    
          			pipeline_register[2][2]=to_string(t*t);
          		}
          		else if(i==-17)
          		{    
          			pipeline_register[2][2]=to_string(regs[r2]*p);
          		}
          		else if(i==-15)
          		{    
          			pipeline_register[2][2]=to_string(p*p);
          		}
          		else if(i==-16)
          		{    
          			pipeline_register[2][2]=to_string(t*p);
          		}
          	  else{
          	  	pipeline_register[2][2]=to_string(regs[r2]*regs[r3]);	
          	  }
          }
            else  if((key=="ldr")||(key=="Ldr")||(key=="LDR"))
          {	if(i==-6)
          	{
          		pipeline_register[2][2]=to_string(t/4);
          	} 
          	else if(i==-7)
          	{
          		pipeline_register[2][2]=to_string(p/4);
          	}
           else
          	{
              pipeline_register[2][2]=to_string((regs[r2])/4);
          	}
          }
           else  if((key=="str")||(key=="Str")||(key=="STR"))
          {
           	  pipeline_register[2][2]=to_string((regs[r2])/4);   
          }
         // output(); // FINALLY CALLS THE OUTPUT TO SHOW REGISTER VALUES
        }


// THIS FUNCTION PERFORMS ARTHEMATIC OPERATIONS BETWEEN REGISTERS AND A CONSTANT
        void enhanced_armsimulator::performoperation2(int i){
          stringstream geek(pipeline_register[2][2]);
          				     int t=0;
          				     geek >> t;
          stringstream ss(pipeline_register[3][1]);
          				     int p=0;
          				     ss >> p;

           if((key=="mov")||(key=="MOV")||(key=="Mov"))
          {
               pipeline_register[2][2]=to_string(i);
          }
          else  if((key=="cmp")||(key=="CMP")||(key=="Cmp"))
          {	  if(num==-6)
          		{	//cout<<"reached"<<t<<endl;
          			if(t==i)
                	{
                		carry=1;//cout<<"rahulsdsdf";
                	}
                	else if(t>i)
                	{
                		carry=2;
                	}
                	else if(t<i)
                	{
                		carry=3;
                	}
                	num=0;
          		}
          		else if(num==-7)
          		{	
          			if(p==i)
                	{
                		carry=1;
                	}
                	else if(p>i)
                	{
                		carry=2;
                	}
                	else if(p<i)
                	{
                		carry=3;
                	}
                	num=0;
          		}
          		else{
             	 	if(regs[r1]==i)
                	{
                		carry=1;
                	}
                	else if(regs[r1]>i)
                	{
                		carry=2;
                	}
                	else if(regs[r1]<i)
               	    {
                		carry=3;
                	}

            	}


          }
          else  if((key=="add")||(key=="ADD")||(key=="Add"))
          {	
          	if(num==-7)
          	{
          		pipeline_register[2][2]=to_string(p+i);
          		num=0;
          	}
          	else if(num==-6)
         	 {
         	 	pipeline_register[2][2]=to_string(t+i);
         	 	num=0;
          	} 
          	else
          	{
              pipeline_register[2][2]=to_string(regs[r2]+i);
          	}
          }
          else   if((key=="sub")||(key=="Sub")||(key=="SUB"))
          {
           if(num==-7)
          	{
          		pipeline_register[2][2]=to_string(p-i);
          		num=0;
          	}
          	else if(num==-6)
         	 {
         	 	pipeline_register[2][2]=to_string(t-i);
         	 	num=0;
          	} 
          	else
          	{
              pipeline_register[2][2]=to_string(regs[r2]-i);
          	}
          }
          else  if((key=="mul")||(key=="mul")||(key=="mul"))
          {
           if(num==-7)
          	{
          		pipeline_register[2][2]=to_string(p*i);
          		num=0;
          	}
          	else if(num==-6)
         	 {
         	 	pipeline_register[2][2]=to_string(t*i);
         	 	num=0;
          	} 
          	else
          	{
              pipeline_register[2][2]=to_string(regs[r2]*i);
          	}
          }
          else  if((key=="ldr")||(key=="Ldr")||(key=="LDR"))
          {	
          	if(num==-7)
          	{
          		pipeline_register[2][2]=to_string((p+i)/4);
          		num=0;
          	}
          	else if(num==-6)
          	{
          		pipeline_register[2][2]=to_string((t+i)/4);
          		num=0;
          	}
          	else{
             		 pipeline_register[2][2]=to_string((regs[r2]+i)/4);
          		}
          }
           else  if((key=="str")||(key=="Str")||(key=="STR"))
          {
             pipeline_register[2][2]=to_string((regs[r2]+i)/4); 
          }
          //output(); // FINALLY CALLS THE OUTPUT
        }    

        // THIS FUNCTION PERFORM BRANCH OPERATIONS WHENEVER CALLED UPON WITHOUT ANY CONSTANT COMPARISON I.E. COMPARISIONS MUST 
		// BE BETWEEN REGISTERS
        int enhanced_armsimulator::performoperation3()
        { 
          if((key=="bne")||(key=="BNE")||(key=="Bne")||(key=="bge")||(key=="BGE")||(key=="Bge")||(key=="ble")||(key=="BLE")||(key=="Ble")){
            string t=words.substr(3);
            if((key=="bne")||(key=="BNE")||(key=="Bne")){
              if((carry==2)||(carry==3))
              {		carry=0;
                  for(int i=0;i<label.size();i++)
                  { 
                     if(t==label[i])
                     {int temp=index[i];
                      regs[15]=4096+(4*temp);
                      return -137;
                     } 
                  } 
              }
          }
          if((key=="ble")||(key=="BLE")||(key=="Ble")){
              if((carry==1)||(carry==3))
              {		carry=0;
                  for(int i=0;i<label.size();i++)
                  {
                     if(t==label[i])
                     {int temp=index[i];
                      regs[15]=4096+(4*temp);
                      return -137;
                     } 
                  } 
              }             
          }
          if((key=="bge")||(key=="BGE")||(key=="Bge")){
              if((carry==1)||(carry==2))
              {	carry=0;
                  for(int i=0;i<label.size();i++)
                  {
                     if(t==label[i])
                     {int temp=index[i];
                      regs[15]=4096+(4*temp);
                      return -137;
                     } 
                  } 
              }             
          }
          return 1;
        }
          else if((key=="bl")||(key=="Bl")||(key=="Bl")){
            string t=words.substr(2);
            for(int i=0;i<label.size();i++)
            {   
               if(t==label[i])
               {int temp=index[i];
                regs[14]=regs[15]+4;
                regs[15]=4096+(4*temp);
                return -137;
               } 
            }
            return 1;
            
          }
          else {

            string t=words.substr(1);
            for(int i=0;i<label.size();i++)
            {
               if(t==label[i])
               {int temp=index[i];
                regs[15]=4096+(4*temp);
                return -137;
               } 
            }
            return 1; 
          
          }        

        }   


        // THIS FUNCTION PERFORM BRANCH OPERATIONS WHENEVER CALLED UPON WITH CONSTANT COMPARISON I.E. COMPARISIONS MUST 
		// BE BETWEEN REGISTER AND A CONSTANT BEFORE BRANCH STATEMENT
        int enhanced_armsimulator::performoperation4()
        { 
          if((key=="bne")||(key=="BNE")||(key=="Bne")||(key=="bge")||(key=="BGE")||(key=="Bge")||(key=="ble")||(key=="BLE")||(key=="Ble")){
            string t=words.substr(3);
            if((key=="bne")||(key=="BNE")||(key=="Bne")){
              if((carry==2)||(carry==3))
              {    carry=0;cout<<"bosss";
                  for(int i=0;i<label.size();i++)
                  { 
                     if(t==label[i])
                     {int temp=index[i];
                      regs[15]=4096+(4*temp);
                      return -137;
                     } 
                  } 
              }
          }
          if((key=="ble")||(key=="BLE")||(key=="Ble")){
              if((carry==1)||(carry==3))
              {   carry=0;
                  for(int i=0;i<label.size();i++)
                  {
                     if(t==label[i])
                     {int temp=index[i];
                      regs[15]=4096+(4*temp);
                      return -137;
                     } 
                  } 
              }             
          }
          if((key=="bge")||(key=="BGE")||(key=="Bge")){
              if((carry==1)||(carry==2))
              {		carry=0;
                  for(int i=0;i<label.size();i++)
                  {
                     if(t==label[i])
                     {int temp=index[i];
                      regs[15]=4096+(4*temp);
                      return -137;
                     } 
                  } 

              }             
          }
          return 1;
        }
          else if((key=="bl")||(key=="Bl")||(key=="Bl")){
            string t=words.substr(2);
            for(int i=0;i<label.size();i++)
            {   
               if(t==label[i])
               {int temp=index[i];
                regs[14]=regs[15]+4;
                regs[15]=4096+(4*temp);
                return -137;
               } 
            }
            return 1;
            
          }
          else {

            string t=words.substr(1);
            for(int i=0;i<label.size();i++)
            {
               if(t==label[i])
               {int temp=index[i];
                regs[15]=4096+(4*temp);
                return -137;
               } 
            }
            return 1; 
          
          }        

        }

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------FUNCTIONS FOR PERFORMING ARTHEMATIC OPERATIONS ENDS HERE-----------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------------------------------------------//

//memory function to show the free memory contents
 void enhanced_armsimulator::memoryadd()
       {  cout<<"FOLLOWING IS THE FREE MEMORY STATUS AFTER EXECUTING THE PROGRAM "<<endl;
        for(int i =0;i<4000;i++)
        { 
          cout<< memory[i]<<" ";
         }
       }

 // THIS FUNCTION IS CALLED AT THE END TO SHOW THE TOTAL NO. OF INSTRUCTION COUNT FROM MAIN.
       int enhanced_armsimulator::instructioncount()
       {
        return instruction_count;
       }

 // LATENCY CHECKING AND UPDATING THE CLOCK COUNT
       int enhanced_armsimulator::latency_check()
       { int clock1=0,clock2=0;
        key=pipeline_register[1][0];
       if((key=="bl")||(key=="BL")||(key=="Bl"))
          clock1+=latency[11];
       else if((key=="add")||(key=="ADD")||(key=="Add"))
          clock1+=latency[4];
      else if((key=="sub")||(key=="SUB")||(key=="Sub"))
          clock1+=latency[5];
      else if((key=="mul")||(key=="MUL")||(key=="Mul"))
          clock1+=latency[7];
      else if((key=="cmp")||(key=="CMP")||(key=="Cmp"))
          clock1+=latency[6];
      else if((key=="bne")||(key=="Bne")||(key=="BNE"))
          clock1+=latency[8];
      else if((key=="bge")||(key=="Bge")||(key=="BGE"))
          clock1+=latency[9];
      else if((key=="mov")||(key=="Mov")||(key=="MOV"))
          clock1+=latency[12];
      else if((key=="b")||(key=="B"))
          clock1+=latency[10];
    else 
      clock1+=0;
    key=pipeline_register[2][0];
    if((key=="ldr")||(key=="LDR")||(key=="Ldr"))
    { 
        clock2+=latency[0];  
    }
    else if((key=="str")||(key=="STR")||(key=="Str"))
    {
        clock2+=latency[2];      
    }
    else 
      clock2+=0;

    if(clock1>clock2)
      clock+=clock1-1;
    else if(clock1<clock2)
      clock+=clock2-1;
    else if((clock1==clock2)&&(clock1!=0))
      clock=clock1-1;
  	return 0;
       }