#include "HashTableQuad.h"
using namespace std;

#include<cmath>
#include <iostream>;

// this is again a helper function used to determine a prime number
// this is the same function that i had implemented in the linear hash table class
// this is required because when we need to select the size of the hash table, we need to use the closest prime number
// that is above the calculated size
//TIME COMPLEXITY: N
// SPACE COMPLEXITY: CONSTANT
bool HashTableQuad::isPrimeNumber(int n){
	if (n == 0 || n ==1){
		return false;
	}

	for (int i = 2; i < n; i++){
		if (n % i == 0){
			return false;
		}
	}
	return true;
}

//// this is our constructor. It should create a hash table object, that is initially empty, and initial load factor equal to load.
// the memory allocated should be large enough to so that maxNUm keys can be stored without exceeding load.
// the size of the hash table is the smallest prime number such that the above requirements are met.
// E.g. If maxNum = 5, load = 0.4, then the size of the table should be 5/0.4 = 12.5, the closes prime-greater prime would be 13
// THIS IS THE EXACT SAME IMPLEMENTATION AS THE LINEAR HASH TABLE, THERE IS NO DIFFERENCE BETWEEN THE CONSTRUCTORS
// TIME COMPLEXITY: theta (n)
// SPACE COMPLEXITY: theta(n)
HashTableQuad::HashTableQuad(int maxNum, double load)
{
	// we can start by defining all of the class variables that we have inside of our linear hash table class
    maxLoad = load;
    numKeys = 0;
    probes = 0;
    size = ceil(double(maxNum)/load);

    // we first need to ensure that the size value that we just calculated is a prime number
    // I have implemented my own prime number checker
    // I will begin by first checking size, then I will add one to size until I have gotten a prime number
    while (!isPrimeNumber(size)){
    	size = size + 1;
    }

    // once we have our size variable set to the nearest greater-prime number, we can begin populating our table with zeros
    // I am using the push_back method, since the table is defined as a vector in our header file
    for (int i = 0; i < size; i++){
    	table.push_back(0);
    }
}

//Below is one of the helper functions that I have implemented, this will help generate the appropriate probe for us to insert
// a given element into the hash table
// this implementation IS different from the liner table. This is because quadratic probing requires that collisions be handled differently
int HashTableQuad::ProbeHelper(int n){

	int initialProbe = n % size;
	int probe = initialProbe;
	int successfulProbes = 1;
	int index = 1;

	// while the current index of the table is not occupied (I.e. has a value of 0)
	while (table [probe] != 0){

		// if we have found that number, then we do not want to insert it, thus we can return negative 1
		if (table[probe] == n){
			return -1;
		}
		else {

			if (index >= size){
				return -1;
			}

			// this allows us to implement that quadratic probing
			probe = (initialProbe + index*index)%size;
			index = index + 1;
			successfulProbes = successfulProbes + 1;
		}

	}

	probes = probes + successfulProbes;
	return probe;

}

// this function will be used to insert the key n into this hash table, if the key is not already there.
// collisions are resolved based upon whether this is being implemented with quadratic or linear probing
// if the inseration causes the table to exceed its maximum limit, then we should rehash the table by calling our rehash() function
// TIME: AVERAGE = 1, WORST CASE = N
// SPACE: CONSTANT
void HashTableQuad::insert(int n)
{
	// we use our probe helper function to help us with this code
    int probe = ProbeHelper(n);

    // probe will only be -1 if that element already exists in the table, or if we exceeded the indeces of the table
    if (probe != -1){

    	// if the table has gotten to capacity, then we should rehash the table
    	if (double(numKeys + 1) / double(size) > maxLoad){
    		rehash();
    		probe = ProbeHelper(n);
    	}
    		table[probe] = n;
    		numKeys = numKeys + 1;

    }
}


// this rehash function should allocate a bigger table, of double the size (also abiding by the greater than prime rule)
// all keys in the old table should be then hashed into this new table.
// TIME: AVERAGE IS N
// SPACE: N
void HashTableQuad::rehash()
{
    // we can first create a copy of our table as a vector
    std::vector<int> copy(table);


    // we will want to double the size and then check for primes as we did previously
    size = 2 * size;
    while (isPrimeNumber(size) == false) {
        size++;
    }

    table.resize(size, 0);

    // we can begin by populating with zeros
    for (int i = 0; i < copy.size(); i++) {
        table[i] = 0;
    }

    // then, as we did before, we can use our insert function to hash all of the old keys into our table
    numKeys = 0;
    probes = 0;
    for (int i = 0; i < copy.size(); i++) {
        if (copy[i] != 0) {
           insert(copy[i]);
        }
    }
}

// this function should return true if the value n is in the hash table, and false if that value n is not in the hash table
// BEST CASE 1, AVERAGE CASE N, WORST CASE N
// SPACE COMPLEXITY: CONSTANT
bool HashTableQuad::isIn(int n)
{
    int initialProbe = n % size;
    int probe = initialProbe;
    int index = 1;

    // while we are still looking at elements that are actually in the table (0s populated initially)
    while (table[probe] != 0){

    	// if the table at the very first probe check is the value we are looking for, then we can automatically return true
    	if (table[probe] == n){
    		return true;
    	}


    	else {
    		// if the probing index exceeds the table size, then we should stop looking
    		if (index >= size){
    			return false;
    		}

    		probe = (initialProbe + index*index) % size;
    		index++;

    	}
    }

    return false;
}

void HashTableQuad::printKeys()
{
    for (int i = 0; i < size; i++){
    	std::cout << table[i] << std::endl;
    }
}

void HashTableQuad::printKeysAndIndexes()
{
    for (int i = 0; i < size; i++){
    	std::cout << i << ", " << table [i] << std::endl;
    }
}

int HashTableQuad::getNumKeys() {
	//return the number of keys
    return numKeys;
}

int HashTableQuad::getTableSize() {
	// TODO, change following code after completing this function
    return size;
}

double HashTableQuad::getMaxLoadFactor() {
	// TODO, change following code after completing this function
    return maxLoad;
}


// this function should preform a simulation to measure the average number of probes for a successful search for linear probing (in this case)
// this function should return a vector of size 9, which contains the number of probes and successful searches in a table with load factor lambda = 0.1, 0.2, etc...
// for each load factor, lambda, we should allocate a hash table, insert at minimu 100k randomly generated numbersa nd then compute the average number of probes
// needed. Repeat the test 100 times.
// ensure that the initial table is large enough to ensure no rehashing is required, and after all 100,000 elements are inserted the load factor should be the
// same as the required lambda (I.e. pass 100,000 and lambda to the constructor)
// make sure to properly count the number of inserted keys, taking into account the fact that duplicates are not inserted into the table
// write a private helper method called insert
std::vector<double> HashTableQuad::simProbeSuccess()
{
    // TODO, change following code after completing this function
    vector<double> result(9);
    double total = 0;
    int totalKeys = 100000;

    for (int i = 1; i <= 9; i++){
    	total = 0;

    	// set our load factor to i/10, so that we can test 0.1 -> 0.9 inclusive
    	double loadFactor = double(double(i) / 10.0);

    	for (int j = 0; j < 100; j++){

    		// make our new hash table
    		HashTableQuad hashTable(totalKeys, loadFactor);

    		// while we are less than the number of keys
    		while (hashTable.numKeys < totalKeys){

    			// insert random num
    			hashTable.insert((rand() + 1) * (rand() + 1));
    		}

    		// add probes to the running total
    		total = total + double(hashTable.probes);
    	}

    	// results matrix
    	result [i - 1] = ((total/100.0) / double(totalKeys));

    }
    return result;
}
