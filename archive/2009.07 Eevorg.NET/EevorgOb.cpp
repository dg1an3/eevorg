// eevorg.cpp : implementation of the EevorgOb class
//

#include "stdafx.h"

#include "EevorgOb.h"

using namespace System::IO;

namespace Eevorg 
{

//////////////////////////////////////////////////////////////////////////////
EevorgOb::EevorgOb()
{
	// default to empty string
	Name = "";

	// a good default rule
	rule = gcnew array<int>(10) {0, 2, 0, 1, 1, 3, 2, 1, 1, 0};

	// set max value
	MaxVal = 3;
}

//////////////////////////////////////////////////////////////////////////////
EevorgOb::EevorgOb(EevorgOb ^parent)
{
	// default to empty string
	Name = "";

	System::Array::Resize(rule, parent->rule->Length);
	System::Array::Copy(parent->rule, rule, parent->rule->Length);
	MaxVal = parent->MaxVal;
	Mutate();
}

//////////////////////////////////////////////////////////////////////////////
int 
	EevorgOb::NumOfGen()	
{
	return gen ? gen->Length : 0; 
}

//////////////////////////////////////////////////////////////////////////////
array<int> ^
	EevorgOb::GetGen(int forGen)	
{
	while (NumOfGen()-1 < forGen)
	{
		NextGen();
	}
	return gen[forGen];
}  

//////////////////////////////////////////////////////////////////////////////
void 
	EevorgOb::NextGen()
{	
	// initialize generation array
	if (!gen)
	{
		array<int> ^firstGen = gcnew array<int>(1) { 1 };
		gen = gcnew array<array<int>^>(1) { firstGen };
	}

	// get ref to last known generation
	array<int> ^prev = gen[gen->Length-1];

	// create next generation array
	array<int> ^next = gcnew array<int>(prev->Length+1);

	// calculate CA rules based on length of prev gen
	if (prev->Length > 1)
	{
		int n = 0;
		next[n++] = rule[prev[0] + 2 * prev[1]];
		while (n < prev->Length-2)
		{
			next[n++] = rule[prev[n-1] + prev[n] + prev[n+1]];
		}
		next[n++] = rule[prev[n-1] + prev[n]];
		next[n++] = rule[prev[n-1]];
	}
	else
	{
		next[0] = rule[prev[0]];
		next[1] = rule[prev[0]];
	}

	// add the next generation to the gen array
	int nextNdx = gen->Length;
	System::Array::Resize(gen, gen->Length+1);
	gen[nextNdx] = next;
}
         
//////////////////////////////////////////////////////////////////////////////
void 
	EevorgOb::Mutate()
{
	bool bMutated = false;
	do
	{
		// pick a rule
		int ruleNum = m_Rand->Next(0, rule->Length);

		// get the value
		int ruleVal = rule[ruleNum];

		// and try to mutate it
		ruleVal += m_Rand->Next(-1, 2);
		ruleVal = System::Math::Max(0, ruleVal);

		// have we succeeded in changing the value?
		if (ruleVal != rule[ruleNum])
		{
			// set the new rule
			rule[ruleNum] = ruleVal;

			// set flag to indicate success
			bMutated = true;

			// see if we need more rules...
			while (ruleVal > MaxVal)
			{
				// create three new rules
				System::Array::Resize(rule, rule->Length+3);

				// and increase max
				MaxVal = MaxVal+1;
			}
		}
	} while (!bMutated);

	// causes recalculation of generations 
	gen = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
EevorgOb ^
	EevorgOb::GetChild(int nIndex)
{
	if (!children)
	{
		children = gcnew array<EevorgOb^>(3);
	}

	if (!children[nIndex])
	{
		children[nIndex] = gcnew EevorgOb(this);
	}

	return children[nIndex];
}

/////////////////////////////////////////////////////////////////////////////
void 
	EevorgOb::RemoveChildren()
{
	children = nullptr;
}

/////////////////////////////////////////////////////////////////////////////
void 
	EevorgOb::LoadOldFormat(System::String ^fileName)
{
	if (File::Exists(fileName))
	{
		BinaryReader^ binReader = gcnew BinaryReader(File::Open(fileName, FileMode::Open));

		// read orig rules
		array<unsigned char> ^rulesIn = binReader->ReadBytes(64);

		// reset max value
		MaxVal = 0;

		// set to current rules
		System::Array::Resize(rule, rulesIn->Length);
		for (int n = 0; n < rulesIn->Length; n++)
		{
			rule[n] = rulesIn[n];

			// compute max value
			MaxVal = System::Math::Max(MaxVal, rule[n]);
		}

		// reset everything
		gen = nullptr;
		children = nullptr;		
	}
}

}