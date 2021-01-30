#include "dictionary.h"
#include <iostream> //cin/cout
#include <list> //std::list and std::list::iterator
#include <string> //std::string
#include <cmath> //pow()
#include <fstream> //ifstream
#include <chrono> //std::chrono
#include <climits> //INT_MIN/INT_MAX
#include <cstring> //strcpy

//constructor
Dictionary::Dictionary(int b){
	buckets = b;
	words = new std::list<std::string>[b]; //an array of lists of words
	bucketSizes = new int[b];
	for(int i = 0; i < b; i++){
		bucketSizes[i] = 0;
	}
}

//hash takes a string as a parameter and converts it to an integer
unsigned long int Dictionary::hash(std::string s){
	unsigned long int h = 0;
	for(int i = s.length(); i >= 0; i--){
		//converts each letter to its ascii number,
		//multiplies it by 128 to the power of its distance from the end,
		//and adds it to the hash value
		h+= s[i]*pow(128,(s.length()-i));
	}
	long double conDec = h*.618;
	unsigned long int conWhole = h*.618;
	float mod = (conDec-conWhole);
	if(mod < 0){
		mod *=-1; //unlikely but just in case
	}
	return buckets*mod;
}

//insert stores a word s in the list pointed to by bucket [hash%bucket]
void Dictionary::insert(std::string s){
	auto loadStart = std::chrono::system_clock::now();
	for(int i = 0; i < s.length(); i++){
		s[i] = tolower(s[i]); //convert to lowercase for ease of searching
	}
	auto loadEnd = std::chrono::system_clock::now();
	loadTime+=(loadEnd-loadStart);
	unsigned long int h = hash(s);
	words[h].push_back(s);
	numWords++;
	bucketSizes[h]++;
}

//search searches for a word s in the dictionary
bool Dictionary::search(std::string s){
	unsigned long int h = hash(s);
	bool found = false;
	if(bucketSizes[h]){
		for(std::list<std::string>::iterator it = words[h].begin(); it != words[h].end(); it++){
			if(*it == s){
				found = true;
				break;
			}
		}
	}
	return found;
}

//dictionary takes one parameter, the name of a file
//if the file exists, each word will be inserted in the dictionary
void Dictionary::load(std::string filename){
	std::ifstream myfile(filename);
	if(myfile.fail()){
		std::cout << "A file with filename " << filename << " cannot be found" << std::endl;
	} else {
		while(!myfile.eof()){
			std::string s;
			getline(myfile,s);
			s = s.substr(0,s.length()); //to get rid of the newline character
			insert(s);
		}
		myfile.close();
	}
}

//prints each bucket and the list it points at
void Dictionary::print(){
	for(int i = 0; i < buckets; i++){
		std::cout << "Bucket " << i << ":";
		for (std::list<std::string>::iterator it=words[i].begin(); it != words[i].end(); ++it){
			std::cout << " " << *it;
		}
		std::cout << std::endl << std::endl;
	}
}

//prints different statistics about the dictionary
void Dictionary::printStats(){
	std::cout << "There are " << numWords << " words in the dictionary." << std::endl;
	int largest = INT_MIN;
	int smallest = INT_MAX;
	int usedBuckets = 0;
	for(int i = 0; i < buckets; i++){
		if(bucketSizes[i] < smallest && bucketSizes[i] != 0){
			smallest = bucketSizes[i];
		}
		if(bucketSizes[i] > largest){
			largest = bucketSizes[i];
		}
		if(bucketSizes[i] != 0){
			usedBuckets++;
		}
	}
	float avg = float(numWords)/float(buckets);
	float avgU = float(numWords)/float(usedBuckets);
	std::cout << "Smallest non-zero bucket size is " << smallest << "." << std::endl;
	std::cout << "Largest bucket size is " << largest << "." << std::endl;
	std::cout << "There are " << buckets << " buckets." << std::endl;
	std::cout << usedBuckets << " buckets are being used." << std::endl;
	std::cout << "There is an average of " << avg << " words per bucket." << std::endl;
	std::cout << "There is an average of " << avgU << " words per used bucket." << std::endl;
	std::cout << "It took " << loadTime.count() << " seconds to load and insert " << numWords << " words." << std::endl;
}

//correctLine takes one parameter, a string of text, analyzes the strings for any mispellings and prints out a short analysis
void Dictionary::analyzeLine(std::string s){
	//start with the raw string
	std::string justChar = onlyChars(s); //returns just a string of all the words

	//split the string by its spaces
	std::list<std::string> splitWords = split(justChar);
	std::cout << std::endl;
	if(splitWords.size() < 1){
		std::cout << "Every word is in the dictionary!" << std::endl;
	} else {
		std::list<std::string>* suggestions = new std::list<std::string>[splitWords.size()];
		int index = 0;
		int numSuggestions = 0;
		auto editStart = std::chrono::system_clock::now();
		//loads suggestions[index] with the editDist list for the word @ corrections[index]
		for(std::list<std::string>::iterator it = splitWords.begin(); it != splitWords.end(); it++){
			suggestions[index] = editDist(*it);
			numSuggestions += suggestions[index].size();
			index ++;
		}
		auto editEnd = std::chrono::system_clock::now();
		std::chrono::duration<double> editTime = editEnd-editStart;
		std::cout << "There are " << index << " mispelled words." << std::endl;
		std::cout << "There are " << numSuggestions << " suggestions availiable." << std::endl;
		std::cout << "It took " << editTime.count() << " seconds to find them." << std::endl;
		std::cout << std::endl;
	}
}

//onlyChars takes one parameter, a string of text, returns a string stripped of all nonletter, nonspace characters
std::string Dictionary::onlyChars(std::string s){
	for(int i = 0; i < s.length(); i++){
		s[i] = tolower(s[i]);
		//skips lowercase letters, apostrophe, uppercase letters, numbers, dash, and space
		if((s[i] > 64 && s[i] < 91) || (s[i] > 96 && s[i] < 123) || (s[i] > 43 && s[i] < 57) || s[i] == ' ' || s[i] == 39){
		} else {
			s.replace(i,1,""); //clears out non char/non space characters
			i--;
		}
	}
	return trim(s); //returns the string without excess spaces
}

//trim takes one parameter, a string of text, and returns the string with no excess spaces
std::string Dictionary::trim(std::string s){
	while(s[0] == ' '){
		s.replace(0,1,""); //clears out leading spaces
	}
	for(int i = 0; i < s.length()-1; i++){
		if(s[i] == ' ' && s[i+1] == ' '){
			s.replace(i,1,""); //removes spaces that are adjacent to each other
			i--;
		}
	}
	if(s[s.length()] == ' '){
		s.replace(s.length(),1,""); //removes trailing spaces
	}
	return s;
}

//split takes a string and returns a list of words in the string split by spaces
std::list<std::string> Dictionary::split(std::string s){
	std::list<std::string> splitWords;
	while(s.length() > 0){
		int start = 0;
		int end = s.find(" ",start);
		if(search(s.substr(start,end))){
			std::cout << s.substr(start,end) << " ";
		} else {
			std::cout << "\033[31m" << s.substr(start,end) << " ";
			splitWords.push_back(s.substr(start,end));
			std::cout << "\033[0m";
		}
		if(end+1 == '\0'){ //if the next character is the null terminatior then its time to make like spinach artichoke and dip
			break;
		}
		s = s.substr(end+1); //removes the word that was just added
	}
	return splitWords;
}

//takes a string and returns a list of words in the string that are one or two edit distances away from the passed in string
std::list<std::string> Dictionary::editDist(std::string s){
	std::list<std::string> finalList;
	std::list<std::string> oneEditOut;
	std::cout << "Searching for valid words within one edit distance of " << s << "." << std::endl;
	oneEditOut.merge(replaceChar(s));
	oneEditOut.merge(addChar(s));
	oneEditOut.merge(delChar(s));
	oneEditOut.merge(swapAdjChar(s));
	oneEditOut.unique();
	std::cout << "Searching for valid words within two edit distances of " << s << "." << std::endl;
	std::list<std::string> twoEditOut;
	for(std::list<std::string>::iterator it = oneEditOut.begin(); it != oneEditOut.end(); it++){
		twoEditOut.merge(replaceChar(*it));
		twoEditOut.merge(addChar(*it));
		twoEditOut.merge(delChar(*it));
		twoEditOut.merge(swapAdjChar(*it));
	}
	twoEditOut.unique(); //lmao this takes really long (relatively)
	//after doing all the editDist shenanigans
	finalList.merge(oneEditOut);
	finalList.merge(twoEditOut);
	finalList.sort();
	finalList.unique();
	if(finalList.size() < 1){
		std::cout << "No corrections for " << s << "." << std::endl;
	}
	return finalList;
}

//take a string and returns a list of strings in the dictionary that have all but one character match
std::list<std::string> Dictionary::replaceChar(std::string s){
	std::string r = s; //string being edited
	std::list<std::string> repl;
	std::string l("abcdefghijklmnopqrstuvwxyz/0123456789- '");
	for(int i = 0; i <= s.length(); i++){ //this loops through the string char by char
		for(int j = 0; j < l.length(); j++){ //this loops through the letters array
			r[i] = l[j];
			if(search(r) && r.compare(s) != 0){
				repl.push_back(r);
			}
		}//end letters loop
		r = s; //resets r to s so we remain one edit distance out through the replace operation
	}//end char loop
	return repl;
}

//takes a string and returns a list of strings in the dictionary that are the same but have an extra character
std::list<std::string> Dictionary::addChar(std::string s){
	std::string r; //string being edited
	std::list<std::string> add;
	std::string l("abcdefghijklmnopqrstuvwxyz/0123456789- '");
	for(int i = 0; i <= s.length(); i++){ //this loops through the string pos by pos
		for(int j = 0; j < l.length(); j++){ //this loops through the letters array
			r.append(s.substr(0,i-1));
			r[i] = l[j];
			r.append(s.substr(i));
			if(search(r) && r.compare(s) != 0){
				add.push_back(r);
			}
		}
	}//end char loop
	return add;
}

//takes a string and returns a list of all strings in the dictionary that are the same but are missing a character
std::list<std::string> Dictionary::delChar(std::string s){
	if(s.length() > 1){
		std::string r = s;
		std::list<std::string> del;
		for(int i = 0; i <= s.length(); i++){
			r.erase(i,1);
			if(search(r) && r.compare(s) != 0){
				del.push_back(r);
			}
			r = s;
		}
		return del;
	}
	return std::list<std::string>();
}

//takes a string and returns a list of all strings in the dictionary that are the same but have an adjacent pair of characters swapped
std::list<std::string> Dictionary::swapAdjChar(std::string s){
	if(s.length() > 1){
		std::string r = s;
		std::list<std::string> swaps;
		for(int i = 1; i <= s.length(); i++){
			char t = r[i-1];
			r[i-1] = r[i];
			r[i] = t;
			if(search(r) && r.compare(s) != 0){
				swaps.push_back(r);
			}
			r = s;
		}
	}
	return std::list<std::string>();
}
