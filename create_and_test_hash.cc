// Jacob Martin
#include <fstream>
#include <iostream>
#include <string>

// Uncomment when you implemented linear probing & double hashing
#include "linear_probing.h"
#include "double_hashing.h"
#include "quadratic_probing.h"

using namespace std;


// You can add more functions here

// @hash_table: a hash table (can be linear, quadratic, or double)
// @words_filename: a filename of input words to construct the hash table
// @query_filename: a filename of input words to test the hash table
template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table,
			      const string &words_filename,
			      const string &query_filename) {
    hash_table.MakeEmpty();

    string word; //string for collecting words from dictionary
    fstream inputfile_(words_filename);//opens words.txt
    if(inputfile_.fail())
        cout << "Error" << endl; //print Error if file cannot be opened
    else
        while(getline(inputfile_, word))//reads word from words.txt file
            hash_table.Insert(word);

    //print statements
    cout<< "number_of_elements: " << hash_table.getCurrentSize() << endl;
    cout<< "size_of_table: " << hash_table.getSize() << endl;
    cout<< "load_factor: " << (double) hash_table.getCurrentSize() / hash_table.getSize() << endl;
    cout<< "collisions: " << hash_table.getCollisions() << endl;
    cout<< "avg_collisions: " << (double) hash_table.getCollisions() / hash_table.getCurrentSize() << endl << endl;
    inputfile_.close();  //closes file

    string word2; //string for collecting words from dictionary
    fstream inputfile2_(query_filename);//opens words.txt
    if(inputfile2_.fail())
        cout<<"Error"<<endl; //Error if file is not opened
    else
        while(getline(inputfile2_, word2))
        {
            hash_table.resetNumOfCollisions();
            if(hash_table.Contains(word2))
                //+1 because probes count the insertion of value in intex + all collisions
                cout<< word2 << " Found " << hash_table.getCollisions()+1<<endl;
            else
                //+1 because probes count the insertion of value in intex + all collisions
                cout<< word2 << " Not_Found " << hash_table.getCollisions()+1<<endl;
        }
    inputfile2_.close();// closes file
}

// @argument_count: argc as provided in main
// @argument_list: argv as provided in imain
// Calls the specific testing function for hash table (linear, quadratic, or double).
int testHashingWrapper(int argument_count, char **argument_list) {
    const string words_filename(argument_list[1]);
    const string query_filename(argument_list[2]);
    const string param_flag(argument_list[3]);
    int R = 89;
    if (argument_count == 5) {
	const string rvalue(argument_list[4]);
	R = stoi(rvalue);
    }

    if (param_flag == "linear") {
        HashTableLinear<string> linear_probing_table;
        TestFunctionForHashTable(linear_probing_table, words_filename,
       			query_filename);
    } else if (param_flag == "quadratic") {
	    HashTable<string> quadratic_probing_table;
	    TestFunctionForHashTable(quadratic_probing_table, words_filename,
				query_filename);
    } else if (param_flag == "double") {
	    cout << "r_value: " << R << endl;
	    HashTableDouble<string> double_probing_table;
	    TestFunctionForHashTable(double_probing_table, words_filename,
	 			query_filename);
    } else {
	cout << "Unknown tree type " << param_flag
	     << " (User should provide linear, quadratic, or double)" << endl;
    }
    return 0;
}

// Sample main for program create_and_test_hash. DO NOT CHANGE IT.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testHashingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char **argv) {
    if (argc != 4 and argc != 5) {
	cout << "Usage: " << argv[0]
	     << " <wordsfilename> <queryfilename> <flag>" << endl;
	cout << "or Usage: " << argv[0]
	     << " <wordsfilename> <queryfilename> <flag> <rvalue>" << endl;
	return 0;
    }

    testHashingWrapper(argc, argv);
    return 0;
}
