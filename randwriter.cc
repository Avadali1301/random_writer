#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "markov.h"
using namespace std;
// use stoi instead of atoi because stoi throws error
int checkK(int argc, char * argv[]) {
	int k;
	try {
		k = stoi(argv[1]);
		if(stod(argv[1]) != k) {
			cerr << "ERROR: k cannot be a double" << endl;
			exit(0);
			return 0;
		}
	}
	catch (...) {
		cerr << "ERROR: k must be an integer" << endl;
		exit(0);
		return 0;
	}
	if(k < 0) {
		cerr << "ERROR: k is negative";
		exit(0);
		return 0;
	}
	return k;
}
int checkLen(int argc, char * argv[]) {
	int length;
	try {
		length = stoi(argv[2]);
		if(stod(argv[2]) != length) {
			cerr << "ERROR: length cannot be a double" << endl;
			exit(0);
			return 0;
		}
	}
	catch(...) {
		cerr << "ERROR: length must be an integer" << endl;
		exit(0);
		return 0;
	}
	if(length < 0) {
		cerr << "ERROR: length is negative";
		exit(0);
		return 0;
	}
	return length;
}
string checkSource(int argc, char * argv[]) {
	string source_name = argv[3];
	char temp;
	string source;
	ifstream source_file(source_name);
	if(!source_file) {
		cerr << "ERROR: Source file does not exist.";
		exit(0);
		return "";
	}
	while(source_file.get(temp)) {
		source += temp;
	}
	source_file.close();
	// if k wasn't int, program would've already terminated
	if(source.length() <= stoi(argv[1])) {
		cerr << "ERROR: Seed size ≥ number of characters in source" << endl;
		exit(0);
		return "";
	}
	return source;
}
void checkResult(int argc, char * argv[]) {
	string result_name = argv[4];
	
	ifstream rf(result_name);
	if(!rf) {
		cerr << "ERROR: Result file does not exist";
		exit(0);
	}
}
int main(int argc, char * argv[]) {
	if(argc != 5) {
		cerr << "ERROR: Too few arguments passed";
		return 0;
	}
	int k = checkK(argc, argv);
	int length = checkLen(argc, argv);
	string source = checkSource(argc, argv);
	checkResult(argc, argv);
	markov m = markov(k, length, source, argv[4]);
	m.execute();
}





