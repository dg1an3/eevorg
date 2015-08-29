// eevorg.c

class Eevorg
{	// Eevorg is evolutionary organism class.
public:
	Eevorg();
	Eevorg(const Eevorg& parent, int x, int y);

	int showAt(int x, int y);

private:
	Eevorg *thisParent;
	int thisX, thisY;
};	// class Eevorg

Eevorg::Eevorg()
{	// default constructor for an Eevorg
	thisParent = (Eevorg *)NULL;
	thisX = 0;
	thisY = 0;
};	// Eevorg::Eevorg

Eevorg::Eevorg(const Eevorg& parent, int x, int y)
{	// inheritence constructor for an Eevorg
	thisParent = &parent;
	thisX = x;
	thisY = y;
};	// Eevorg::Eevorg

Eevorg::showAt(int x, int y)
{	// displays the Eevorg on the screen
	putpixel(x+thisX, y+thisY, 15);
	thisParent->showAt(x, y);
};	// Eevorg::showAt


class Session
{	// Session is the user interface class.
public:
	Session(char *fileName);    // open and append existing file

private:
	char user[30];
	int date;
	int numOfKeyPresses;
	int keyPressTime[1000];
	char keyPressed[1000];
	(Eevorg *)atEevorg[1000];
};	// class Session

Session::Session();