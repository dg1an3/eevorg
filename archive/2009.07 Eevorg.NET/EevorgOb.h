// eevorg.h : interface of the Eevorg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

namespace Eevorg
{
	/// <summary>
	/// Represents an Eevorg object, with children
	/// </summary>
	[Serializable]
	public ref class EevorgOb 
	{
	public:
		EevorgOb();
		EevorgOb(EevorgOb ^parent);

		// name for the Eevorg
		property System::String ^Name;

		// returns number of generations
		int NumOfGen();

		// retrieve a generation
		array<int> ^GetGen(int forGen);

		// stores the score for the Eevorg
		property int Score;

		// access to the max cell value for the Eevorg;
		property int MaxVal;

		// accessors for children
		EevorgOb ^GetChild(int nIndex);
		void RemoveChildren();

		// override of the ToString method
		virtual System::String ^ToString() override { return Name; }

	// Operations
	public:
		void Mutate();
	    
	// Implementation
	public:
		// compute the next generation
		void NextGen();	

		// used to load old format files
		void LoadOldFormat(System::String ^fileName);

	protected:
		// stores the rules for the eevorg
		array<int> ^rule;

		// the successive generations
		[NonSerialized]
		array<array<int> ^> ^gen;

		// stores ref to the kids
		[NonSerialized]
		array<EevorgOb ^> ^children;

		// the random object
		[NonSerialized]
		static System::Random ^m_Rand = gcnew System::Random();
	};
}

/////////////////////////////////////////////////////////////////////////////
