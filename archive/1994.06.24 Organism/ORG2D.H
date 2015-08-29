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
