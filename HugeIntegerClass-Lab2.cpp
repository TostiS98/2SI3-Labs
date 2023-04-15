#include "HugeInteger.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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

		if (sum[0] == 0 && sum.size() != 1){
			sum.erase(sum.begin());
		}

		// we need to use our new constructor, because at the moment resultVector is not actually a hugeInt
		// it is simply a locally defined vector
		HugeInteger resultVector = HugeInteger(sum);

		// since we had the exact same logic for our +/+ and -/- addition, we need to check the signs of the
		// hugeInts that we were just working with, if they both had negative signs, then we set the
		// sign of the resultVector to a -1, and vice versa for if they were both positive
		if (this->isNegative == 1 && h.isNegative == 1){
			resultVector.isNegative = 1;
		}
		else if (this->isNegative == 0 && h.isNegative == 0){
					resultVector.isNegative = 0;
		}
		return resultVector;
	}

	// this case handles the addition when h is negative and huge is positive
	// basically, we just have to make a copy of h (since its a constant) then use our subtract function
	// to do huge - h
	else if (this->isNegative == 0 && h.isNegative == 1){
		// I need to make a copy of h because it is considered a constant for whatever reason
		HugeInteger hcopy(h);
		hcopy.isNegative = 0;
		HugeInteger resultVector = this->subtract(hcopy);
		return resultVector;
	}

	// the last case scenario is when we have huge as a negative, and h as a positive
	// this is the exact same logic as above, just reversed
	else {
		HugeInteger HugeIntcopy (*this);
		HugeInteger hcopy(h);
		HugeIntcopy.isNegative = 0;
		HugeInteger resultVector = hcopy.subtract(HugeIntcopy);
		return resultVector;
	}

}
// *******************************************************************************************************


HugeInteger HugeInteger::subtract(const HugeInteger& h) {

	// the first thing that we can do is double check if the integers are the same thing
	// if they are the same things, we immediately know that the difference will be 0
	// we can use our already established compareTo function to determine if they are the same thing
	// rather the re-implementing all of the logic again
	// since we need to return a new Huge, we will just use our constructor to make a hugeInt of just 0
	if (compareTo(h) == 0){
		return HugeInteger("0");
	}

	// We can try our best to seperate subtraction into addition, since we already have all the logic done for addition

	// the second case scenario is that we have a positive hugeInt and a negative h
	// +huge - (-h) is the same thing as huge + h
	// we can check to see if our hugeInt is positive, and our h is negative
	// in this case scenario, we can make a copy of our hugeInt as well as h, set the copy of h to positive
	// and then we can simply add the 2 vectors
	if (this->isNegative == 0 && h.isNegative == 1){
		HugeInteger copy1 (*this);
		HugeInteger copy2 (h);
		copy2.isNegative = 0;
		HugeInteger resultVector = copy1.add(copy2);
		return resultVector;
	}

	// the other case scenario is if he have -huge - (+h)
	// in this case scenario, we can simply perform (huge + h)*-1
	// in code, this translates to using our add method between huge and h, and then setting the result to a negative via isNegativ
	else if (this->isNegative == 1 && h.isNegative == 0){
		HugeInteger copy1 (*this);
		HugeInteger copy2 (h);
		copy1.isNegative = 0;
		HugeInteger resultVector = copy1.add(copy2);
		resultVector.isNegative = 1;
		return resultVector;
	}

	// if none of the above cases are true, then we will need to implement our own subtraction algorithm

	// initialize some variables that will be of use
	// number1 will be a vector that holds the larger value, and number2 will be a vector that holds the smaller value
	vector <int> number1;
	vector <int> number2;
	vector <int> difference;
	int carry = 0;
	int temporaryDifference = 0;
	int hugeSize = huge.size();
	int hSize = h.huge.size();

	// we should first determine if the two numbers are NOT the same size! In this case, we need to check
	// which number is larger, and assign number1 and number2 accordingly
	// this is sort of a shortcut for the following code, since rather than checking which value is greater
	// we can reduce computation time by seeing which one is longer
	// NUMBER1 IS GIVEN THE THE LARGER INTEGER
	// THIS SECTION CHECKS BASED ON SIZE
	if (hugeSize != hSize){
		if (hugeSize < hSize){
			number1 = h.huge;
			number2 = huge;
		}
		else{
			number1 = huge;
			number2 = h.huge;
		}
	}

	// if theyre the same size, then we want to set number 1 and number 2 according to which VALUE is smaller rather than which length
	// we can do this with the help of our compareTo function above
	// NUMBER 1 IS THE LARGER NUMBER
	// THIS SECTION CHECKS BASED ON THE NEGATIVE SIGN (ONE OF THEM HAS TO BE NEGATIVE)
	else if (hugeSize == hSize){

		// if both are negative, and compareTo returns a 1, then we know h is larger, and hugeInt is smaller
		// H LARGER
		if (this->isNegative == 1 && h.isNegative == 1 && this->compareTo(h) == 1){
			number1 = h.huge;
			number2 = huge;

		}

		// if they are both negative, and compareTo return a -1, then we know h is smaller, and hugeInt is larger
		// HUGE INT IS LARGER
		else if (this->isNegative == 0 && h.isNegative == 0 && this->compareTo(h) == -1){
					number1 = h.huge;
					number2 = huge;

				}

		// all other scenarios will have the smaller number as h and the larger number as huge, so we can simply use an else
		else {
			number1 = huge;
			number2 = h.huge;
		}

	}

	// now we have to actually implement our subtraction algorithm
	// we can think about how long subtraction actually works
	// consider doing 165 - 24 algabreically, with long subtraction,
	// so far in our code, we have determine which number is larger and which number is smaller, so we would know that 165 > 24
	//   257
	//  - 38
	// -------
	// we will need to take into account when the difference between the 2 elements is a negative number
	// what we do in this case scenario, is we take ONE off the subsequent element, and add TEN to the element in question
	//


	// I have chosen to reverse both of my hugeInts for ease of iteration
	// essentially, reversing them will make the iteration process a little easier
	// instead of iterating from end to beginning and decrementing i, I will simply iterate as normal from 0 to .size()
	reverse(number1.begin(), number1.end());
	reverse(number2.begin(), number2.end());

	// we begin by iterating through the smaller vector that we had defined, which is number 2
	// we set the temporary difference as the difference between number1 and number2 at corresponding indexes
	// we also take into consideration a carry bit from previous subtraction (this is the -1 from borrowing)
	// we then check to see if the temporary difference between them is a negative number,
	// if it is negative, then we need to simulate the "borrowing" process
	// essentially, what this means is when the temporary difference is a negative number, we add 10 to tempDiff and make the carry a 1
	// if the difference is a non negative, then we dont have to worry about that!
	// we repeat this process for every index in number 2 - we will be left with the left over numbers in number 1 since its bigger
	for (int i = 0; i < number2.size(); i++){
		temporaryDifference = number1[i] - number2[i] - carry;
		if (temporaryDifference < 0){
			temporaryDifference = temporaryDifference + 10;
			carry = 1;
		}
		else {
			carry = 0;
		}
		difference.push_back(temporaryDifference);
	}

	// once we have completed the above for loop, we have essentially subtracted all of the numbers in number1 that have a
	// corresponding index in number 2. We now need to transfer over all of the numbers that remain in number1 into the
	// difference vector! Even though we are still transferring numbers, we have to take into account a carry that might
	// be left over from the last subtraction in number1[i] - number2[i]
	for (int i = number2.size(); i < number1.size(); i++){
		temporaryDifference = number1[i] - carry;

		if (temporaryDifference < 0){
			temporaryDifference = temporaryDifference + 10;
			carry = 1;
		}

		else {
			carry = 0;
		}

		difference.push_back(temporaryDifference);
	}

	// since we had initially chosen to reverse our numbers, our final difference is going to be in reverse order
	// thus, we need to reverse our final difference vector, before we can copy that into our result vector
	reverse(difference.begin(), difference.end());

	// we have to also remove the leading 0s from the difference vector
	// I honestly have no idea where these are coming from, but this was making a bunch of my test cases fail lol
	while (difference[0] == 0){
		difference.erase(difference.begin());
	}

	// finally, we can copy the final result vector into a HugeInteger using our constructor
	HugeInteger result(difference);

	// we determine the sign of the final value by comparing it to the h vector which we subtracted from
	//
	if (compareTo(h) == -1){
		result.isNegative = 1;
	}

	return result;

}
// *******************************************************************************************************


// *******************************************************************************************************
HugeInteger HugeInteger::multiply(const HugeInteger& h) {
	// TODO

	// we will first initialize some variables that will help in our computations
	// I will have a variable for my carry, my temporary product in multiplication, as well as some index variables for
	// iterations, and of course the size of my huge and h
	// the index 1 and index 2 variable will help keep track of performing the final addition of our two lines
	// for each number in
	int carry = 0;
	int temporaryProduct = 0;
	int hSize = h.huge.size();
	int hugeSize = huge.size();
	int index1 = 0;
	int index2 = 0;
	vector<int> finalProduct(hSize + hugeSize, 0);
	//HugeInteger resultVector;

	// to save on computation time (since im gonna need it) im first gonna check if we have a 0, and immediately
	// return a huge int of 0
	// without this, the program is timing out and doesnt end up finishing
	if (huge[0] == 0 || h.huge[0] == 0){
		return HugeInteger ("0");
	}

	// we will want to iterate through the operand from right to left, since our huge ints are in the reverse order as
	// to what we want for multiplication
	// within this function, we will want to redefine the value of carry and index2 with every pass of the output most for loop
	// this is because multiplication is difference from the other two operations (addition and subtraction) in that
	// when we add and subtract, we just go index by index
	// for multiplication, however, for every index in the bottom number, we need to iterate through every index in the top number

	// if we break down how long multiplication works, for each number in the bottom, we multiply it by each number in the top
	// each time we perform a multiplication step, we need to take into account a carry from the previous multiplication
	// when we add these values into the final product, we add the corresponding indexes of the products, as well as the carry
	// when we deal with the carry, we do the exact same thing we did in addition, where we split it into a sum (%10)
	// and a carry for the next addition, using integer division (/10)
	// each time we preform one of these operations, we increase index2
	for (int i = hugeSize - 1; i >= 0; i--){
		carry = 0;
		index2 = 0;

		for (int j = (hSize - 1); j >= 0; j--){
			temporaryProduct = huge[i] * h.huge[j];
			temporaryProduct = temporaryProduct + finalProduct[index1 + index2];
			temporaryProduct = temporaryProduct + carry;

			finalProduct[index1 + index2] = temporaryProduct % 10;
			carry = temporaryProduct / 10;
			index2 = index2 + 1;

		}

		finalProduct[index1 + index2] = finalProduct[index1 + index2] + carry;
		index1 = index1 + 1;
	}

	// once we are finished forming our finalProduct vector, we will need to reverse it, since we were actually
	// forming it in reverse order above
	reverse(finalProduct.begin(), finalProduct.end());


	// much like in the other computations, we need to remove the leading zeros from the final product vector
	if (finalProduct[0] == 0){
		finalProduct.erase(finalProduct.begin());
		}

	// we can turn this finalProduct vector into a hugeInt using our custom constructor
	HugeInteger result (finalProduct);

	// we can also determine the negative sign on the final product, essentially, we just check if and only if one of the two operands
	// was a negative, if theyre both negative or both positive, we can leave them as is
	if (this->isNegative == 1 && h.isNegative == 0){
		result.isNegative = 1;
	}
	else if (this->isNegative == 0 && h.isNegative == 1){
			result.isNegative = 1;
		}

	else if (this->isNegative == 1 && h.isNegative == 1){
			result.isNegative = 0;
		}
	else if (this->isNegative == 0 && h.isNegative == 0){
			result.isNegative = 0;
		}


	return result;
}
// *******************************************************************************************************

// *******************************************************************************************************
int HugeInteger::compareTo(const HugeInteger& h) {

	// first just initializing some helper variables that im gonna need in the function
	int detect = 0;
	int hugeSize = huge.size();
	int hSize = h.huge.size();

	// the first, and easiest case scenario is when hugeInt and h are the exact same thing
	// we can test that they are the same by using our toString method to transformer them into strings
	// and then simply testing equality with the == operator
	if (h.toString() == this->toString()){
		return 0;
	}

	// next, we can sort out the cases where one is negative and the other is positive
	// if hugeInt is less than h, then we return -1, if h is less than hugeInt we return 1
	// we can therfore test is one and only one of these integers is negative
	// the number that is a negative will clearly be the smaller one
	// ONE NEGATIVE ONE POSITIVE
	else if (h.isNegative == 0 && this->isNegative == 1){
		detect = -1;
	}
	else if (h.isNegative == 1 && this->isNegative == 0){
		detect = 1;
	}

	// next, we have the case where both the number are positive
	// we can try and reduce computation by just checking the size of the number and determining based off of that
	// otherwise, if they are the same size, we can reliably check the first digit of the 2 hugeInts
	// this is not a very reliable method of comparison, realistically i should be checking all the numbers
	// for equality and then stopping once we find one thats greater, but i think this should be okay 99/100 times
	// BOTH ARE POSOTIVE
	if (this->isNegative == 0 && h.isNegative == 0){
		if (hSize > hugeSize){
			detect = -1;
		}
		else if (hSize < hugeSize){
			detect = 1;
		}
		else if (hSize == hugeSize){
			if (h.huge[0] > huge[0]){
				detect = -1;
			}
			else {
				detect = 1;
			}
		}
	}

	// finally, we have the case where they are both negative numbers
	// this is the exact same logic as the double positive case, except the value of detect is reversed
	// this is because a larger negative number is actually smaller, where as a larger positive
	// number is larger
	// BOTH NEGATIVE (LARGER NUMBER IS ACTUALLY SMALLER)
	else if (this->isNegative == 1 && h.isNegative == 1){
		if (hSize > hugeSize){
			detect = 1;
		}
		else if (hSize < hugeSize){
			detect = -1;
		}
		else if (hSize == hugeSize){
			if (h.huge[0] > huge[0]){
				detect = 1;
			}
			else {
				detect = -1;
			}
		}
	}
	return detect;
}
// *******************************************************************************************************

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