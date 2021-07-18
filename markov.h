#include <iostream>
#include <cstdlib>
#include "hashMap.h"
using namespace std;
// CITATION: used these 2 papers for 2-way search pseudocode + logic + proofs :)
// http://www.quretec.com/u/vilo/edu/2002-03/Tekstialgoritmid_I/Articles/Exact/Two-way-p650-crochemore.pdf
// http://www.stupros.com/site/postconcept/Breslauer-Grossi-Mignosi.pdf
// used this website for a bit of code guidence + logic: https://www-igm.univ-mlv.fr/~lecroq/string/node26.html
// Look at README for fairly detailed explanation of workings of 2-way search
class markov {
	private:
		string seed;
		int k;
		int length;
		string source;
		string result;
		// map for storing records of seed and char pairs
		hashMap records;
		bool useMap;
	public:
		markov(int ks, int l, string s, string r);
		~markov();
		void mas(int pair[], bool condition); // maximum alphabetical suffix
		string tws(); // 2-way string search
		int max(int a, int b);
		// KMP search + pre-process
		void pre_process(int ps[]) const;
		string search() const;
		
		int randIndex(int limit) const;
		string genSeed() const;
		void iteration(string chars);
		void execute();
};



