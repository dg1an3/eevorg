#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "EevorgOb.h"

namespace Eevorg {

	/// <summary>
	/// Summary for EevorgRenderer
	/// </summary>
	public ref class EevorgRenderer : public System::Windows::Forms::UserControl
	{
	public:
		EevorgRenderer(void)
		{
			InitializeComponent();

			// make an eevorg
			EevorgRoot = gcnew EevorgOb();
		}

		// stores root eevorg
		property EevorgOb ^EevorgRoot
		{
			EevorgOb ^get()
			{
				return m_Eevorg;
			}

			void set(EevorgOb ^eev)
			{
				m_Eevorg = eev;
				m_mapRegions.Clear();
				Invalidate();
				EevorgRootChanged(this, nullptr);
			}
		}

		// handler for the EevorgRootChanged
		event EventHandler^ EevorgRootChanged;

		void AddHistory(EevorgOb ^eev)
		{
			m_history.Add(eev);

			// empty the beginning, to keep memory low
			if (m_history.Count > 10)
			{
				m_history.RemoveAt(0);
			}
		}

		// moves back in the history list
		void BackHistory()
		{
			if (m_history.Count > 0)
			{
				EevorgRoot = m_history[m_history.Count-1];
				m_history.RemoveAt(m_history.Count-1);
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EevorgRenderer()
		{
			if (components)
			{
				delete components;
			}
		}

		// helper drawing functions
		void DrawScale(EevorgOb ^Eev, Graphics ^grf, PointF orig, float height);

		// computes position of a particular cell
		PointF CelPos(Point atCel, float actHeight, float scale);

		// draws a cell
		void DrawCel(Graphics ^grf, int atX, int atY, int nRadius, Color clr, 
			System::Drawing::Region ^reg); 

		// computes relative offset to a child's origin
		SizeF ChildOrig(int nChild, float actHeight, float radius);

		// retrieves (and constructs) the colormap
		array<Color> ^GetColormap();

	protected:
		
		// the root eevorg
		EevorgOb ^m_Eevorg;

		// stores the colormap
		array<Color> ^m_colormap;

		// stores the regions for the Eevorg
		Dictionary<EevorgOb^, System::Drawing::Region^> m_mapRegions;
		
		// history of Eevorg
		System::Collections::Generic::List<EevorgOb^> m_history;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// EevorgRenderer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Name = L"EevorgRenderer";
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &EevorgRenderer::EevorgRenderer_MouseMove);
			this->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &EevorgRenderer::EevorgRenderer_MouseClick);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &EevorgRenderer::EevorgRenderer_Paint);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: 
		System::Void EevorgRenderer_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		{
			System::Drawing::Rectangle rect = this->ClientRectangle;
			Point orig = rect.Location;
			orig.X += rect.Width / 2;
			DrawScale(EevorgRoot, e->Graphics, orig, (float) rect.Height / 3.0f);
		}
	private: 
		System::Void EevorgRenderer_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		{
			for each (KeyValuePair<EevorgOb ^, System::Drawing::Region^> kvp in m_mapRegions)
			{
				System::Drawing::Region ^region = kvp.Value;
				if (region->IsVisible(e->Location))
				{
					if (EevorgRoot != kvp.Key)
					{
						AddHistory(EevorgRoot);
					}
					else
					{
						EevorgRoot->RemoveChildren();
					}

					// triggers change event
					EevorgRoot = kvp.Key;

					return;
				}
			}
		}
	private: 
		System::Void EevorgRenderer_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		{
			for each (KeyValuePair<EevorgOb ^, System::Drawing::Region^> kvp in m_mapRegions)
			{
				System::Drawing::Region ^region = kvp.Value;
				if (region->IsVisible(e->Location))
				{
					this->Cursor = Cursors::Hand;
					return;
				}
			}
			this->Cursor = Cursors::Default;
		}
};
}
