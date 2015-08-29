// Organism Class Hierarchy
//
//               Organism
//                  *
//             ***********
//             *         *
//          CAOrg1D    Org2D
//                       *
//             *********************
//             *         *         *
//          CAOrg2D   LifeOrg  <other 2D forms>
//

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <graph.h>
#include "organism.h"

#define random(max) ((long)rand()*max/RAND_MAX)
    // helper macro to calculate random numbers in a particular range
#define plot(X, Y, C) _setcolor(C); _setpixel(X, Y)
    // helper macro to plot a point

//////////////////////////////////////////////////////////////////////////
// class Organism

Organism::Organism(int RandomCells /* = 1 */)
{   // default constructor
    // RandomCells indicates how many cells to assign random values
    size = 5;
    cells = new char[size*size];
    memset(cells, 0, size*size);
    cells[size*size/2] = 1;

    age = 0;

    leftChild = (Organism *)NULL;
    rightChild = (Organism *)NULL;
};  // Organism::Organism()


Organism::Organism(Organism& parent)
{   // copy constructor
    size = parent.size;
    cells = new char[size*size];
    memcpy(cells, parent.cells, size*size);

    age = parent.age;

    leftChild = (Organism *)NULL;    // NO value semantics for child
    rightChild = (Organism *)NULL;    // NO value semantics for child
};  // Organism::Organism()


Organism::~Organism()
{   // destructor
    delete leftChild;
    delete rightChild;
    delete cells;
};  // Organism::~Organism()


int Organism::grow()
{   // expands the square by 1 layer on all sides
    int count;
    size += 2;
    char *newCells = new char[size*size];
    memset(newCells, 0, size*size);
    for (count = 1; count < size-1; count++)
        memcpy(&newCells[count*size+1], &cells[(count-1)*size], size-2);
    delete cells;    // free the old memory area
    cells = newCells;
    return size;
};  // Organism::grow()


int Organism::checkBounds()
{   // returns how many homogeneous layers line the square
    // returns 0 if there are different cells in outer layer,
    // 1 if there are different cells in the second-to-outer layer,
    // and 2 otherwise
    int at;
    int cellsInZone = 0;

    for (at = 0; at < size; at++)
    {   // see if any cells are in the outer layer
        if (getCell(0, at) != getCell(0, 0))
            return 0;
        if (getCell(size-1, at) != getCell(0, 0))
            return 0;
        if (getCell(at, 0) != getCell(0, 0))
            return 0;
        if (getCell(at, size-1) != getCell(0, 0))
            return 0;
    };  // for


    for (at = 1; at < size-1; at++)
    {   // see if any cells are in the second-to-outer layer
        if (getCell(1, at) != getCell(0, 0))
            return 1;
        if (getCell(size-2, at) != getCell(0, 0))
            return 1;
        if (getCell(at, 1) != getCell(0, 0))
            return 1;
        if (getCell(at, size-2) != getCell(0, 0))
            return 1;
    };  // for

    return 2;    // means 2 or more homogeneous layers
};  // Organism::checkBounds()


void Organism::evolve()
{   // user-mediated evolution procedure
    char key = 0;

    // now grow the parent and two children until a key is pressed
    // try a new child while this is happening
    Organism *newChild = spawnChild();
    do
    {   // repeat this loop until the space key is pressed
        plotAt(320, 288);
        nextGen();

        if (leftChild != NULL)
        {   // plot the left child and calc next generation
            leftChild->plotAt(160, 144);
            leftChild->nextGen();
        } else
        {   // otherwise, make new left child
            leftChild = spawnChild();
            leftChild->plotAt(160, 144);
            leftChild->nextGen();
        };  // if

        if (rightChild != NULL)
        {   // plot the left child and calc next generation
            rightChild->plotAt(160, 144);
            rightChild->nextGen();
        } else
        {   // otherwise, make new left child
            rightChild = spawnChild();
            rightChild->plotAt(160, 144);
            rightChild->nextGen();
        };  // if

        if (newChild->fitness() > leftChild->fitness())
        {   // replace the left child with the new child
            delete leftChild;
            leftChild = newChild;
            newChild = spawnChild();
        } else if (newChild->fitness() > rightChild->fitness())
        {   // replace the left child with the new child
            delete rightChild;
            rightChild = newChild;
            newChild = spawnChild();
        } else if (newChild->age > 50)
        {   // new child is 50 generations old and still has not promoted
            delete newChild;
            newChild = spawnChild();
        } else
            newChild->nextGen();

        if (kbhit())
        {   // read the key and see what needs to be done
            key = getch();
            if (key == 'A' || key == 'a')
                leftChild->evolve();
            if (key == 'L' || key == 'l')
                rightChild->evolve();
        } else
            key = 0;
    } while (key != ' ');
    delete newChild;
};  // Organism::evolve()


//////////////////////////////////////////////////////////////////////////
// class CAOrg1D

CAOrg1D::CAOrg1D()
    : Organism()
{   // default constructor
    currentRow = size/2;
    numOfRules = 4;
    rules = new char[numOfRules];
    rules[0] = 0;  rules[1] = 1;
    rules[2] = 0;  rules[3] = 1;
    temporalEntropy = 0;
    spatialEntropy = 0;
};  // CAOrg1D::CAOrg1D()


CAOrg1D::CAOrg1D(CAOrg1D& parent)
    : Organism(parent)
{   // copy constructor
    numOfRules = parent.numOfRules;
    rules = new char[numOfRules];
    memcpy(rules, parent.rules, numOfRules);
    currentRow = parent.currentRow;
    temporalEntropy = parent.temporalEntropy;
    spatialEntropy = parent.spatialEntropy;
};  // CAOrg1D::CAOrg1D()


CAOrg1D::~CAOrg1D()
{   // destructor
    delete rules;
};  // CAOrg1D::~CAOrg1D()


void CAOrg1D::plotAt(int X, int Y)
{   // displays generations of organism
    int atRow, atCol;
    for (atRow = 0; atRow < size; atRow++)
        for (atCol = 0; atCol < size; atCol++)
            plot(X-size/2+atRow, Y-size/2+atCol,
                getCell((atRow+currentRow) % size, atCol));
};  // void CAOrg1D::plotAt(int X, int Y)


void CAOrg1D::nextGen()
{   // calculates next generation, temporal entropy, spatial entropy
    int at;
    int te = 0;
    int se = 0;
    int maxval = 0;
    for (at = 1; at < size-1; at++)
    {   // calculate for entire row
        putCell(currentRow, at, rules[getCell(currentRow, at-1)+
            getCell(currentRow, at)+getCell(currentRow, at+1)]);
        te += abs(getCell(currentRow, at)-getCell(currentRow, at-1));
        se += abs(getCell(currentRow, at)-getCell(currentRow-1, at));
        maxval = (maxval < getCell(currentRow, at)) ?
            getCell(currentRow, at) : maxval;
    };  // for
    if (checkBounds() < 2)
        grow();
    if (maxval > 0)
    {   // add the amounts to the temporal and spatial entropy averages
        temporalEntropy = (temporalEntropy*5+(float)te/(size-2)/maxval)/6;
        spatialEntropy = (spatialEntropy*5+(float)te/(size-2)/maxval)/6;
    } else
    {   // otherwise, add zero to the averages
        temporalEntropy = temporalEntropy*5/6;
        spatialEntropy = spatialEntropy*5/6;
    };  // if
    if ((currentRow++) == size)
    {   // move to the top again
        memcpy(&cells[(size-1)*size], cells, size);
        currentRow = 1;
    };  // if
};  // void CAOrg1D::nextGen()


Organism *CAOrg1D::spawnChild()
{   // introduces one change in the rules
    int tempAge = age;
    age = 0;
    CAOrg1D *newChild = new CAOrg1D(*this);
    age = tempAge;
    // mutate one rule
    int ruleAt = random(numOfRules);
    if (rules[ruleAt] > 0 && random(2) < 1)
        rules[ruleAt]--;    // decrement the rule
    else if ((rules[ruleAt]++)*3+1 > numOfRules)
    {   // need more space for rules
        char *newRules = new char[rules[ruleAt]*3+1];
        memset(newRules, 0, rules[ruleAt]*3+1);
        memcpy(newRules, rules, numOfRules);
        delete rules;
        rules = newRules;
    };  // if
    return (Organism *)newChild;
};  // Organism *CAOrg1D::spawnChild()


float CAOrg1D::fitness()
{   // returns the fitness of the organism
    // fitness is proportional to |0.5-SE| |0.5-TE|
    return temporalEntropy*(1-temporalEntropy)*
        spatialEntropy*(1-spatialEntropy)*16;
};  // float CAOrg1D::fitness()


//////////////////////////////////////////////////////////////////////////
// class Org2D

Org2D::Org2D()
    : Organism()
{   // default constructor
};  // Org2D::Org2D()


Org2D::~Org2D()
{   // destructor
};  // Org2D::~Org2D()


void Org2D::plotAt(int X, int Y)
{   // displays organism cell body
};  // void Org2D::plotAt(int X, int Y)


//////////////////////////////////////////////////////////////////////////
// class CAOrg2D

CAOrg2D::CAOrg2D()
{   // default constructor
};  // CAOrg2D::CAOrg2D()


CAOrg2D::CAOrg2D(CAOrg2D& parent)
{   // copy constructor
};  // CAOrg2D::CAOrg2D(CAOrg2D& parent)


CAOrg2D::~CAOrg2D()
{   // destructor
};  // CAOrg2D::~CAOrg2D()


void CAOrg2D::nextGen()
{   // calculates next generation
};  // void CAOrg2D::nextGen()


Organism *CAOrg2D::spawnChild()
{   // introduces one change in the rules
    return this;
};  // Organism *CAOrg2D::spawnChild()


float CAOrg2D::fitness()
{   // returns the fitness of the organism
    return 0;
};  // float CAOrg2D::fitness()


//////////////////////////////////////////////////////////////////////////
// LifeOrg

LifeOrg::LifeOrg()
{   // default constructor
};  // LifeOrg::LifeOrg()


LifeOrg::LifeOrg(LifeOrg& parent)
{   // copy constructor
};  // LifeOrg::LifeOrg(LifeOrg& parent)


void LifeOrg::nextGen()
{   // calculates next generation
};  // void LifeOrg::nextGen()


Organism *LifeOrg::spawnChild()
{   // introduces one change in the rules
    return this;
};  // void LifeOrg::spawnChild()


float LifeOrg::fitness()
{   // returns the fitness of the organism
    return 0;
};  // float LifeOrg::fitness()


//////////////////////////////////////////////////////////////////////////
// main

void main()
{   // initialize an organism, and then begin evolution
    // initialize video mode
    _setvideomode(_MAXRESMODE);

    // seed random number generator
    srand((unsigned)time(NULL));

    CAOrg1D FoundingFather;
    FoundingFather.evolve();
};  // void main()
