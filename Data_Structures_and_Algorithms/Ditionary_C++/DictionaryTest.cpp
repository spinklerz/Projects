/****************************************************************************************
 *  DictionaryTest.cpp
 *  
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa7
 *****************************************************************************************/
#include<iostream>
#include<string>
#include<stdexcept>
#include"Dictionary.h"

using namespace std;

int main(){

   string s;
   int x;
   string S[] =   {
                     "plaices",
                     "ambusher",
                     "crosby",
                     "wattles",
                     "pardoner",
                     "pythons",
                     "second",
                     "forms",
                     "impales",
                     "pic",
                     "verticals",
                     "recriminator",
                     "depressants",
                     "cul",
                     "potables",
                     "norm",
                     "reinsurer",
                     "deschooler",
                     "recoining",
                     "dissocialised",
                     "cohabit",
                     "hemiolia",
                     "cantling",
                     "glamorized",
                     "millesimal",
                     "glagolitic"
                  };
   string T[] =   {
                     "second",
                     "forms",
                     "impales",
                     "pic",
                     "verticals",
                     "recriminator",
                     "depressants",
                     "cul",
                     "potables",
                     "norm",
                     "reinsurer",
                     "deschooler",
                     "recoining",      
                  };
                  

   Dictionary A;
   Dictionary B;
   /*
    A.setValue("c", 3);
    A.setValue("b", 2);
    A.setValue("a", 1);
    A.begin();
    A.clear();
    if (A.size() != 0) {
      return 1;
    }
    try {
      A.currentKey();
      return 2;
    } catch (logic_error const &) {
    }
    return 0;

    return 0;
    */   
    /* 
      A.clear();
    B.clear();
    if (!(A == B))
      return 1;

    A.setValue("a", 1);
    A.setValue("b", 5);
    A.setValue("e", 10);
    A.setValue("h", 15);
    A.setValue("f", 20);
    A.setValue("i", 100);

    if ((A == B))
      return 2;

    B.setValue("a", 1);
    B.setValue("b", 5);
    B.setValue("e", 10);
    B.setValue("h", 15);
    B.setValue("f", 20);
    B.setValue("i", 100);

    if (!(A == B))
      return 3;

    B.clear();
    B.setValue("i", 100);
    B.setValue("f", 20);
    B.setValue("h", 15);
    B.setValue("e", 10);
    B.setValue("b", 5);
    B.setValue("a", 1);
	cout << B << endl;
	cout << A << endl; 
    if (!(A == B))
     cout << 4;
*/
    A.setValue("g", 1);
    A.setValue("f", 2);
    A.setValue("d", 3);
    A.setValue("c", 4);
    A.setValue("e", 5);
    A.setValue("b", 6);
    A.setValue("a", 7);
    /*
    A.begin();
    A.remove("a");
    if (A.hasCurrent())
      cout << 1;
    A.end();
    A.remove("d");
    if (A.currentKey() != "g" || A.currentVal() != 30)
      cout << 2;
    A.remove("g");
    if (A.hasCurrent())
      cout << 3;
    cout << A << endl ;
    cout << B << endl;
	/*
   cout << endl;
   */
/*
   // insert some pairs into A
   for(int i=0; i<26; i++){
      A.setValue(S[i], i+1);
   }
   
   // call operator=()
   B = A;

   cout << "A.size() = " << A.size() << endl  << A << endl;
   cout << "B.size() = " << B.size() << endl  << B << endl;

   B.setValue("deschooler", 101);
   B.setValue("reinsurer", 102);
   B.setValue("glagolitic", 103);
   B.setValue("cul", 104);
   B.setValue("ambusher", 105);

   // call copy constructor
   Dictionary C = B;

   cout << "B.size() = " << B.size() << endl  << B << endl;
   cout << "C.size() = " << C.size() << endl  << C << endl;

   // check operator==()
   cout << "A==B is " << (A==B?"true":"false") << endl;
   cout << "B==C is " << (B==C?"true":"false") << endl;
   cout << "C==A is " << (C==A?"true":"false") << endl << endl;

   // perform alterations on A
   cout << A.getValue("hemiolia") << endl;
   A.getValue("hemiolia") *= 10; // change the value associated with "hemiolia"
   cout << A.getValue("hemiolia") << endl << endl;

   // check state of A
   cout << "A.size() = " << A.size() << endl  << A << endl;
   cout << A.pre_string() << endl;

   // remove some pairs from A
   for(int i=0; i<13; i++){
      cout << "removing " << T[i] << " from Dictionary A" << endl; 
      A.remove(T[i]);
   }

   cout << "after remove loop" << endl;

   // check state of A
   cout << "A.size() = " << A.size() << endl  << A << endl;
   cout << A.pre_string() << endl;

   // do forward iteration on A
   for(A.begin(); A.hasCurrent(); A.next()){
      s = A.currentKey();
      x = A.currentVal();
      cout << "("+s+", " << x << ") ";
   }
   cout << endl << endl;

   // do reverse iteration on A
   for(A.end(); A.hasCurrent(); A.prev()){
      s = A.currentKey();
      x = A.currentVal();
      cout << "("+s+", " << x << ") ";
   }
   cout << endl << endl;
   
   // check exceptions
   try{
      A.getValue("blah");
   }catch(logic_error& e){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }
   try{
      A.remove("blah");
   }catch(logic_error& e){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }
   try{
      A.currentKey();
   }catch(logic_error& e){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }
   try{
      A.currentVal();
   }catch(logic_error& e){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }  
   try{
      A.next();
   }catch(logic_error& e){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }
   try{
      A.prev();
   }catch(logic_error& e){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }

   cout << endl;  
	*/
   return( EXIT_SUCCESS );
}
