#include <iostream>
#include <cstdlib>
#include "hashMap.h"
using namespace std;
hnode::hnode() {
	next = NULL;
	prev = NULL;
}
hashMap::hashMap() {
}
// circular dll for hashmap
hashMap::hashMap(int tsize, int ssize) {
	// number keys of size ssize possible 
	// from a text of tsize is tsize - ssize + 1
	table_size = tsize - ssize + 1;
	table = new hnode * [table_size];
	for(int i = 0; i < table_size; i++) {
		table[i] = new hnode;
		table[i] -> next = table[i];
		table[i] -> prev = table[i];
	}
}
// need separate destructor bc init hashMap in markov constructor
hashMap::~hashMap() {
}
// real destructor
void hashMap::destroy() {
	for(int i = 0; i < table_size; i++) {
		hnode * n = table[i] -> next;
		while(n != table[i]) {
			hnode * temp = n;
			n = n -> next;
			delete temp;
		}
		delete table[i];
	}
}
int hashMap::hf(string k) const {
	// 256 characters in ASCII table, so a about 256 & a prime
	int a = 257;
	// large prime works well
	int b = 1e9 + 9;
	// going to blow up quickly so extra long
	long long p = 1;
	long long value = 0;

	for(int i = 0; i < k.length(); i++) {
		// hash weights ascii value & location in string
		value += k[i] * p;
		// since a & b large primes, p value unlikely to repeat
		p = (p * a) % b;
	}
	// keep hash in range [0, table_size - 1]
	return value % table_size;
}
// PRE: don't already have entry with same key in hashMap
void hashMap::add(string k, string v) {
	int index = hf(k);
	hnode * front = table[index];
	hnode * n = new hnode;
	n -> prev = front -> prev;
	n -> next = front;
	front -> prev -> next = n;
	front -> prev = n;
	n -> key = k;
	n -> value = v;
}
string hashMap::get(string k) const {
	int index = hf(k);
	hnode * front = table[index];
	hnode * n = front -> next;
	while(n != front) {
		if((n -> key).compare(k) == 0) {
			return n -> value;
		}
		n = n -> next;
	}
	// # characters after seed could be any string of length < table_size + 1
	// generate string of length table_size + 1
	// return this, and verifies element not in hashmap. 
	string max_string = "";
	for(int i = 0; i < table_size + 1; i++) {
		max_string += "0";
	}
	return max_string;
}

