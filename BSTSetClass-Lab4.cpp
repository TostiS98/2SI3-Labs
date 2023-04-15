#include "BSTSet.h"
#include <iostream>
#include <vector>
using namespace std;

// Stefan Tosti - 400367761 - Tostis


// CONSTRUCTOR CODE
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
BSTSet::BSTSet()
{
	// TODO

	// an empty set will have a single node, the root, which is defined to be NULL
	// thus, our constructor will define an empty set as a single NULL root node
	root = NULL;
}

BSTSet::BSTSet(const std::vector<int>& input)
{
	// TODO, change following code after completing this function

	// this constructor should initialize the BSTSet of values in the input vector, with no repetitions
	//root = new TNode(-1, NULL, NULL);

	// if we are trying to create a BST Set from an empty vector, than we need to just initialize a NULL root
	if (input.empty() == true){
		root = NULL;
		return;
	}

	// otherwise, we will simply define the root of the tree to be the first index of the set
	// then, we will iterare through all the elements of the vetor and add them to the BSTset
		root = new TNode(input[0], NULL, NULL);
		for (int i = 1; i < input.size(); i++){
			add(input[i]);
	}
}
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************

// DESTRUCTOR CODE
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// this will be a recursive deletion function, as talked about in class
void BSTSet::RecursiveDelete(TNode* &ptr){

	// check if the pointer is null, if it is not null then we are not at a leaf node, and thus we want to
	// call the recursive delete function on the the left and right child of our current nodes
	if (ptr != NULL){
		RecursiveDelete(ptr->left);
		RecursiveDelete(ptr->right);
		delete (ptr);
	}
}

BSTSet::~BSTSet()
{
	RecursiveDelete(root);
}

// ************************************************************************************************************************************************
// ************************************************************************************************************************************************


// IS IN CODE
// Average time complexity - O(logn)
// Worst time complexity - O(n) (degenerate tree)
// Space complexity - O(1)
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
bool BSTSet::isIn (int v, TNode* &ptr){

	// if the pointer to the root is null, then we know we have an empty BSTSet and thus we can immediately return false
	if (ptr == NULL){
		return false;
	}

	// if v is less than element, then we can call isIn with the left subtree and a pointer to the left node
	else if (v < ptr -> element){
		return isIn(v, ptr -> left);
	}

	// if v is greater than the element, then we can call isIn with the right subtree and a pointer to the right node
	else if (v > ptr -> element){
		return isIn(v, ptr -> right);
	}

	else {
		return true;
	}
}

bool BSTSet::isIn(int v)
{
	// TODO
    return isIn(v, root);
}

// ************************************************************************************************************************************************
// ************************************************************************************************************************************************


// ADD CODE
// Average time complexity - O(logn)
// Worst time complexity - O(n) (degenerate tree)
// Space complexity - O(1)
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// this will call our newly implemented add function
void BSTSet::add(int v)
{
	add(v, root);
}

void BSTSet::add (int v, TNode* &ptr){

	// this is our base case, once we have gotten here, we need to create a new node in our tree, as we have found the correct location
	if (ptr == NULL){
		ptr = new TNode (v, NULL, NULL);
	}

	// if our element is less than the current element, go left and call add on the left sub tree
	else if (v < ptr->element){
		add (v, ptr->left);
	}
	// if our element is greater than the current element, go right and call add on the right subtree
	else if (v > ptr->element){
		add(v, ptr->right);
	}

	// if we have gotten here, that means we have duplicates (not greater than or less than) and thus we need do nothing (no repeats in BSTSet)
	else {
		return;
	}
}
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************

// REMOVE CODE
// Average time complexity - O(logn)
// worst time complexity - O(n) degenerate tree
// Space complexity - O(1)
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
bool BSTSet::remove (int v, TNode* &ptr){

	// return false if the given v is not an element of the BST set
	if (ptr == NULL){
		return false;
	}

	// recursively remove left if we are less than current node
	if (v < ptr -> element){
		return remove(v, ptr->left);
	}

	// recursively remove right if we are greater than the current node
	else if (v > ptr -> element){
		return remove (v, ptr->right);
	}

	// once we find the node that we are looking for, we need to remove it and then replace it accordingly
	// we have 2 options when replacing. We can either replace with the largets element on the left or the smaller element on the right
	// I could not figure out how to implement that for my lab demo, but i know how i would do it theoretically

}
bool BSTSet::remove(int v)
{
	// TODO
    return remove(v, root);
}
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************


// UNION CODE
// average time complexity - O(n^2)
// worst time complexity - O(n^2)
// space complexity - O(1)
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
void BSTSet::Union(const BSTSet& s, TNode* ptr){
	// if we get a null pointer then we can return right away
	if (ptr == NULL){
		return;
	}

	// check each current node in S is in the BSTSet
	// if it is not, then we call our add function to add it to the BSTSet
	else if (isIn(ptr->element) == false){
		add(ptr->element);
	}

	// we then call union with each node, right and left
	Union (s, ptr->left);
	Union (s, ptr->right);
}
void BSTSet::Union(const BSTSet& s)
{
	Union(s, s.root);
}
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************

// INTERSECTION CODE
// average time complexity - O(n^2)
// worst time complexity - O(n^2)
// space complexity - O(n);
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
void BSTSet::intersection(TNode* ptr, BSTSet* temp){

	// if we get a null pointer, we can immediately return
	if (ptr == NULL){
		return;
	}

	// otherwise, we check if the element is within our bstSet, and if so, we recursively call intersection with the left and right nodes
	// union wants to add everything that is in BOTH
	if (isIn(ptr -> element) == true){
		temp -> add(ptr -> element);
	}
	intersection (ptr->left, temp);
	intersection (ptr->right, temp);

}
void BSTSet::intersection(const BSTSet& s)
{

	// the fact that we make a new, temporary BST set makes this function have n space complexity
	// the space required is dependent on the number (n) nodes that we want to insert into our unionized BSTset
	BSTSet* temp = new BSTSet();
	intersection (s.root, temp);
	RecursiveDelete(root);
	root = temp->root;

}
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************


// DIFFERENCE CODE
// average time complexity - O(n^2)
// worst time complexity - O(n^2)
// space complexity - O(1)
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// difference should tell us the things are that in BSTset but are NOT in s
void BSTSet::difference(const BSTSet& s)
{
	difference (s.root);
}

void BSTSet::difference(TNode* ptr){
	if (ptr == NULL){
		return;
	}

	else {
		difference (ptr -> left);
		remove (ptr -> element);
		difference (ptr -> right);
	}
}
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************

// SIZE CODE
// average time complexity - O(n)
// worst time complexity - O(n)
// space complexity - O(1)
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************

int BSTSet::size()
{

	int digits = 0;
	return size (digits, root);
}

int BSTSet::size(int& total, TNode* ptr){
	if (ptr == NULL){
		return total;
	}

	else {
		total = size (total, ptr -> left);
		total = size (total, ptr -> right);
		total = total + 1;
		return (total);
	}
}
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************

// HEIGHT CODE
// average time complexity - O(logn)
// worst time complexity - O(n)
// space complexity - O(1)
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
int BSTSet::height()
{
    return height(root);
}

int BSTSet::height(TNode* ptr){

	if (ptr == NULL){
		return (-1);
	}

	int rightHeight = height(ptr->right);
	int leftHeight = height(ptr->left);
	return max(rightHeight, leftHeight)+1;
}
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************

// MY STACK CLASS CODE
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
class SNode {
	public:
		TNode* node;
		SNode* next;

	SNode(SNode* n, TNode* m){
		node = m;
		next = n;
	}
};
class MyStack {
	private:
		SNode* head;
	public:
		MyStack(){
			head = NULL;
		}

		bool isEmpty(){
			if (head == NULL){
				return true;
			}
			else {
				return false;
			}
		}

		TNode* pop() {
			if (head != NULL){
				SNode* temp = head;
				TNode* output = head->node;
				head = head->next;
				delete temp;
				return output;
			}
			else {
				throw underflow_error("underflow LOLZ :P");
			}
		}

		void push(TNode* n){
			head = new SNode (head, n);
		}

};
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************


// create and use class MyStack
// average time complexity - O(n)
// worst time complexity - O(n)
// space comeplxity - O(n)

// **This is the implementation of the algorithm that was given in the class slides
void BSTSet::printNonRec()
{
	// TODO
	// start with an empty stack
	MyStack stack = MyStack();
	TNode* ptr = root;

	// for all of the nodes while the stack is not empty
	while(stack.isEmpty() == false || ptr != NULL){
		// if the pointer is not null, push the pointer onto the stack, and go to the left child
		if(ptr!=NULL){
			stack.push(ptr);
			ptr = ptr->left;
		}

		// if the current node is null and the stack is not empty, pop the node, print the element, and then go to the right
		else{
			ptr = stack.pop();
			cout<<ptr->element<<",";
			ptr = ptr->right;
		}
	}
}



// Do not modify following provided methods

void BSTSet::printBSTSet()
{
    if (root == NULL)
        std::cout << "";
    else {
        printBSTSet(root);
    }
}

void BSTSet::printBSTSet(TNode* t)
{
    if (t != NULL) {
        printBSTSet(t->left);
        std::cout << t->element << ",";
        printBSTSet(t->right);
    }
}