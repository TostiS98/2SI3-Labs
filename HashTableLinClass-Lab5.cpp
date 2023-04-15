#include "HashTableLin.h"
using namespace std;

#include <cmath>;
#include <iostream>;


// this is our constructor. It should create a hash table object, that is initially empty, and initial load factor equal to load.
// the memory allocated should be large enough to so that maxNUm keys can be stored without exceeding load.
// the size of the hash table is the smallest prime number such that the above requirements are met.
// E.g. If maxNum = 5, load = 0.4, then the size of the table should be 5/0.4 = 12.5, the closes prime-greater prime would be 13
// TIME COMPLEXITY: theta (n)
// SPACE COMPLEXITY: theta(n)
HashTableLin::HashTableLin(int maxNum, double load)
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

bool HashTableLin::isPrimeNumber(int n){
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


// I am going to implement this new helper method to help with the insertion process
int HashTableLin::ProbeHelper(int n){

	// begin by defininf the value of the probe and the number of successful probes
	int probe = n % size;
	int successfulProbes = 1;

	// while the table at the given index of probe is not 0 (I.e. it is not occupied)
	while (table[probe] != 0){

		// is the item already there?? If it is, then we will just return a negative 1
		if (table [probe] == n){
			return -1;
		}

		// otherwise, we need to egenerate a new probe, and also increase our successful probes
		else {
			probe = (probe + 1) % size;
			successfulProbes = successfulProbes + 1;
		}
	}

	// finally, we increment the probes variable by the number of tries it took, and then return the probe variable
	probes = probes + successfulProbes;
	return probe;
}

// ********** INSERTING TIME COMPLEXITIES
// WORST = N (ALL HASH TO THE SAME VALUE)
// AVERAGE = 1
// BEST = 1

// ********** SEARCHING TIME COMPLEXITIES
// WORST = N
// AVERAGE = 1
// BEST = 1

// HASH TABLES MITIGATE THE TRADEOFF BETWEEN FAST SEARCHING AND FAST INSERTING, BUT TAKE A TRADEOFF IN SPACE

// IN GENERAL, QUADRATIC AND LINEAR PROBING HAVE SIMILAR PREFORMANCE IN TERMS OF ASYMPTOTIC ANALYSIS
// BUT QUADRATIC PROBING SUFFERS FROM LESS CLUSTERING

// this function will be used to insert the key n into this hash table, if the key is not already there.
// collisions are resolved based upon whether this is being implemented with quadratic or linear probing
// if the inseration causes the table to exceed its maximum limit, then we should rehash the table by calling our rehash() function
// BEST CASE TIME COMPLEXITY IS 1, WORST CASE TIME COMPLEXITY IS N, average case is 1
// SPACE COMPLEXITY IS CONSTANT IN BEST CASE, IN WORST CASE WE NEED TO REHASH WHICH HAS N COMPLEXITY

// STACK
// INSERTING - ORDER OF 1,
// SEARCHING - ORDER OF N
// FASTER ACCESSING IN A HASH TABLE - HASH FUNCTION THAT REDUCES CLUSTERING, MORE LIKELY TO GET A HASH VALUE ON THE FIRST HASH
//
void HashTableLin::insert(int n)
{
    // we first make use of our probe helper function by setting the value of probe
	int probe = ProbeHelper(n);

	// if that element does not already exist in our hash table
	if (probe != -1){

		// we will need to rehash if the table has gotten too full, and set probe
		if (double(numKeys+1)/double(size) > maxLoad){
			rehash();
			probe = ProbeHelper(n);
		}

		// otherwise, we can simply insert the value of n, at the index of probe within the table
		table[probe] = n;
		numKeys = numKeys + 1;
	}

}

// this rehash function should allocate a bigger table, of double the size (also abiding by the greater than prime rule)
// all keys in the old table should be then hashed into this new table.
//AVERAGE CASE TIME COMPLEXITY SHOULD BE N, WORST CASE TIME COMPLEXITY SHOULD BE N ASWELL
// SPACE COMPLEXITY IS ALSO N
void HashTableLin::rehash()
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
bool HashTableLin::isIn(int n)
{

	// we being by defining our probe based on the given hash function
    int probe = n % size;

    // for all of the elements in our table
    while (table [probe] != 0){

    	// if we find the element, return true
    	if (table [probe] == n){
    		return true;
    	}
    	else {
    		// otherwise, move the probe to the next probe location
    		probe = (probe + 1) % size;
    	}
    }
    return false;
}

// all this function has to do is to print all of the keys in the hash table
// TIME: WORST, AVERAGE, BEST = N
// SPACE: CONSTANT
void HashTableLin::printKeys()
{
    for (int i = 0; i < size; i++){
    	std::cout << table[i] << std::endl;
    }
}

// this function should simply print the keys and indeces of the hash table
// TIME: N (FOR ALL)
// SPACE: CONSTANT
void HashTableLin::printKeysAndIndexes()
{
    for (int i = 0; i < size; i ++){
    	std::cout << i << ", " << table[i] << std::endl;
    }
}

int HashTableLin::getNumKeys() {
	// TODO, change following code after completing this function
    return numKeys;
}

int HashTableLin::getTableSize() {
	// TODO, change following code after completing this function
    return size;
}

double HashTableLin::getMaxLoadFactor() {
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
std::vector<double> HashTableLin::simProbeSuccess()
{

	// we begin by defining the result vector, which we know is of size 9, as well as some variables to hold the number of keys and the total
    vector<double> result(9);
    double runningTotal = 0;

    // the total keys have been decreased to 1000 for testing and speed while running.
    const int totalKeys = 100000;


    for (int i = 1; i <= 9; i++) {

    	// we define our running total to be 0 initially
    	runningTotal = 0;

    	// then our load factor is set to i/10, which will range from [0.1, 0.9] since the outer for loop goes from i = 0 to i = 9
        double loadFact = double(double(i)/10.0);


        for (int j = 0; j < 100; j++) {


        	// make our new hash table
            HashTableLin hashTable(totalKeys, loadFact);

            // while we are less than the total number of keys (100K)
            while (hashTable.numKeys < totalKeys) {
            	hashTable.insert((rand()+1) * (rand()+1));
            }

            // add the number of probes to the runningTotal of probes
            runningTotal += double(hashTable.probes);
        }

        // compute the result vector of all corresponding values in the vector for average successful probes
        result[i - 1] = ((runningTotal / 100.0) / double(totalKeys));
    }
    return result;
}

// for bonus
std::vector<double> HashTableLin::simProbeUnsuccess()
{
    // TODO, change following code after completing this function
    vector<double> result(9);
    return result;
}
