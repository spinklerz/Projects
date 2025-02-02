/****************************************************************************************
 *  Order.cpp
 *  
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa8
 *****************************************************************************************/

#include <string>
#include "Dictionary.h"
#include <iostream>
#include <fstream>

using namespace std; 

int main(int argc, char *argv[] ){
	// check command line for correct number of arguments
	if( argc != 3 ){
		cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
	}
	ifstream in;
   	ofstream out;
	// open files for reading and writing 
	in.open(argv[1]);
	if( !in.is_open() ){
		cerr << "Unable to open file " << argv[1] << " for reading" << endl;
	}
	out.open(argv[2]);
	if( !out.is_open() ){
		cerr << "Unable to open file " << argv[2] << " for writing" << endl;
		return(EXIT_FAILURE);
	}
	std::string line; 
	Dictionary A; 
	for(int i = 1; getline(in, line); i++ ){
		A.setValue(line,i);
	}
	out << A.to_string() << endl; 
	out << A.pre_string() << endl; 
	in.close();
   	out.close();

   return(EXIT_SUCCESS);

	
}
