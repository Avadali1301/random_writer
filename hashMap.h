#include <iostream>
#include <cstdlib>
using namespace std;
// CITATION: Learned about separate training from Algorithms by Sedgewick
struct hnode{
    string key;
    string value;
    hnode * next;
    hnode * prev;
    
    hnode();
};
// separate chaining through circ. dll to prevent collisions
class hashMap {
	private:
		hnode ** table;
		int table_size;
	public:
		// tsize = source size
		// ssize = seed size
		hashMap(int tsize, int ssize);
		hashMap();
		~hashMap();
		void destroy();
		// hf = hash function
		int hf(string k) const;
		void add(string k, string v);
		string get(string k) const;
};