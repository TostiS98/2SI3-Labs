// Lab 3 - Polynomials
// Code to implement the Poly class
// Written by Stefan Tosti (400367761) and Yash Bhatia (400362372)
// 8 March 2023

#include "Poly.h"

Poly::Poly()
{
	// This constructor should create the zero polynomial with degree = -1
	// PolyNode has parameters (degree, coefficient, next)

	// space complexity: O(1)
	// time complexity: O(1)
	head = new PolyNode(-1, 0, NULL);
}

Poly::Poly(const std::vector<int>& deg, const std::vector<double>& coeff)
{
	// This constructor creates a new polynomial based off of the given paramaters
	// deg contains the degrees of the non-negative terms
	// coeff contains the coefficients of the non-negative terms
	// these vectors are the same size, and are sorted in DECREASING order
	// coeff [j] is the coeff of the term with degree equal to deg [j]

	// we first define the head of the polynomial by making a PolyNode with degree -1 and 0 coeff
	// then, we define a PolyNode pointer to that head that we just created
	// for every element in degree, we create a new node with degree = deg[i], and coefficient = coeff[i]
	// we make our pointers next value point to the newest created node
	// we then set pointer to pointer next

	// space complexity: O(n)
	// time complexity: O(n)
	head = new PolyNode(-1, 0, NULL);
	PolyNode* pointer = head;
	for (int i = 0; i < deg.size(); i++) {
		PolyNode* newestNode = new PolyNode(deg[i], coeff[i], NULL);
		pointer->next = newestNode;
		pointer = pointer->next;
	}
}

Poly::~Poly()
{
	// this is simply the destructor of the Poly class, which should free all of the memory

	// we start with 2 pointers to the head of the linked list
	// we loop such that pointer -> next is not NULL, pointer -> next will be NULL when we are at the last element in the LL
	// if we are not at the last element, we increment pointer, delete the temporary pointer, and then
	// again move tempPointer to where pointer is
	// once we have successfully looped through the entire LL, we can delete pointer, since we don't have anything left
	// in the LL, we don't have to worry about losing access to that element

	// space complexity: O(1)
	// time complexity: O(n)
	PolyNode* pointer = head;
	PolyNode* tempPointer = head;

	while (pointer->next != NULL) {
		pointer = pointer->next;
		delete tempPointer;
		tempPointer = pointer;
	}

	delete pointer;
}

void Poly::addMono(int i, double c)
{
	// this method adds some arbitrary monomial, cX^i to the current polynomial
	// i is non-negative, and C CAN be 0

	// space complexity is O(1) since we are not creating any new nodes
	// time complexity is O(n) since we have to iterate through the entire linked list

	// we first check if the coeffeicent is 0! This is an easy case, because the monomial is non-existant if c = 0
	// in this case, we can simply return from the function, no changes need to be made to the polynomial
	if (c == 0) {
		return;
	}

	PolyNode* pointer = head;
	PolyNode* tempPointer;

	// we will iterate through all of the elements in our linked list

	while (pointer->next != NULL) {

		// if we have the scenario where i is equal to one of the degrees of our polynomial, then we will not need to insert a new node
		// in this case, all we need to do is add the value of C to that nodes coeff
		// in the case where the addition of C to that new node makes that node have a 0 coeffeicent, we delete that node by moving temp
		// to pointer -> next, then moving pointer 2 spots ahead, and then deleting temp (deleting the node with the 0 coeff)
		if (i == pointer->next->deg) {
			pointer->next->coeff = pointer->next->coeff + c;

			if (pointer->next->coeff == 0) {
				tempPointer = pointer->next;
				pointer->next = pointer->next->next;
				delete tempPointer;
			}
			return;
		}

		// this second case scenarion occurs when the next node that we visit is larger than the value of i that we are given
		// this means that there is NOT an existing node with the same degree as the one we want to insert,
		// thus, we have to create a new node and insert that in the position of pointer -> next so that it comes BEFORE
		// the old pointer-> next

		else if (i > pointer->next->deg) {
			PolyNode* newNode = new PolyNode(i, c, pointer->next);
			pointer->next = newNode;
			return;
		}
		pointer = pointer->next;
	}

	// the third and final case scenario will occur when none of the above conditions are met
	// if we reach this point, then it means the degree of the monomial we are trying to insert is the smallest in the polynomial
	// thus, we have to create a new node at the end, and make it point to NULL
	pointer->next = new PolyNode(i, c, NULL);
	return;

}

void Poly::addPoly(const Poly& p)
{
	// This method should adda a polynomial, P, to this polynomial, but not modify p

	// to help in the implementation of this function, we can use out previously defined addMono function
	// recall that the add mono function accepts a coeffeicent (c) and an exponent (i) in the arguements
	// this, we can simply iterate through every node in p, each time passing in the degree and coeffeicent of our current node

	// space complexity: O(m) since we are creating m new nodes
	// time complexity: O(n^2) since add mono is O(n), and we are calling it n times
	PolyNode* pointer = p.head;
	while (pointer->next != NULL) {
		addMono(pointer->next->deg, pointer->next->coeff);
		pointer = pointer->next;
	}
}

void Poly::multiplyMono(int i, double c)
{
	// this method modified this polynomial by multiplying by a monomial with exponent i and coeffeicent C
	// we can assume that i is non negative, but c can be zero

	// space complexity: O(1) since no new nodes are created
	// time complexity: O(n) since we are iterating through the entire linked list

	PolyNode* pointer = head;

	// we can first consider the case where we are multiplying by a zero
	// in this case, we can simply remove all the nodes
	// we will want to leave the head, and point that to NULL since we are implementing a dummy header
	if (c == 0 && getDegree() != -1) {
		pointer = pointer->next;
		PolyNode* tempPointer = pointer;
		while (pointer->next != NULL) {
			pointer = pointer->next;
			delete tempPointer;
			tempPointer = pointer;
		}
		delete pointer;
		head->next = NULL;
		return;
	}

	// the other case scenario is we are multiplying by some non-zero monomial
	// in that case, we can iterate through the LL, adding i to the degree, and multyplying all coeffeicents by C
	while (pointer->next != NULL) {
		pointer->next->deg = pointer->next->deg + i;
		pointer->next->coeff = (pointer->next->coeff) * (c);
		pointer = pointer->next;
	}
}

void Poly::multiplyPoly(const Poly& p)
{
	// This method should multiply this polynomial, by a given polynomial, P

	// space complexity: O(n^2) since we are creating a new polynomial, and a temporary polynomial n times
	// time complexity: O(n^3) since we are calling add poly, which is O(n^2), and we are calling it n times
	PolyNode* pointer = p.head;
	Poly* mult = new Poly();
	duplicate(*mult);
	Poly* tempPoly = new Poly();
	Poly* finalProduct = new Poly();

	// we will first consider the case where either this polynomia or P is 0
	// in this case, we can simply set this polynomial to 0
	// since we are using a dummy header, we can simply set the next pointer to NULL
	// cannot use p.getDegree() because it is a const function, and we need to modify the polynomial

	if (getDegree() == -1 || pointer->next == NULL) {
		head->next = NULL;
		return;
	}

	// if we think about how multiplying two polynomials would work by hand, we are basically multiplying each monomial in P
	// by each monomial in this, and then we add all of those polynomials together
	// to accomplish this, we can use out multiplyMono method, and our addPolyMethod
	// for each node in the temporary polynomial we created, we call multiplyMono to multiply the given term in P
	// by each team in this polynomial. That is saved in the temp polynomial.
	// we then take this temporary polynomial, and add it to the finalProduct polynomial
	// each time we are adding some multiple of the origional 'this' polynomial, thus giving us the final product
	while (pointer->next != NULL) {
		mult->duplicate(*tempPoly);
		tempPoly->multiplyMono(pointer->next->deg, pointer->next->coeff);
		finalProduct->addPoly(*tempPoly);
		pointer = pointer->next;
	}

	finalProduct->duplicate(*this);



}

void Poly::duplicate(Poly& outputPoly)
{
	// this function should duplicate this polynomial to outputPoly
	PolyNode* pointerThis = head;
	PolyNode* pointerOut = outputPoly.head;
	PolyNode* tempPointer;

	// we need to iterate through all of the nodes in This poynomial
	// for each node, we will check if there exists a node in the output polynomial
	// if a node already exists, then we can simply set the desires degree and coeffeicent
	// if a node does not exist already, we have to create a new node and make it point to NULL

	// space complexity: O(n) since we are creating a new polynomial
	// time complexity: O(n) since we are iterating through the entire linked list

	while (pointerThis->next != NULL) {
		if (pointerOut->next != NULL) {
			pointerOut->next->coeff = pointerThis->next->coeff;
			pointerOut->next->deg = pointerThis->next->deg;
		}

		else {
			pointerOut->next = new PolyNode(pointerThis->next->deg, pointerThis->next->coeff, NULL);
		}
		pointerThis = pointerThis->next;
		pointerOut = pointerOut->next;
	}
}

int Poly::getDegree()
{
	// this function should simply return the degree of the polynomial

	// space complexity: O(1) since no new nodes are created
	// time complexity: O(1) since we are simply returning the degree of the first node in the linked list

	if (head->next == NULL) {
		return head->deg;
	}

	else {
		return head->next->deg;
	}
}

int Poly::getTermsNo()
{
	// this function should return the number of non zero terms in the polynomial

	// space complexity: O(1) since no new nodes are created
	// time complexity: O(n) since we are iterating through the entire linked list

	PolyNode* pointer = head->next;
	int numOfTerms = 0;
	while (pointer != NULL) {
		numOfTerms = numOfTerms + 1;
		pointer = pointer->next;
	}

	return numOfTerms;
}

double Poly::evaluate(double x)
{
	// This function should evaluate this polynomial at the given value of X (I.e. plug x into the polynomial)
	double finalSum = 0;
	double term = 1;
	PolyNode* pointer = head->next;

	// in order to implement this evaluation method, we first iterate through all of the nodes in this poly
	// in this case we do not want to start at the dummy node, so we start at head -> next
	// we first evaluate the exponent on x, this is a simple for loop that runs a nuber of times
	// equal to the degree of the monomial in that specific term, each time we multiply the term by x
	// we then multiply x^i by the coeffeicent from that specific node
	// finally, we increment the sum and move to the next node

	// space complexity is O(1) since we are not creating any new nodes
	// time complexity: for the for loop, we are multiplying x by itself 'deg' times, and the deg goes n, n-1, .... 1 therefore the time complexity is O(n^2)

	while (pointer != NULL) {
		term = 1;
		for (int i = 0; i < pointer->deg; i++) {
			term = term * x;
		}
		term = term * pointer->coeff;
		finalSum = finalSum + term;
		pointer = pointer->next;
	}
	return finalSum;

}

std::string Poly::toString()
{
	// Suppose we have the given Polynomial... P(x) = 4x^3 + 5x + 2
	// the output of this function should then be "degree = 3; a(3) = 4.0; a(1) = 5.0; a(0) = 2.0"

	// we first define an empty string to hold our output string
	// then we get a pointer to the head of the polynomial
	// we start forming the output with 'degree=' and using the to_string function with out getDegree method
	// we can check to see if we have an empty linked list, and print our final statement directly from that
	// otherwise, we want to iterate through all items in the linked list, adding them to our output statement

	// space complexity: O(n) as the string gets longer as the polynomial gets larger
	// time complexity: O(n) as we iterate through all of the nodes in the polynomial

	std::string output = "";
	PolyNode* pointer = head->next;
	output = output + "degree=" + std::to_string(getDegree()) + ";";

	if (pointer == NULL) {
		output = output + " a(-1)=0;";
	}

	else {
		for (int i = 0; i < getTermsNo(); i++) {
			output = output + " a(" + std::to_string(pointer->deg) + ")=" + std::to_string(pointer->coeff) + ";";
			pointer = pointer->next;
		}
	}

	return output;
}








