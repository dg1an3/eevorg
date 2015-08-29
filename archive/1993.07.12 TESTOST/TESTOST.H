//////////////////////////////////////////////////////////////////////////
// Organism Class Hierarchy
//
//                        Organism
//                           *
//          ***********************************
//          *                                 *
//        Org1D                             Org2D
//          *                                 *
//     ***********          *************************************
//     *         *          *        *        *        *        *
//  CAOrg1D      *       CAOrg2D  LifeOrg  JuliaOrg MiraOrg     *
//               *                                              *
//        <other 1D forms>                               <other 2D forms>


#define numOfGen 100
    // number of generations to store

class Organism
{   // abstract base class for all Organism classes
    public:

    Organism();    // default constructor
    Organism(Organism& parent);    // copy constructor
    virtual ~Organism();    // destructor

    // user-interface functions
    char evolve();	// user-mediated evolution procedure

    // pure virtual functions--need to be supplied by derived classes
    // NOTE: changed to non-abstract so that CObject could serialize
    virtual void plotAt(int X, int Y) { };    // display the organism
    virtual void nextGen() { };    // calculate the next generation
    virtual Organism *spawnChild() { return (Organism *)NULL; };
        // create copy with one mutation
    virtual float fitness() { return 0; };
        // returns fitness (0 == not viable, 1 == maximally viable)

    virtual int free_memory(size_t size);
        // garbage-collection anchor function

    int age;	  // number of generations since spawn
    int terminal;	// set to 1 if all of the children are null
    Organism *leftChild;    // left-hand offspring of organism
    Organism *midChild;    // middle offspring of organism
    Organism *rightChild;	 // right-hand offspring of organism
    Organism *parent;	// parent organism
	char *cells[numOfGen];	  // pointers to cell bodies
};  // Organism


class Org1D : public Organism
{   // base class for all one-dimensional organisms
    public:

    Org1D();    // default constructor
    Org1D(Org1D& parent);    // copy constructor
    virtual ~Org1D();    // destructor

    void plotAt(int X, int Y);    // plots generations at X, Y
    float fitness();    // returns the fitness of the organism

    int free_memory(size_t size);    // garbage collection

    // protected:
	// char *cells[numOfGen];    // pointers to cell bodies
        int size[numOfGen];    // array of sizes
        int maxState;    // maximum cell state in the cell body
        float temporalEntropy;    // stores organisms average TE
        float spatialEntropy;    // stores organisms average SE
};  // Org1D


class CAOrg1D : public Org1D
{   // one-dimensional cellular automaton organism
    // genome is the rule table
    public:

    CAOrg1D();    // default constructor
    CAOrg1D(CAOrg1D& parent);    // copy constructor
    ~CAOrg1D();    // destructor

    void nextGen();    // calculates next generation
    Organism *spawnChild();    // creates child with one mutation to rules

    private:
        char *rules;    // points to rules
};  // CAOrg1D


class Org2D : public Organism
{   // abstract base class for all two-dimensional organisms
    public:

    Org2D();    // default constructor
    ~Org2D();    // destructor

    void plotAt(int X, int Y);    // plots cell body at X, Y
    float fitness();    // returns the fitness of the organism

    protected:
        char *cells[numOfGen];    // pointer to cell bodies
        int size[numOfGen];    // array of sizes
        float temporalEntropy;    // stores organisms average TE
        float spatialEntropy;    // stores organisms average SE
};  // Org2D


class CAOrg2D : public Org2D
{   // two dimensional cellular automaton organism
    // genome is the table of rules
    public:

    CAOrg2D();    // default constructor
    CAOrg2D(CAOrg2D& parent);    // copy constructor
    ~CAOrg2D();    // destructor

    void nextGen();    // calculates next generation
    Organism *spawnChild();    // creates child with one mutation to rules

    private:
        int numOfRules;    // number of rules defined
        char *rules;    // pointer to rules
};  // CAOrg2D


class LifeOrg : public Org2D
{   // organism of Conway's Game of Life
    // genome is the starvation rule, overcrowding rule, and birth rule
    public:

    LifeOrg();    // default constructor
    LifeOrg(LifeOrg& parent);    // copy constructor

    void nextGen();    // calculates next generation
    Organism *spawnChild();    // creates child with one mutation in rules
    float fitness();    // returns the fitness of the organism

    //private:
        int starveRule;    // minimum neighbors for cell to stay alive
        int overcrowdRule;    // maximum neighbors for cell to stay alive
        int birthRule;    // number of neighbors for cell to be born
};  // LifeOrg


class JuliaOrg : public Org2D
{   // organism based on Julia sets
    // genome is parameters A and B (real numbers), step size, color map
};  // class JuliaOrg


class MiraOrg : public Org2D
{   // organism based on Mira plots
    // genome is parameters A and B (real numbers)
};  // class MiraOrg
