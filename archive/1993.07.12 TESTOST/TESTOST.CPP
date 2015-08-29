#define _DEBUG
#include <stdlib.h>    // include the standard C library
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <graph.h>
#include <new.h>
#include <malloc.h>
#include "testost.h"

#define max(x, y) ((x) > (y)) ? (x) : (y)
#define random(max) ((long)rand()*(max)/RAND_MAX)
    // helper macro to calculate random numbers in a particular range
#define assert(x) if (!(x)) { printf("Assertion failed: line %i of %s",  \
    __LINE__, __FILE__); exit(-1); }


struct _videoconfig vc;

//////////////////////////////////////////////////////////////////////////
// class Organism

Organism::Organism()
{   // default constructor
    age = 0;
    terminal = 1;
    leftChild = (Organism *)NULL;
    midChild = (Organism *)NULL;
    rightChild = (Organism *)NULL;
    parent = (Organism *)NULL;
};  // Organism::Organism()


Organism::Organism(Organism& fromParent)
{   // copy constructor
    age = fromParent.age;
    terminal = 1;
    leftChild = (Organism *)NULL;
    midChild = (Organism *)NULL;
    rightChild = (Organism *)NULL;
    parent = &fromParent;
};  // Organism::Organism()


Organism::~Organism()
{   // destructor
    delete leftChild;
    delete midChild;
    delete rightChild;
};  // Organism::~Organism()


void drawTree(int leftBranch, int midBranch, int rightBranch)
{   // draws a tree on the screen
    _clearscreen(_GCLEARSCREEN);
    _setcolor(15);
    _setlinestyle(0xAAAA);
    _moveto(vc.numxpixels/2, vc.numypixels);
    _lineto(vc.numxpixels/2, vc.numypixels*3/4);
    _lineto(vc.numxpixels/6, vc.numypixels/3);
    _moveto(vc.numxpixels/2, vc.numypixels*3/4);
    _lineto(vc.numxpixels/2, vc.numypixels/4);
    _moveto(vc.numxpixels/2, vc.numypixels*3/4);
    _lineto(vc.numxpixels*5/6, vc.numypixels/3);
    if (leftBranch)
    {   _moveto(vc.numxpixels/6, vc.numypixels/3);
        _lineto(vc.numxpixels/18, 0);
        _moveto(vc.numxpixels/6, vc.numypixels/3);
        _lineto(vc.numxpixels*3/18, 0);
        _moveto(vc.numxpixels/6, vc.numypixels/3);
        _lineto(vc.numxpixels*5/18, 0);
    };  // if
    if (midBranch)
    {   _moveto(vc.numxpixels/2, vc.numypixels/4);
        _lineto(vc.numxpixels*7/18, 0);
        _moveto(vc.numxpixels/2, vc.numypixels/4);
        _lineto(vc.numxpixels*9/18, 0);
        _moveto(vc.numxpixels/2, vc.numypixels/4);
        _lineto(vc.numxpixels*11/18, 0);
    };  // if
    if (rightBranch)
    {   _moveto(vc.numxpixels*5/6, vc.numypixels/3);
        _lineto(vc.numxpixels*13/18, 0);
        _moveto(vc.numxpixels*5/6, vc.numypixels/3);
        _lineto(vc.numxpixels*15/18, 0);
        _moveto(vc.numxpixels*5/6, vc.numypixels/3);
        _lineto(vc.numxpixels*17/18, 0);
    };  // if
};  // drawTree


char Organism::evolve()
{   // user-mediated evolution procedure
    char key = 0;
    drawTree(leftChild != NULL && !leftChild->terminal,
        midChild != NULL && !midChild->terminal,
        rightChild != NULL && !rightChild->terminal);
    // grow the parent and two children until a key is pressed
    // try a new child while this is happening
    Organism *newChild = spawnChild();
    assert(newChild != NULL);
    do
    {   // repeat this loop until the space key is pressed
        plotAt(vc.numxpixels/2, vc.numypixels*3/4);
        nextGen();

        if (leftChild == NULL)
            leftChild = spawnChild();    // make new left child
        else
            leftChild->plotAt(vc.numxpixels/6, vc.numypixels/3);
        assert(leftChild != NULL);
        leftChild->nextGen();

        if (midChild == NULL)
            midChild = spawnChild();    // make new mid child
        else
            midChild->plotAt(vc.numxpixels/2, vc.numypixels/4);
        assert(leftChild != NULL);
        midChild->nextGen();

        if (rightChild == NULL)
            rightChild = spawnChild();    // make new left child
        else
            rightChild->plotAt(vc.numxpixels*5/6, vc.numypixels/3);
        assert(leftChild != NULL);
        rightChild->nextGen();

        terminal = 0;    // if the node was terminal before, its not now

        if (newChild->age < 25)
            newChild->nextGen();
        else if (leftChild->terminal == 1 &&
            newChild->fitness()*0.7 > leftChild->fitness())
        {   // replace the left child with the new child
            delete leftChild;
            leftChild = newChild;
            newChild = spawnChild();
            assert(newChild != NULL);
        } else if (midChild->terminal == 1 &&
            newChild->fitness()*0.7 > midChild->fitness())
        {   // replace the left child with the new child
            delete midChild;
            midChild = newChild;
            newChild = spawnChild();
            assert(newChild != NULL);
        } else if (rightChild->terminal == 1 &&
            newChild->fitness()*0.7 > rightChild->fitness())
        {   // replace the left child with the new child
            delete rightChild;
            rightChild = newChild;
            newChild = spawnChild();
            assert(newChild != NULL);
        } else if (newChild->age > 50)
        {   // new child is 50 generations old and still has not promoted
            delete newChild;
            newChild = spawnChild();
            assert(newChild != NULL);
        } else
            newChild->nextGen();

        if (kbhit())
        {   // read the key and see what needs to be done
            key = getch();
	    // if (_stackavail() > 0x0400)
	    //{	 // only process child branch if there is enough stack space
	    //	 if (key == 'A' || key == 'a')
	    //	     leftChild->evolve();
	    //	 else if (key == 'G' || key == 'g')
	    //	     midChild->evolve();
	    //	 else if (key == 'L' || key == 'l')
	    //	     rightChild->evolve();
	    //	 drawTree(leftChild != NULL && !leftChild->terminal,
	    //	     midChild != NULL && !midChild->terminal,
	    //	     rightChild != NULL && !rightChild->terminal);
	    //};  // if
        } else
            key = 0;
    } while (key == 0);
    delete newChild;

    return key;
};  // Organism::evolve()


int Organism::free_memory(size_t sz)
{   // attempts to free sz bytes
    long mem_freed = leftChild->free_memory(sz);
    if (mem_freed < sz)
        mem_freed += midChild->free_memory(sz-mem_freed);
    if (mem_freed < sz)
        mem_freed += rightChild->free_memory(sz-mem_freed);
    return mem_freed;
};  // Organism::free_memory


//////////////////////////////////////////////////////////////////////////
// class Org1D

Org1D::Org1D()
    : Organism()
{   // default constructor
    memset(cells, 0, sizeof(cells));
    cells[0] = new char[7];
    memset(cells[0], 0, 7);
    cells[0][7/2] = 1;    // set initial cell value
    memset(size, 0, sizeof(size));
    size[0] = 7;
    temporalEntropy = 0;
    spatialEntropy = 0;
    assert(cells != NULL);
};  // Org1D::Org1D()


Org1D::Org1D(Org1D& parent)
    : Organism(parent)
{   // copy constructor
    memset(cells, 0, sizeof(cells));
    cells[0] = new char[7];
    memset(cells[0], 0, 7);
    cells[0][7/2] = 1;    // set initial cell value
    memset(size, 0, sizeof(size));
    size[0] = 7;
    //int count;
    //for (count = 0; parent.size[count] != 0; count++)
    //{   // copy the cell bodies
        //size[count] = parent.size[count];
        //cells[count] = new char[size[count]];
        //memcpy(cells[count], parent.cells[count], size[count]);
    //};  // for
    temporalEntropy = parent.temporalEntropy;
    spatialEntropy = parent.spatialEntropy;
    assert(cells != NULL);
};  // Org1D::Org1D()


Org1D::~Org1D()
{   // destructor
    int count;
    for (count = 0; count < numOfGen; count++)
        delete [] cells[count];
};  // CAOrg1D::~CAOrg1D()


void Org1D::plotAt(int X, int Y)
{   // plots generations at X, Y
    int gen, at;
    for (gen = 0; gen < numOfGen; gen++)
        for (at = 0; at < size[gen]; at++)
        {   // draw the indicated pixel
            _setcolor((vc.numcolors-1)*cells[gen][at]/maxState);
            _setpixel(X-size[gen]/2+at, Y+numOfGen/2-gen);
        };  // for
};  // Org1D::plotAt()


#define optSize 50

float Org1D::fitness()
{   // returns the fitness of the organism
    float fit = temporalEntropy*(1-temporalEntropy)*
        spatialEntropy*(1-spatialEntropy)*16;
    //int count;
    //float avSize = 0.0;
    //float deltaSize = 0.0;
    //for (count = 0; count < 10; count++)
    //{   // calculate average size and delta size over 10 generations
        //avSize = avSize+size[count];
        //deltaSize = deltaSize+(size[count]-size[count+1]);
    //};  // for
    //avSize /= 10;
    //deltaSize /= 10;
    //sizeFit = (avSize/(avSize+optSize))*(1-avSize/(avSize+optSize))*4;
    //fit *= sizeFit*(avSize/(avSize+optSize))*(2-deltaSize)
    return fit;
};  // Org1D::fitness()


int Org1D::free_memory(size_t sz)
{   // tries to free up size bytes on the heap
    int count;
    long mem_freed = 0;
    for (count = 0; cells[count] != NULL; count++)
    {   delete [] cells[count];
        mem_freed += size[count];
        size[count] = 0;
    };  // for
    if (mem_freed < sz)
        return Organism::free_memory(sz-mem_freed);
    else
        return mem_freed;
};  // free_memory


//////////////////////////////////////////////////////////////////////////
// class CAOrg1D

CAOrg1D::CAOrg1D()
    : Org1D()
{   // default constructor
    maxState = 1;
    rules = new char[maxState*3+1];
    rules[0] = 0;  rules[1] = 1;
    rules[2] = 0;  rules[3] = 1;
    assert(rules != NULL);
};  // CAOrg1D::CAOrg1D()


CAOrg1D::CAOrg1D(CAOrg1D& parent)
    : Org1D(parent)
{   // copy constructor
    maxState = parent.maxState;
    rules = new char[maxState*3+1];
    memcpy(rules, parent.rules, maxState*3+1);
    assert(rules != NULL);
};  // CAOrg1D::CAOrg1D()


CAOrg1D::~CAOrg1D()
{   // destructor
    delete [] rules;
};  // CAOrg1D::~CAOrg1D()


void CAOrg1D::nextGen()
{   // calculates next generation, temporal entropy, spatial entropy
    char *newCells;
    // normalize the existing cell body
    while (cells[0][0] != cells[0][1] ||
        cells[0][1] != cells[0][2] ||
        cells[0][size[0]-1] != cells[0][size[0]-2] ||
        cells[0][size[0]-2] != cells[0][size[0]-3])
    {   // grow if there is not a three-cell buffer on both sides
        size[0] += 2;
        newCells = new char[size[0]];
        newCells[0] = cells[0][0];
        newCells[size[0]-1] = cells[0][size[0]-3];
        memcpy(&newCells[1], cells[0], size[0]-2);
        delete [] cells[0];
        cells[0] = newCells;
        //char *newCells = new char[size[0]];
    };  // while

    int te = 0;
    int se = 0;
    int maxval = 0;
    int at;
    newCells = new char[size[0]];
    newCells[0] = rules[cells[0][0]+cells[0][1]+cells[0][2]];
    for (at = 1; at < size[0]-1; at++)
    {   // calculate for entire row
        newCells[at] = rules[cells[0][at-1]+cells[0][at]+cells[0][at+1]];
        se += abs(newCells[at-1]-newCells[at]);
        te += abs(newCells[at]-cells[0][at]);
        maxval = max(maxval, newCells[at]);
    }; // for
    newCells[size[0]-1] = rules[cells[0][size[0]-3]+
        cells[0][size[0]-2]+cells[0][size[0]-1]];
    delete [] cells[numOfGen-1];
    memmove(&cells[1], &cells[0], (numOfGen-1)*sizeof(cells[0]));
    memmove(&size[1], &size[0], (numOfGen-1)*sizeof(size[0]));
    cells[0] = newCells;

    if (maxval > 0)
    {   // add the amounts to the temporal and spatial entropy averages
        temporalEntropy = (temporalEntropy*5+(float)te/(size[0]-2)/maxval)/6;
        spatialEntropy = (spatialEntropy*5+(float)se/(size[0]-2)/maxval)/6;
    } else
    {   // otherwise, add zero to the averages
        temporalEntropy = temporalEntropy*5/6;
        spatialEntropy = spatialEntropy*5/6;
    }; // if

    age++;    // increment age counter
};  // void CAOrg1D::nextGen()


Organism *CAOrg1D::spawnChild()
{   // introduces one change in the rules
    int tempAge = age;
    age = 0;
    CAOrg1D *newChild = new CAOrg1D(*this);
    assert(newChild->rules != NULL);
    age = tempAge;
    // mutate one rule
    int ruleAt = random(newChild->maxState*3+1);
    if (newChild->rules[ruleAt] > 0 && random(2) < 1)
        newChild->rules[ruleAt]--;    // decrement the rule
    else if ((++newChild->rules[ruleAt]) > newChild->maxState)
    {   // need more space for rules
        char *newRules = new char[newChild->rules[ruleAt]*3+1];
        memset(newRules, 0, newChild->rules[ruleAt]*3+1);
        memcpy(newRules, newChild->rules, newChild->maxState*3+1);
        delete [] newChild->rules;
        newChild->rules = newRules;
        newChild->maxState = newChild->rules[ruleAt];
    }; // if
    assert(newChild->maxState >= newChild->rules[ruleAt]);
    return (Organism *)newChild;
};  // Organism *CAOrg1D::spawnChild()


//void CAOrg1D::Serialize(CArchive& archive)
//{   // Serializes the object
    //int count;
    //WORD wordNR;
    //BYTE byteRule;
    //Org1D::Serialize(archive);
    //if (archive.IsStoring())
    //{   // store the member variables
	//archive << (WORD)maxState;
	//for (count = 0; count < maxState*3+1; count++)
	    //archive << (BYTE)rules[count];
    //} else
    //{   // retrieve the member variables
	//archive >> wordNR;
	//maxState = (int)wordNR;
	//rules = new char[maxState*3+1];
	//for (count = 0; count < maxState*3+1; count++)
	//{
	    //archive >> byteRule;
	    //rules[count] = (char)byteRule;
	//};  // for
    //};  // if
//};  // CAOrg1D::Serialize


//////////////////////////////////////////////////////////////////////////
// main

CAOrg1D FoundingFather;

int free_memory(size_t size)
{   // tries to free up size bytes on the heap
    return FoundingFather.free_memory(size);
};  // free_memory

void main()
{   // initialize an organism, and then begin evolution
    // initialize video mode
    Organism *currOrg;
    char key;
    _setvideomode(_MAXRESMODE);
    _getvideoconfig(&vc);

    // set the special memory handler
    _set_new_handler(free_memory);

    // seed random number generator
    srand((unsigned)time(NULL));

    //CFile treeFile;
    //if (treeFile.Open("TREE.DAT", CFile::modeRead))
    //{   // file exists, so deserialize
	//CArchive treeArc(&treeFile, CArchive::load);
	//FoundingFather.Serialize(treeArc);
	//treeArc.Close();
	//treeFile.Close();
    //};  // if

    //FoundingFather.Dump(afxDump);
    currOrg = &FoundingFather;
    int count;
    do
    {	// evolution loop
	key = currOrg->evolve();
	for (count = 0; count < numOfGen; count++)
	{   // delete all the cell bodies
	    delete [] currOrg->cells[count];
	    delete [] currOrg->leftChild->cells[count];
	    delete [] currOrg->midChild->cells[count];
	    delete [] currOrg->rightChild->cells[count];
	};  // for
	if (key == 'A' || key == 'a')
	    currOrg = currOrg->leftChild;
	else if (key == 'G' || key == 'g')
	    currOrg = currOrg->midChild;
	else if (key == 'L' || key == 'l')
	    currOrg = currOrg->rightChild;
	else if (key == ' ')
	    currOrg = currOrg->parent;
    } while (key != '`');

    //if (treeFile.Open("TREE.DAT", CFile::modeCreate | CFile::modeWrite))
    //{   // now serialize the family tree
	//CArchive treeArc(&treeFile, CArchive::store);
	//FoundingFather.Serialize(treeArc);
	//treeArc.Close();
	//treeFile.Close();
    //};  // if

    _clearscreen(_GCLEARSCREEN);
    _setvideomode(_TEXTC80);
};  // void main()
