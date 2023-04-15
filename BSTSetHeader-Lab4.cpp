#ifndef BSTSET_H_
#define BSTSET_H_

#include "TNode.h"
#include <vector>
#include <cstddef>

class BSTSet
{
private:
	// must contain only one private field
	TNode* root = NULL;

public:

	BSTSet();
	BSTSet(const std::vector<int>& input);
	~BSTSet();

	// required methods
	bool isIn(int v);
	void add(int v);
	bool remove(int v);
	void Union(const BSTSet& s);
	void intersection(const BSTSet& s);
	void difference(const BSTSet& s);
	int size();
	int height();
	void printNonRec(); // create and use class MyStack

	// provided recursive print method
	void printBSTSet();

	// Used for testing
	TNode* getRoot()
	{
		return root;
	}

private:
	// provided helper methods
	void printBSTSet(TNode* t);
	void RecursiveDelete(TNode* &ptr);
	bool isIn(int v, TNode* &ptr);
	void add(int v, TNode* &ptr);
	TNode* findMax(TNode* &ptr);
	bool remove(int v, TNode* &ptr);
	void Union(const BSTSet& s, TNode* ptr);
	int size(int& tally, TNode* ptr);
	void intersection(TNode* ptr,BSTSet* temp);
	void difference(TNode* ptr);
	int height(TNode* ptr);
};

#endif /* BSTSET_H_ */