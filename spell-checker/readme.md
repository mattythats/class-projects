# Spell Checker

This folder contains a spell checking program I created in C++ for COSC 320 at Salisbury University.
It works best in a Linux environment due to the usage of console colors.
The program takes a line of text as input and checks each word against a dictionary thats loaded into a hashtable.
If a word is found to be incorrect, the program searches for words in the dictionary that are one or two edit distances away from the incorrect word.

Edit distance is defined as:
-One letter added
-One letter removed
-Adjacent letters swapping places
-One letter replaced with another

To run the code download this folder run the makefile with the command make and then run the program with the command ./main.
