// Jacob Martin
// spell_check.cc: A simple spell checker.
#include <fstream>
#include <iostream>
#include <string>

// You can change to quadratic probing if you
// haven't implemented double hashing.
//#include "linear_probing.h"
#include "double_hashing.h"
//#include "quadratic_probing.h"
using namespace std;

// You can add more functions here.
//Adds a-z at each index in the word
void caseA(string &word, const HashTableDouble<string>& dictionary)
{
  string copyword_ = word;
  for(size_t  i = 0; i <= word.size(); i++)//inserts a-z in each index of string
  {//x<=word.size() because a-z is added at the end of word_Copy
    for(char x = 'a'; x <= 'z'; x++)
    {
      copyword_ = word;
      copyword_.insert(i,1,x);
      if(dictionary.Contains(copyword_))
        cout << "** " << word << " -> " << copyword_ << " ** case A" <<endl;
    }
  }
}

//Removes a letter from word one by one and checks if it is in the dictionary 
void caseB(string &word, const HashTableDouble<string>& dictionary)
{
  for(size_t  i = 0; i < word.size(); i++)
  {
    string copyword_ = word;
    if(dictionary.Contains( copyword_.erase(i,1)))
      cout << "** " << word << " -> " << copyword_ << " ** case B" <<endl;
  }
}

//swaps all adjacent letters and checks if it is in the dictionary 
void caseC(string &word, const HashTableDouble<string>& dictionary)
{
  string copyword_ = word;
  for(size_t  i = 0; i < word.size()-1; i++)
  {
    copyword_ = word;
    swap(copyword_[i],copyword_[i+1]);
    if(dictionary.Contains(copyword_))
      cout << "** " << word << " -> " << copyword_ << " ** case C" <<endl;
  }
}

// Creates and fills double hashing hash table with all words from dictionary_file
HashTableDouble<string> MakeDictionary(const string &dictionary_file) {
  HashTableDouble<string> dictionary_hash;
  //dictionary_hash.setRValue(7);
  // Fill dictionary_hash.
  string word = ""; //string for collecting words from dictionary
  fstream inputfile_(dictionary_file);//opens words.txt
  if(inputfile_.fail())
    cout << "Error" << endl;
  while(inputfile_ >> word)
    dictionary_hash.Insert(word);

  inputfile_.close();
  return dictionary_hash;
}

// For each word in the document_file, it checks the 3 cases for a word being
// misspelled and prints out possible corrections
void SpellChecker(const HashTableDouble<string>& dictionary, const string &document_file) 
{      
  string word = ""; //string for collecting words from dictionary
  fstream inputfile_(document_file);//opens words.txt
  if(inputfile_.fail())
    cout << "Error" << endl;
  else
  {
    while(inputfile_ >> word)//reads word from document1.txt ingoring spaces
    {
      transform(word.begin(), word.end(), word.begin(), ::tolower); //converts word to lowercase

      //removes punctuation at begining
      int x = 0;
      while(!(isalnum(word[x])))//checks if character is a-z if not then
        word.erase(0,1);
      
      //removes punctuation at end
      x = word.size()-1;
      while(!(isalnum(word[x])))
      {
        word.erase(word.size()-1,1);
        x = word.size()-1;
      }

      if(dictionary.Contains(word))
      {
        cout << word << " is CORRECT" << endl;
      }
      else
      {
        cout << word << " is INCORRECT" <<endl;
        //Run Case A
        caseA(word, dictionary);
        //Run Case B
        caseB(word, dictionary);
        //Run Case C
        caseC(word, dictionary);
      }
    }  
  }
 
  inputfile_.close();
}

// @argument_count: same as argc in main
// @argument_list: save as argv in main.
// Implements
int testSpellingWrapper(int argument_count, char** argument_list) 
{
  const string document_filename(argument_list[1]);
  const string dictionary_filename(argument_list[2]);

  // Call functions implementing the assignment requirements.
  HashTableDouble<string> dictionary = MakeDictionary(dictionary_filename);
  SpellChecker(dictionary, document_filename);

  return 0;
}

// Sample main for program spell_check.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testSpellingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char** argv) 
{
  if (argc != 3) 
  {
    cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>" << endl;
    return 0;
  }
  
  testSpellingWrapper(argc, argv);
  
  return 0;
}
