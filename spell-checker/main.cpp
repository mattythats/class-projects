#include <iostream> //cin/cout
#include "dictionary.h" //dictionary headers

int main(){
	Dictionary d(2000);
	d.load("big-english.txt");	
	std::string in;
	while(in.compare("quit") != 0){
		std::cout << "Enter a line of text to be analyzed. Enter \"quit\" to quit." << std::endl;
		getline(std::cin,in);
		if(in.compare("quit") == 0){
			break;
		}
		d.analyzeLine(in);
	}
	std::cout << std::endl << "Dictionary statistics" << std::endl;
	d.printStats();
}
