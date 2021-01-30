#ifndef DICT_H
#define DICT_H
#include <list> //std::list
#include <string> //std::string
#include <chrono> //std::chrono

//this class defines a dictionary class using a hashtable structure
class Dictionary{
	private:
		int buckets; //number of buckets
		std::list<std::string>* words; //an array of lists to hold the word
		int* bucketSizes; //stores the sizes of the buckets
		unsigned long int hash(std::string s); //a function to hash words
		int numWords = 0; //number of words
		std::chrono::duration<double> loadTime = std::chrono::duration<double>::zero(); //time to load words in the dictionary

		//everything under this is a helper function for correctLine(std::string)
		std::list<std::string> correctWord(std::string); //returns a list of possible corrections for a word
		std::string onlyChars(std::string); //strips a string of all non character members
		std::string trim(std::string); //trims extra spaces from a string
		std::list<std::string> split(std::string); //splits a string by the spaces

		//these are helper methods for editDist
		std::list<std::string> replaceChar(std::string); //replaces one character in string
		std::list<std::string> addChar(std::string); //adds one character to string
		std::list<std::string> delChar(std::string); //deletes a character from a string
		std::list<std::string> swapAdjChar(std::string); //swaps two adjacent characters in a string
	public:
		Dictionary(int); //constructs a dictionary with n buckets
		void insert(std::string s); //inserts a word in the dictionary
		bool search(std::string s); //finds a word in the dictionary
		void load(std::string filename); //given a file, inserts all the words in the file to the dictionary
		void print(); //prints the dictionary
		void printStats(); //prints statistics on the dictionary
		void analyzeLine(std::string); //given a string, corrects the misspelled words
		std::list<std::string> editDist(std::string); //returns a list of strings in the dictionary within one edit distance of s
};

#endif
