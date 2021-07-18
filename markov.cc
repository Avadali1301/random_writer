#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include "markov.h"

markov::markov(int ks, int l, string s, string r) {
	k = ks;
	length = l;
	source = s;
	result = r;
	srand(time(0));
	// seed guaranteed to repeat if length > source.length() + 3
	if(length > source.length() + 3 && k != 0) {
		records = hashMap(source.length(), k);
		useMap = true;
	}
	else {
		useMap = false;
	}
	// hashMap destructor call here
}
markov::~markov() {
	if(useMap){
		records.destroy();
	}
}
int markov::max(int a, int b) {
	if(a >= b) {
		return a;
	}
	else {
		return b;
	}
}
// alphabetically maximal suffix of source
// used for computing critical factorization
void markov::mas(int pair[], bool condition) {
	pair[0] = 1; //period
	pair[1] = -1; //index beginning max suffix (a bit like lps in KMP)
	int j = 0; // suffix test
	int q = 1; // period test
	char a;
	char b;
	while(j + q < k) {
		a = seed[pair[1] + q];
		b = seed[j + q];
		if((b < a && condition) || (b > a && !condition)) {
			// period is current prefix
			j += q;
			q = 1;
			pair[0] = j - pair[1];
		}
		else {
			if(b == a) {
				// reset q
				if(q == pair[0]) {
					j += pair[0];
					q = 1;
				}
				else {
					q += 1;
				}
			}
			else {
				// suffix greater. restart
				pair[1] = j;
				j += 1;
				q = 1;
				pair[0] = 1;
			}
		}
	}
}
string markov::tws() {
	int pair1[2];
	mas(pair1, true);
	int p0 = pair1[0];
	int l0 = pair1[1];
	mas(pair1, false);
	int p1 = pair1[0];
	int l1 = pair1[1];
	int p = p1;
	int l = l1;
	int n = source.length();
	string chars = "";
	// decide to use left or right factorization
	if(l0 > l1) {
		p = p0;
		l = l0;
	}
	// substring periodic?
	if(seed.substr(0, l + 1).compare(seed.substr(p, l + 1)) == 0) {
		l1 = 0;
		p0 = -1; // recycle p0 variable
		while(l1 < n - k + 1) {
			// which factorization start at?
			l0 = max(l + 1, p0 + 1);
			while(l0 < k && seed[l0] == source[l0 + l1]) {
				l0 += 1;
			}
			if(l0 > k - 1) {
				// reset l0
				l0 = l;
				while(l0 > p0 && seed[l0] == source[l0 + l1]) {
					l0 -= 1;
				}
				if(l0 < p0 + 1 && l1 + k < n) {
					chars += source[l1 + k];
				}
				// increase by period
				l1 += p;
				// reset p0
				p0 = k - p - 1;
			}
			// go forward in source
			else {
				l1 += (l0 - l);
				p0 = -1;
			}
		}
	}
	else {
		// resetting period
		p = max(l + 2, k - l);
		l1 = 0;
		while(l1 < n - k + 1) {
			l0 = l + 1;
			while(l0 < k && seed[l0] == source[l0 + l1]) {
				l0 += 1;
			}
			// reset l0
			if(l0 > k - 1) {
				l0 = l;
				while(l0 > -1 && seed[l0] == source[l0 + l1]) {
					l0 -= 1;
				}
				if(l0 < 0 && l1 + k < n) {
					chars += source[l1 + k];
				}
				l1 += p;
			}
			// go forward in source
			else {
				l1 += (l0 - l);
			}
		}
	}
	return chars;
}
// KMP pre-process
void markov::pre_process(int ps[]) const{
	// first entry is 0 since no proper prefix/suffix
	ps[0] = 0;
	int i = 1;
	int j = 0;
	while(i < k) {
		if(seed[i] == seed[j]) {
			ps[i] = j + 1;
			i += 1;
			j += 1;
		}
		else if(j == 0) {
			ps[i] = 0;
			i += 1;
		}
		// could still be prefix suffix match earlier, so traverse back
		else {
			j = ps[j - 1];
		}
	}
}
// KMP search
string markov::search() const {
	int ps[k];
	pre_process(ps);
	string chars;
	int i = 0;
	int j = 0;
	int n = source.length();
	while(i < n) {
		if(source[i] == seed[j]) {
			i++;
			j++;
		}
		if(j == k) {
			// i-j = index of start of seed
			// check if character after seed exists
			if(i + k - j < source.length()) {
				chars += source[i - j + k];
			}
			j = ps[j - 1];
		}
		else if(i < n && source[i] != seed[j]) {
			if(j != 0) {
				// could still be match earlier, so traverse back
				j = ps[j - 1];
			}
			else {
				i++;
			}
		}
	}
	return chars;
}
int markov::randIndex(int limit) const {
	return rand() % limit;
}
string markov::genSeed() const {
	// if index > source.length() - k, run out of chars to get for seed
	int index = randIndex(source.length() - k + 1);
	return source.substr(index, k);
}
void markov::iteration(string chars) {
	int index = randIndex(chars.length());
	char update = chars[index];
	// seed doesn't matter if k = 0
	if(k != 0) {
		seed = seed.substr(1, k - 1) + update;
	}
	ofstream result_file;
	result_file.open(result, ios::app);
	result_file.put(update);
	result_file.close();
}

void markov::execute() {
	string chars;
	string bad_seed;
	if(k != 0) {
		seed = genSeed();
		chars = tws();
	}
	for(int i = 0; i < length; i++) {
		// generate new seed?
		bad_seed = seed;
		while(k != 0 && chars.compare("") == 0 && seed.compare(bad_seed) == 0) {
			seed = genSeed();
		}
		if(k != 0) {
			iteration(tws());
			if(useMap){
				// checks if entry in records
				if(records.get(seed).length() != source.length() - k + 2) {
					chars = records.get(seed);
				}
				else {
					chars = tws();
					records.add(seed, chars);
				}
			}
			else {
				chars = tws();
			}
		}
		else {
			iteration(source);
		}
	}
}



