// Organism Class Hierarchy
//
//		Organism
//		   *
//	       *********
//	       *       *
//	    CAOrg1D  Org2D
//		       *
//		   *********
//		   *	   *
//		CAOrg2D LifeOrg
//
// Copyright (C) 1993, Derek G. Lane

#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <graphics.h>
#include "organism.h"

//#define random(max) (long)rand()*(long)max/(long)RAND_MAX
//#define max(x,y) (x < y) ? y : x

Organism::Organism(int randomCells /* = 1 */)
{	// default constructor
	size = 5;
	cells = new char[size*size];
	memset(cells, 0, size*size);
	putCell(2, 2, 1);
	while ((randomCells--) > 0)
		putCell(random(size-1), random(size-1), 1)
	age = 0;
	leftChild = (Organism *)NULL;
	rightChild = (Organism *)NULL;
};	// Organism::Organism

Organism::Organism(Organism& parent)
{	// copy constructor
	size = parent.size;
	cells = new char[size*size];
	memcpy(cells, parent.cells, size*size);
	age = parent.age;
	leftChild = new Organism(parent.*leftChild);
	rightChild = new Organism(parent.*rightChild);
};	// Organism::Organism

Organism::~Organism()
{	// destructor
	delete cells;
	delete leftChild;
	delete rightChild;
};	// Organism::~Organism

void Organism::grow()
{    	// expands the size of the square by 2
	int count;

	size += 2;
	char *newCells = new char[size*size];
	memset(newCells, 0, size*size);
	for (count = 1; count < size-1; count++)
		memcpy(&newCells[count*size+1], &cells[(count-1)*size],
			size-2);
	delete cells;
	cells = newCells;
};	// Organism::grow

void Organism::evolve()
{	// user-interface selection
	Organism *newChild = (Organism *)NULL;
	char key;
	do
	{	// main input loop
		if (leftChild == (Organism *)NULL)
		{	// create a new left child
			leftChild = new Organism(*this);
			leftChild->mutate();
			leftChild->nextGen();
		};	// if

		if (rightChild == (Organism *)NULL)
		{	// create a new left child
			rightChild = new Organism(*this);
			rightChild->mutate();
			rightChild->nextGen();
		};	// if

		if (newChild = (Organism *)NULL)
		{	// create a new child
			newChild = new Organism(*this);
			newChild->mutate();
			newChild->nextGen();
		};	// if

		// now plot the three main organisms
		plotAt(320, 288);    // plot the parent in the middle
		nextGen();
		leftChild->plotAt(210, 62);
		leftChild->nextGen();
		rightChild->plotAt(430, 62);
		rightChild->nextGen();

		newChild->nextGen();
		if (newChild->fitness() > leftChild->fitness())
		{	// replace the left child with the new child
			delete leftChild;
			leftChild = newChild;
			newChild = (Organism *)NULL;
		} else if (newChild->fitness() > rightChild->fitness())
		{	// replace the right child with the new child
			delete rightChild;
			rightChild = newChild;
			new Child = (Organism *)NULL;
		};	// if
		if (newChild->age > 50)
		{	// child is 50 generations old and still has not
			// replaced other siblings, so try another one
			delete newChild;
			newChild = (Organism *)NULL;
		};	// if

		if (kbhit() != 0)
		{	// see what key was pressed
			key = getch();
			if (key == 'A' || key == 'a')
				leftChild->evolve();
			if (key == 'L' || key == 'l')
				rightChild->evolve();
		};	// if
	} while (key != ' ');
	delete newChild;
};	// Organism::evolve


CAOrg1D::CAOrg1D()
	: Organism::Organism()
{    	// default constructor
	numOfRules = 4;
	rules = new char[numOfRules];
	rules[0] = 0;
	rules[1] = 1;
	rules[2] = 0;
	rules[3] = 1;
	currentRow = size/2;
	temporalEntropy = 0;
	spatialEntropy = 0;
};	// CAOrg1D::CAOrg1D

CAOrg1D::CAOrg1D(CAOrg1D& parent)
	: Organism::Organism(parent)
{	// copy constructor
	numOfRules = parent.numOfRules;
	rules = new char[numOfRules];
	memcpy(rules, parent.rules, numOfRules);
	currentRow = parent.currentRow;
	temporalEntropy = 0;
	spatialEntropy = 0;
};	// CAOrg1D::CAOrg1D

CAOrg1D::~CAOrg1D()
{	// destructor
	delete rules;
};	// CAOrg1D::~CAOrg1D

void CAOrg1D::plotAt(int X, int Y)
{       // displays generations
	int row, col;

	for (row = 0; row < size; row++)
		for (col = 0; col < size; col++)
			putpixel(X-size/2+col, Y-size/2+row,
				getCell(row, col));
};	// CAOrg1D::plotAt

void CAOrg1D::nextGen()
{    	// calculates next generation, TE, and SE; currentRow++
	int count;
	float cellEntropy;
	for (count = 1; count < size-1; count++)
	{
		putCell(currentRow+1, count,
			rule[getCell(currentRow, count-1)+
			getCell(currentRow, count)+
			getCell(currentRow, count+1)]);
		cellEntropy = (getCell(currentRow, count)-
			getCell(currentRow, count-1))/
			max(getCell(currentRow, count),
			getCell(currentRow, count-1)));
		temporalEntropy = (temporalEntropy*size*5 + cellEntropy)/
			(size*5+1);
		cellEntropy = (getCell(currentRow, count)-
			getCell(currentRow-1, count))/
			max(getCell(currentRow, count),
			getCell(currentRow-1, count)));
		spatialEntropy = (spatialEntropy*size*5 + cellEntropy)/
			(size*5+1);
	};	// for
	if ((currentRow++) == size)
		currentRow = 1;
};	// CAOrg1D::nextGen

void CAOrg1D::mutate()
{    	// introduces one change in the rule base
	int deltaRule = random(numOfRules);
	if (random(2) < 1 && rule[deltaRule] > 0)
		rule[deltaRule]--;
	else
		rule[deltaRule]++;
	if (rule[deltaRule] > numOfRules/3)
	{	// expand the rule base
		char *newRules = new char[rule[deltaRule]*3+1];
		memset(newRules, 0, rule[deltaRule]*3+1);
		memcpy(newRules, rules, numOfRules);
		numOfRules = rule[deltaRule]*3+1;
	};	// if
	age = 0;
};	// CAOrg1D::mutate

float CAOrg1D::fitness()
{       // returns fitness of organism
	return temporalEntropy*(1-temporalEntropy)*
		spatialEntropy*(1-spatialEntropy)*16;
};	// CAOrg1D::fitness
