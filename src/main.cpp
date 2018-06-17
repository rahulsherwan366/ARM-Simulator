//1. ASSUMING THAT THE ARM CODE WILL NOT HAVE ANY COMMENT AND ALSO NOT HAVE ANY MULTIPLE INSTRUCTION IN THE SAME LINE
//2. THERE SHOULD NOT BE ANY TAB IN THE PROGRAM OTHERWISE IT WILL SHOW AN ERROR.
// HOWEVER YOU CAN HAVE ANY NUMBER OF SPACES IN BETWEEN .
//3. THERE SHOULDN'T BE ANY EMPTY LINE IN BETWEEN


//HEADERS REQUIRED 
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>

//HEADERS INCLUDED
#include "enhanced_armsimulator.h"
using namespace std;


// THE MAIN FUNCTION
int main ()
{ fstream ifile,cfile; // FILE INPUT /OUTPUT MODE 
   enhanced_armsimulator a; // OBJECT a OF armsimulator TYPE 
  int i=0,flag;    
  string temp;
  vector<string> s; //FOR STORING INSTRUCTIONS
   vector<string> label;  // LABEL VECTOR WHICH TAKES THE LABELS FROM THE ARM FILE
   vector<int> index;     // INDEXING WITH THE LINE NO. WILL BE DONE BY INDEX VECTOR. 
  ifile.open("sample.txt",ios::in|ios::out); // FILE NAME MUST BE "sample.txt"
  while(!ifile.eof()){    // WHILE END OF FILE IS NOT REACHED
    getline(ifile,temp);  // GETTING A LINE FROM FILE
    s.push_back(temp); // PUSHING IT TO THE S VECTOR 
    flag=0;               
     
    int pos=0;            
    bool found=false;              
    for(int r=0;r<temp.size();r++){ // CHECKING WHETHER A LABEL IS FOUND OR NOT
      if(temp[r]==':')
        found=true;
    }
    pos=temp.find(":");
    if(found==true)   // IF FOUND THEN TAKE THE RIGHT PART OF IT
    {
    string k,t=temp.substr(0,pos); 
     for(int p=0;p<t.length();p++){         
           if(t[p]==' ')
           continue;
           else
           k+=t[p];         
           } 
    label.push_back(k);  //PUSH THE FOUND LABEL INTO LABEL VECTOR
    index.push_back(i);  // PUSH THE LINE NO. AT WHICH IT IS FOUND INTO INDEX VECTOR.
    }
    i++;
    }
    ifile.close();
     int latency[13];
    for(int h=0;h<13;h++)
    {
      latency[h] = 0;
    }
    cfile.open("latency.txt",ios::in);
    while(!cfile.eof())
    { 
      string k,right,left,ints;
      int w,value=0;
      getline(cfile,temp);
       for(int p=0;p<temp.length();p++){         
           if(temp[p]==' ')
           continue;
           else
           k+=temp[p];         
           } 
        w=k.find("=");
        right=k.substr(0,w);
        left=k.substr(w+1);
        for(int l=0;left[l]!=';';l++)
        {
          ints+=left[l];
        }
        stringstream geek(ints);
        geek >> value;
      //  cout<<value<<endl;
        if(right=="ldr")
          latency[0]=value;
        else if(right=="ldr_pseudo")
          latency[1]=value;
        else if(right=="str")
          latency[2]=value;
        else if(right=="str_pseudo")
          latency[3]=value;
        else if(right=="add")
          latency[4]=value;
        else if(right=="sub")
          latency[5]=value;
        else if(right=="cmp")
          latency[6]=value;
        else if(right=="mul")
          latency[7]=value;
        else if(right=="bne")
          latency[8]=value;
        else if(right=="bge")
          latency[9]=value;
        else if(right=="b")
          latency[10]=value;
        else if(right=="bl")
          latency[11]=value;
        else if(right=="mov")
          latency[12]=value;
        else 
            continue;
    }    
    cfile.close();
    int tr;
    a.getdata(label,index,s,latency); //CALLING GETDATA FUNCTION FOR PROVIDING LABELS AND INSTRUCTIONS.
    tr=a.instruction_section();// CALLING INSTRUCTION_SECTION FOR PROCESSING INSTRUCTION.
    return 0;
  }