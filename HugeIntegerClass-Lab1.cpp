#include "HugeInteger.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// this was a new constructor made to use a vector to hold the integer values for the huge int
// it is being used in the addition function when making the copy of h, we use this to transform
// it into a vector
HugeInteger::HugeInteger(std::vector<int>& h){
	huge = h;
}

HugeInteger::HugeInteger(const std::string& val){

	int i;
	int len = val.length();

	/* The first exception that we will check is if the string has length 0! We will check if the
	 * len variable that we defined has a value of 0, and if it does, throw an appropriate exception
	 *
	 */
	if (len == 0){
		throw invalid_argument("Cannot have a huge int of length 0!!");
	}

	/* Next, we need to check if the value is intended to be a negative! We will check this by seeing
	 * If the first element in the string is a '-'
	 * If it does contain a negative as the first element, we will set the negative variable to 1
	 * we will also set i to 1, because we will want our iteration variable to start from index 1
	 * as supposed to index 0, since we dont want to actually turn the negative sign into an int
	 */
	if(val[0]=='-'){
		isNegative = 1;
		i = 1;
	}

	// if it is not negative, then we leave the negative flag as 0
	else{
		isNegative = 0;
		i = 0;
	}

	// we will iterate from i = 0 (or 1 if its negative) to the size of the string
	while(i < len){

		/* if our current value is out of the ASCII range for acceptable values (48 - 57)
		 * then we want to throw an appropriate error
		 */
		if (val[i] < 48 || val[i] > 57){
			throw invalid_argument("Invalid character!!");
		}

		/* If we are in the appropriate range, then we can convert the char to an int
		 * and then add it to our vector
		 */
		huge.push_back(val[i] - '0');
		i++;
	}

}

HugeInteger::HugeInteger(int n) {

	/* the first thing we want to check if that we are not being told to
	 * create a huge int of size 0! If so, we will throw an appropriate error
	 */
	if(n <= 0){
		throw invalid_argument("Invalid HugeInteger");
	}

	// insert a random number from 1 - 9 as the very first number
	huge.insert (huge.begin(), rand() % 9 + 1);

	// for the resultVectort of the numbers, insert any random number from 0 - 9
	for (int i = 2; i <= n; i ++){
		huge.push_back(rand() % 10);
	}

}

// *******************************************************************************************************
HugeInteger HugeInteger::add(const HugeInteger& h) {


	/* First we should define some useful constants
		 * carryDigit and addSum will be used to define the sum component and carryDigit component of our addition operations
		 * i, j and k will be used to index the larger vector, smaller vector, and vector of sums resultVectorpectively
		 * the vector 'sum' will be used to store the final sum between the 2 huge ints -- it gets turned in...
		 * ... to a huge int later in the code
		 */
	int carryDigit = 0;
	int sumDigit = 0;
	int i = 0;
	int j = 0;
	int hugeSize = huge.size();
	int hSize = h.huge.size();
	vector<int> sum;

	// this very first code block defines the logic for addition with 2 posotive, and 2 negative numbers
	// these two processes are the same, because addition with 2 negative numbers (for my purposes)
	// is the exact same process, but the numbers will retain their isNegative = 1, rather than isNegative = 0
	if( (this->isNegative == 0 && h.isNegative == 0) || (this->isNegative == 1 && h.isNegative == 1) ){

		// ----------------------------------------------------------------------------------------------------------
		/* This first case scenario is that our original hugeInt is larger in size (or equal) than the
		* parameter int, If this is the case, then we will know which variable to base our iteration off of
		* we define our iterable variables to start at the very end of the vector, this is to
		* simulate addition on paper - we always start with the most significant digits and move to the
		* least significant digits
		*/
		if (hugeSize >= hSize){
			i = hugeSize - 1;
			j = hSize - 1;


			/* Now we are going to loop through the smaller array, we will loop such that the value for
			* j is greater than or equal to 0
		    * Inside of this loop, all we are doing is adding the corresultVectorponding elements of h and huge
			* we also need to take into account the carry from the previous edition
			* we will split the edition into 2 parts essentially
			* I.e. lets say huge[i] + h[j] = 12, then 12%10 = 2, and 12/10 = 1,
			* thus, modding by 10 gives us the sum, and integer division with 10 gives us the carry
			*/
			while(j>=0){
				sumDigit = huge[i] + h.huge[j] + carryDigit;
				sum.insert(sum.begin(),(sumDigit%10));
				carryDigit = sumDigit/10;

				i--;
				j--;
			}

			/* This portion handles the fact that the size of i can potentially be larger than j
			* this means that there will be integers that are left over in the larger vector, that will need
		    * to carry over into the sum vector
			* even thought we are simply transferring numbers, we need to take into account the carry from
			* the previous step addition, as well as the very last addition, which we handle in
			* the very last if statement (if carry == 1)
			*/
			while(i>=0){
				sumDigit = huge[i] + carryDigit;
				sum.insert(sum.begin(),(sumDigit%10));
				carryDigit = sumDigit/10;

				i--;
			}
			if(carryDigit == 1){
				sum.insert(sum.begin(),(carryDigit));
			}
		}
		// -----------------------------------------------------------------------------------------------

		// this implementation is the exact same thing as above, except it is for when h is a larger size than huge
		// the only thing that is different from this code to the code above is that we iterate through the
		// hugeSize vector (since it is smaller in this case) rather than the huge vector.

		else if(hSize > hugeSize){
			j = hugeSize - 1;
			i = hSize - 1;


			while(j>=0){
				sumDigit = huge[j] + h.huge[i] + carryDigit;
				sum.insert(sum.begin(),(sumDigit%10));
				carryDigit = sumDigit/10;

				i--;
				j--;
			}

			while(i>=0){
				sumDigit = h.huge[i] + carryDigit;
				sum.insert(sum.begin(),(sumDigit%10));
				carryDigit = sumDigit/10;

				i--;
			}

			if(carryDigit == 1){
				sum.insert(sum.begin(),(carryDigit));
			}

		}
		// --------------------------------------------------------------------------------------------------------------


		// we need to use our new constructor, because at the moment resultVector is not actually a hugeInt
		// it is simply a locally defined vector
		HugeInteger resultVector = HugeInteger(sum);

		// since we had the exact same logic for our +/+ and -/- addition, we need to check the signs of the
		// hugeInts that we were just working with, if they both had negative signs, then we set the
		// sign of the resultVector to a -1, and vice versa for if they were both positive
		if (this->isNegative == 1 && h.isNegative == 1){
			resultVector.isNegative = 1;
		}
		else if (this->isNegative == -1 && h.isNegative == -1){
					resultVector.isNegative = -1;
		}
		return resultVector;
	}
	// -- END OF +/+ AND -/- ADDITION BLOCK

	// this code now implements
	else if(this->isNegative == -1 && h.isNegative == 1){
		// have not really planned this out yet, i think i would just call the subtract method when done though??
		// anyways, its lab 2 stuff anyways
	}

}
// *******************************************************************************************************


HugeInteger HugeInteger::subtract(const HugeInteger& h) {
	// TODO
	return HugeInteger("");
}

HugeInteger HugeInteger::multiply(const HugeInteger& h) {
	// TODO
	return HugeInteger("");
}

int HugeInteger::compareTo(const HugeInteger& h) {
	// TODO
	return 0;
}

// *******************************************************************************************************

std::string HugeInteger::toString() const{

	// first we will define our output string, and retrieve the size of the huge integer that we are converint
	string str = "";
	int n = huge.size();

	/* we first need to check if our value is a negative number, if it is a negative (isNegative is 1)
	 * then we first need to append a negative sign to the string
	*/
	if(isNegative == 1){
		str.push_back('-');
	}

	/* once we have added the negative sign (if necessary) we can iterate through the resultVectort of the huge int
	 * and append the remaining characters to the string
	 * note that we add '0' to huge[i] to convert the actual integer to a character (adding 48; int -> char)
	 */
	for(int i = 0; i < n; i++){
		str.push_back(huge[i]+'0');
	}

	return str;
}

// *******************************************************************************************************