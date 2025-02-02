/****************************************************************************************
 *  Order.cpp
 *  
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa8
 *****************************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include "Dictionary.h"

using namespace std;

#define MAX_LEN 300
std::string to_lower(std::string&s){
	string str = ""; 
	for( int i = 0; i < s.length(); i++){
		if( s[i] >= 'A' && s[i] <= 'Z' ){
			str+= s[i] + 32; 
		}
		else{
			str += s[i]; 
		}
	}
	return str; 
}


int main(int argc, char * argv[]){

   int token_count, line_count;
   int x; 
   size_t begin, end, len;
   ifstream in;
   ofstream out;
   string line;
   string tokenBuffer;
   string token;
   string delim = " \t\\\"\',<.>/?;:[{]}|`~\231!\-\“@#$%^&*()-_=+0123456789";
   Dictionary A;
   // open files for reading and writing 
   in.open(argv[1]);
   if( !in.is_open() ){
      cerr << "Unable to open file " << argv[1] << " for reading" << endl;
      return(EXIT_FAILURE);
   }
   out.open(argv[2]);
   if( !out.is_open() ){
      cerr << "Unable to open file " << argv[2] << " for writing" << endl;
      return(EXIT_FAILURE);
   }
   // read each line of input file, then count and print tokens 
   line_count = 0;
   while( getline(in, line) )  {
      line_count++;
      len = line.length();
      
      // get tokens in this line
      token_count = 0;
      tokenBuffer = "";

      // get first token
      begin = min(line.find_first_not_of(delim, 0), len);
      end   = min(line.find_first_of(delim, begin), len);
      token = line.substr(begin, end-begin);
      begin = min(line.find_first_not_of(delim, 0), len);
      end   = min(line.find_first_of(delim, begin), len);
      token = line.substr(begin, end-begin);
      while( token!="" ){
         tokenBuffer += "   "+token+"\n";
         token_count++;
         token = to_lower(token); 
	 if( A.contains(token) && token != delim ){
         	A.getValue(token)++;
         }
         else{ A.setValue(token, 1); }
         // get next token
         begin = min(line.find_first_not_of(delim, end+1), len);
         end   = min(line.find_first_of(delim, begin), len);
         token = line.substr(begin, end-begin);
      }

   }
   out << A << endl;
   // close files 
   in.close();
   out.close();

   return(EXIT_SUCCESS);
}



