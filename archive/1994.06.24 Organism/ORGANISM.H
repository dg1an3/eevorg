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
    void plotAt(int X, int Y);	  // display the organism
    void nextGen();	  // calculate the next generation
    Organism *spawnChild();
        // create copy with one mutation
    float fitness();
        // returns fitness (0 == not viable, 1 == maximally viable)

    int free_memory(size_t size);
        // garbage-collection anchor function

    // member variables
    int age;	  // number of generations since spawn
    int terminal;	// set to 1 if all of the children are null
    Organism *child[5];	  // offspring of organism
    Organism *parent;	// parent organism

    char *cells[numOfGen];    // pointers to cell bodies
    int size[numOfGen];    // array of sizes
    int maxState;	// maximum cell state in the cell body
    float temporalEntropy;	 // stores organisms average TE
    float spatialEntropy;	// stores organisms average SE

    char *rules;	// points to rules
};  // Organism
