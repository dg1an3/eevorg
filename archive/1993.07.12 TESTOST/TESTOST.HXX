// Organism Class Hierarchy
//
//              Organism               Abstract base class
//                  *
//             ***********
//             *         *
//          CAOrg1D    Org2D           Dimensions: One versus Two
//                       *
//                  ***********
//                  *         *
//               CAOrg2D   LifeOrg     Rules: Cellular Automaton versus
//                                         Game of Life


class Organism
{   // abstract base clsas for all Organism classes
    Organism(int RandomCells = 1);    // default constructor
    virtual ~Organism();

    int adjustSize();    // trims excess boundary OR expands if in boundary
        // used to maintain 2-cell buffer around square perimeter

    void evolve();    // user-mediated evolution procedure

    // pure virtual functions need to be supplied by derived classes
    virtual void plotAt(int X, int Y) = 0;    // display the organism at (X, Y)
    virtual void nextGen() = 0;    // calculate the next generation
    virtual void mutate() = 0;    // change the genome by one increment
    virtual float fitness() = 0;    // returns fitness (0.0 == not viable)

    protected:
        Organism *leftChild;    // left-hand offspring of organism
        Organism *rightChild;    // right-hand offspring of organism
        char *cells;    // pointer to cell body
        int size;    // size of cell body (always a square)
        int prevSize[10];    // sizes from last 10 adjustSize() calls
        int age;
};  // Organism


class CAOrg1D : public Organism
{   // one-dimensional cellular automaton organism
    CAOrg1D();    // default constructor
    CAOrg1D(CAOrg1D& parent);    // copy constructor
    ~CAOrg1D();    // destructor

    void plotAt(int X, int Y);    // displays generations of organism
    void nextGen();    // calculates next generation
    void mutate();    // introduces one change in the rules
    float fitness();    // returns the fitness of the organism

    private:
        int numOfRules;    // number of rules defined
            // should equal maximum rule value*3
        char *rules;    // points to rules
        int currentRow;    // row of cell body that is current generation
        float temporalEntropy;    // stores organisms average TE
        float spatialEntropy;    // stores organisms average SE
};  // CAOrg1D


class Org2D : public Organism
{   // abstract base class for all two-dimensional organisms
    Org2D();    // default constructor
    //~Org2D();

    void plotAt(int X, int Y);    // displays organism cell body
};  // Org2D


class CAOrg2D : public Org2D
{   // two dimensional cellular automaton organism
    CAOrg2D();    // default constructor
    CAOrg2D(CAOrg2D& parent);    // copy constructor
    ~CAOrg2D();    // destructor

    void nextGen();    // calculates next generation
    void mutate();    // introduces one change in the rules
    float fitness();    // returns the fitness of the organism

    private:
        int numOfRules;    // number of rules defined
            // should equal maximum rule value*9
        char *rules;    // pointer to rules
        float temporalEntropy;    // stores organisms average TE
        float spatialEntropy;    // stores organisms average SE
};  // CAOrg2D


class LifeOrg : public Org2D
{   // organism of Conway's Game of Life
    LifeOrg();    // default constructor
    LifeOrg(LifeOrg& parent);    // copy constructor
    //~LifeOrg();

    void nextGen();    // calculates next generation
    void mutate();    // introduces one change in the rules
    float fitness();    // returns the fitness of the organism

    private:
        int minRule;    // minimum neighbors for cell to stay alive
        int maxRule;    // maximum neighbors for cell to stay alive
        int birthRule;    // number of neighbors for cell to be born
        float temporalEntropy;    // stores organisms average TE
        float spatialEntropy;    // stores organisms average SE
};  // LifeOrg
