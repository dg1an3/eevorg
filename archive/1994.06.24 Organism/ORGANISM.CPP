#define _DEBUG
#include <stdlib.h>    // include the standard C library
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <graph.h>
#include <new.h>
#include <malloc.h>
#include <math.h>
#include "organism.h"

#define max(x, y) ((x) > (y)) ? (x) : (y)
#define random(max) ((long)rand()*(max)/RAND_MAX)
    // helper macro to calculate random numbers in a particular range
//#define assert(x) if (!(x)) { printf("Assertion failed: line %i of %s",  \
//    __LINE__, __FILE__); exit(-1); }
#define assert(x)
#define RGB(r,g,b) (((long) ((b) << 8 | (g)) << 8) | (r))


struct _videoconfig vc;

//////////////////////////////////////////////////////////////////////////
// class Organism

Organism::Organism()
{   // default constructor
    age = 0;
    terminal = 1;

    int count;
    for (count = 0; count < 5; count++)
	child[count] = (Organism *)NULL;
    parent = (Organism *)NULL;

    memset(cells, 0, sizeof(cells));
    cells[0] = new char[7];
    memset(cells[0], 0, 7);
    cells[0][7/2] = 1;    // set initial cell value
    memset(size, 0, sizeof(size));
    size[0] = 7;
    temporalEntropy = 0;
    spatialEntropy = 0; 			   assert(cells != NULL);

    maxState = 1;
    rules = new char[maxState*3+1];
    rules[0] = 0;  rules[1] = 1;
    rules[2] = 0;  rules[3] = 1;		   assert(rules != NULL);
};  // Organism::Organism()


Organism::Organism(Organism& fromParent)
{   // copy constructor
    age = fromParent.age;
    terminal = 1;
    int count;
    for (count = 0; count < 5; count++)
	child[count] = (Organism *)NULL;
    parent = &fromParent;

    memset(cells, 0, sizeof(cells));
    cells[0] = new char[7];
    memset(cells[0], 0, 7);
    cells[0][7/2] = 1;    // set initial cell value
    memset(size, 0, sizeof(size));
    size[0] = 7;
    temporalEntropy = fromParent.temporalEntropy;
    spatialEntropy = fromParent.spatialEntropy;	   assert(cells != NULL);

    maxState = fromParent.maxState;
    rules = new char[maxState*3+1];
    memcpy(rules, fromParent.rules, maxState*3+1); assert(rules != NULL);
};  // Organism::Organism()


Organism::~Organism()
{   // destructor
    int count;
    for (count = 0; count < 5; count++)
	delete child[count];

    for (count = 0; count < numOfGen; count++)
	delete [] cells[count];

    delete [] rules;
};  // Organism::~Organism()


char Organism::evolve()
{   // user-mediated evolution procedure
    char key = 0;
    int childCount;
    do
    {	// repeat this loop until any key is pressed
        plotAt(vc.numxpixels/2, vc.numypixels*3/4);
        nextGen();

	for (childCount = 0; childCount < 5; childCount++)
	{   // for each child
	    if (child[childCount] == NULL)
		child[childCount] = spawnChild();    // make new child
	    else
		child[childCount]->plotAt(vc.numxpixels*(childCount+1)/6,
		    vc.numypixels/3);
	    assert(child[childCount] != NULL);
	    child[childCount]->nextGen();
	};  // if

        terminal = 0;    // if the node was terminal before, its not now

        if (kbhit())
        {   // read the key and see what needs to be done
            key = getch();
        } else
            key = 0;
    } while (key == 0);
    return key;
};  // Organism::evolve()


void Organism::plotAt(int X, int Y)
{   // plots generations at X, Y
    int gen, at;
    for (gen = 0; gen < numOfGen; gen++)
    {	// draw 2 lines for each generation
	for (at = 0; at < size[gen]; at++)
        {   // draw the indicated pixel
	    _setcolor((vc.numcolors-1)*cells[gen][at]/maxState);
	    _setpixel(X-size[gen]/2+at, Y+numOfGen/2-gen);
        };  // for
	//for (at = 0; at < size[gen]*2; at++)
	//{   // draw the indicated pixel
	//    _setcolor((vc.numcolors-1)*cells[gen][at/2]/maxState);
	//    _setpixel(X-size[gen]+at, Y+(numOfGen)-gen*2+1);
	//};  // for
    };	// for
};  // Organism::plotAt()


void Organism::nextGen()
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
};  // void Organism::nextGen()


Organism *Organism::spawnChild()
{   // introduces one change in the rules
    int tempAge = age;
    age = 0;
    Organism *newChild = new Organism(*this);
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
};  // Organism *Organism::spawnChild()



#define optSize 50

float Organism::fitness()
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
};  // Organism::fitness()


int Organism::free_memory(size_t sz)
{   // attempts to free sz bytes
    int count;
    long mem_freed = 0;
    for (count = 0; cells[count] != NULL; count++)
    {	// erase the cells
	delete [] cells[count];
        mem_freed += size[count];
        size[count] = 0;
    };  // for
    //if (mem_freed < sz)
    //	mem_freed += leftChild->free_memory(sz-mem_freed);
    //if (mem_freed < sz)
    //	mem_freed += midChild->free_memory(sz-mem_freed);
    //if (mem_freed < sz)
    //	mem_freed += rightChild->free_memory(sz-mem_freed);
    return mem_freed;
};  // Organism::free_memory


Organism FoundingFather;

int free_memory(size_t size)
{   // tries to free up size bytes on the heap
    return FoundingFather.free_memory(size);
};  // free_memory

void main(int argc, char *argv[])
{   // initialize an organism, and then begin evolution
    // initialize video mode
    Organism *currOrg;
    char key;
    int count;

    FILE *orgFile;
    char rules[256];
    size_t ruleSize;

    if ((orgFile = fopen(argv[1], "rb")) != NULL )
    {	// open designated rule file
	ruleSize = fread(&rules, 1, sizeof(rules), orgFile);
	fclose(orgFile);
	FoundingFather.maxState = (ruleSize-1) / 3;
	FoundingFather.rules = rules;
    };	// if

    // set the video mode to 640x480 256 color
    _setvideomode(_VRES256COLOR);
    _getvideoconfig(&vc);

    // remap the palette
    long colors[256];
    for (count = 0; count < 256; count++)
	colors[count] =
	    RGB((short)(32.0-32.0*cos((double)count*3.14159/256.0)),
		(short)(32.0-32.0*cos((double)count*9.42478/256.0)),
		(short)(32.0-32.0*cos((double)count*15.70796/256.0)));
    _remapallpalette(colors);

    // set the special memory handler
    // _set_new_handler(free_memory);

    // seed random number generator
    srand((unsigned)time(NULL));

    currOrg = &FoundingFather;
    do
    {	// evolution loop
	_clearscreen(_GCLEARSCREEN);
	key = currOrg->evolve();
	if (key == 'A' || key == 'a')
	    currOrg = currOrg->child[0];
	else if (key == 'D' || key == 'd')
	    currOrg = currOrg->child[1];
	else if (key == 'G' || key == 'g')
	    currOrg = currOrg->child[2];
	else if (key == 'J' || key == 'j')
	    currOrg = currOrg->child[3];
	else if (key == 'L' || key == 'l')
	    currOrg = currOrg->child[4];
	else if (key == ' ')
	    currOrg = currOrg->parent;
	assert(currOrg != NULL);
    } while (key != '`');

    // now save currOrg rule to a file
    if ((orgFile = fopen("rule.dat", "wb")) != NULL)
    {	// open designated rule file
	fwrite(currOrg->rules, 1, currOrg->maxState*3+1, orgFile);
	fclose(orgFile);
    };	// if

    _clearscreen(_GCLEARSCREEN);
    _setvideomode(_TEXTC80);
};  // void main()
