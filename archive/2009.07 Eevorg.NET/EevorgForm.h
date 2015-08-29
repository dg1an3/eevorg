#pragma once

#include "EevorgRenderer.h"

namespace Eevorg {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Runtime::Serialization::Formatters::Soap;


	/// <summary>
	/// Summary for EevorgForm 
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class EevorgForm : public System::Windows::Forms::Form
	{
	public:
		EevorgForm (void)
		{
			InitializeComponent();

			eevorgRenderer = gcnew EevorgRenderer();
			eevorgRenderer->Location = System::Drawing::Point(0, 0);
			eevorgRenderer->Size = panelEevorg->Size;
			eevorgRenderer->EevorgRootChanged += 
				gcnew System::EventHandler(this, &EevorgForm::eevorgRenderer_EevorgRootChanged);
			this->panelEevorg->Controls->Add(eevorgRenderer);

			// trigger update event
			eevorgRenderer_EevorgRootChanged(eevorgRenderer, nullptr);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EevorgForm()
		{
			// serialize out
			Stream^ stream = File::Open( "data.xml", FileMode::Create );
			SoapFormatter^ formatter = gcnew SoapFormatter;

			//BinaryFormatter* formatter = new BinaryFormatter();
			formatter->Serialize( stream, eevorgRenderer->EevorgRoot );
			stream->Close();

			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Button^  btnLoad;
	protected: 

		// manually added
		EevorgRenderer ^eevorgRenderer;

	private: System::Windows::Forms::TextBox^  textScore;
	private: System::Windows::Forms::TextBox^  textMaxValue;
	protected: 

	protected: 


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Panel^  panelEevorg;
	private: System::Windows::Forms::Button^  btnBack;
	private: System::Windows::Forms::ComboBox^  comboBoxName;




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
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->btnLoad = (gcnew System::Windows::Forms::Button());
			this->textScore = (gcnew System::Windows::Forms::TextBox());
			this->textMaxValue = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->panelEevorg = (gcnew System::Windows::Forms::Panel());
			this->btnBack = (gcnew System::Windows::Forms::Button());
			this->comboBoxName = (gcnew System::Windows::Forms::ComboBox());
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// btnLoad
			// 
			this->btnLoad->Location = System::Drawing::Point(436, 315);
			this->btnLoad->Name = L"btnLoad";
			this->btnLoad->Size = System::Drawing::Size(99, 23);
			this->btnLoad->TabIndex = 0;
			this->btnLoad->Text = L"Load";
			this->btnLoad->UseVisualStyleBackColor = true;
			this->btnLoad->Click += gcnew System::EventHandler(this, &EevorgForm::button1_Click);
			// 
			// textScore
			// 
			this->textScore->Location = System::Drawing::Point(436, 209);
			this->textScore->Name = L"textScore";
			this->textScore->Size = System::Drawing::Size(100, 20);
			this->textScore->TabIndex = 2;
			this->textScore->TextChanged += gcnew System::EventHandler(this, &EevorgForm::textScore_TextChanged);
			// 
			// textMaxValue
			// 
			this->textMaxValue->Location = System::Drawing::Point(436, 256);
			this->textMaxValue->Name = L"textMaxValue";
			this->textMaxValue->ReadOnly = true;
			this->textMaxValue->Size = System::Drawing::Size(100, 20);
			this->textMaxValue->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(439, 193);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Score";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(439, 240);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(57, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Max Value";
			// 
			// panelEevorg
			// 
			this->panelEevorg->BackColor = System::Drawing::Color::DimGray;
			this->panelEevorg->Location = System::Drawing::Point(25, 19);
			this->panelEevorg->Name = L"panelEevorg";
			this->panelEevorg->Size = System::Drawing::Size(384, 325);
			this->panelEevorg->TabIndex = 6;
			// 
			// btnBack
			// 
			this->btnBack->Location = System::Drawing::Point(436, 25);
			this->btnBack->Name = L"btnBack";
			this->btnBack->Size = System::Drawing::Size(99, 25);
			this->btnBack->TabIndex = 7;
			this->btnBack->Text = L"<< Back";
			this->btnBack->UseVisualStyleBackColor = true;
			this->btnBack->Click += gcnew System::EventHandler(this, &EevorgForm::btnBack_Click);
			// 
			// comboBoxName
			// 
			this->comboBoxName->FormattingEnabled = true;
			this->comboBoxName->Location = System::Drawing::Point(436, 108);
			this->comboBoxName->Name = L"comboBoxName";
			this->comboBoxName->Size = System::Drawing::Size(121, 21);
			this->comboBoxName->TabIndex = 8;
			this->comboBoxName->SelectedIndexChanged += gcnew System::EventHandler(this, &EevorgForm::comboBoxName_SelectedIndexChanged);
			this->comboBoxName->TextChanged += gcnew System::EventHandler(this, &EevorgForm::comboBoxName_TextChanged);
			this->comboBoxName->DropDown += gcnew System::EventHandler(this, &EevorgForm::comboBoxName_DropDown);
			// 
			// EevorgForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(575, 375);
			this->Controls->Add(this->comboBoxName);
			this->Controls->Add(this->btnBack);
			this->Controls->Add(this->panelEevorg);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textMaxValue);
			this->Controls->Add(this->textScore);
			this->Controls->Add(this->btnLoad);
			this->Name = L"EevorgForm";
			this->Text = L"Eevorg";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: 
		System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				EevorgOb ^eev = gcnew EevorgOb();
				eev->LoadOldFormat(openFileDialog1->FileName);

				// populate fields
				eev->Name = System::IO::Path::GetFileNameWithoutExtension(openFileDialog1->FileName);

				// set score
				eev->Score = 1;

				// see if score can be had from file name
				String ^strExt = System::IO::Path::GetExtension(openFileDialog1->FileName);
				if (strExt != "")
				{
					try
					{
						eev->Score = 
							System::Convert::ToInt16(strExt->Substring(1));
					}
					catch (System::Exception ^ex)
					{
						// just leave as default score
						System::String^ strMessage = ex->Message;
					}
				}	

				// set the root
				eevorgRenderer->EevorgRoot = eev;
			}
		}

		System::Void btnBack_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			eevorgRenderer->BackHistory();
		}

		System::Void eevorgRenderer_EevorgRootChanged(System::Object ^sender, System::EventArgs ^ e) 
		{
			EevorgOb ^eev = eevorgRenderer->EevorgRoot;
			if (System::String::IsNullOrEmpty(eev->Name))
			{
				comboBoxName->SelectedItem = nullptr;
				comboBoxName->Text = "";
			}
			else
			{
				comboBoxName->SelectedItem = eev;
			}
			textScore->Text = System::Convert::ToString(eev->Score);
			textMaxValue->Text = System::Convert::ToString(eev->MaxVal);
		}

		System::Void textScore_TextChanged(System::Object^  sender, System::EventArgs^  e) 
		{
			EevorgOb ^eev = eevorgRenderer->EevorgRoot;
			eev->Score = System::Convert::ToInt32(textScore->Text);
		}

		System::Collections::Generic::List<EevorgOb^> m_PersistEevorgs;

		System::Void comboBoxName_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		{
			// select a new EevorgRoot
			EevorgOb ^eev = (EevorgOb ^) comboBoxName->SelectedItem;

			// check if null (if the text box is set to nothing
			if (eev != nullptr
				&& eev != eevorgRenderer->EevorgRoot)
			{
				eevorgRenderer->EevorgRoot = eev;
			}
		}

		System::Void comboBoxName_TextChanged(System::Object^  sender, System::EventArgs^  e) 
		{
			// access to the current selected object
			EevorgOb ^eev = eevorgRenderer->EevorgRoot;

			// see if eev is the same as the selected item (if not, then the selection is changing)
			if (comboBoxName->SelectedItem != nullptr
				&& comboBoxName->SelectedItem != eev)
			{
				return;
			}

			// is name changed?
			if (eev->Name != comboBoxName->Text)
			{
				// set the name
				eev->Name = comboBoxName->Text;

/*				// is the name empty?
				if (System::String::IsNullOrEmpty(eev->Name))
				{
					// if so, remove from list
					comboBoxName->Items->Remove(eev);
				}
				else
				{
					// otherwise, refresh combo box item

					// remove from the combo box, if it is there
					if (comboBoxName->Items->Contains(eev))
					{
						comboBoxName->Items->Remove(eev);
					}
					// add to the list
					comboBoxName->Items->Add(eev);
					comboBoxName->SelectedItem = eev;
				} */
			}
		}
	private: 
		System::Void comboBoxName_DropDown(System::Object^  sender, System::EventArgs^  e) 
		{
			// access to the current selected object
			EevorgOb ^eev = eevorgRenderer->EevorgRoot;

			// remove from the combo box, if it is there
			if (comboBoxName->Items->Contains(eev))
			{
				comboBoxName->Items->Remove(eev);
			}

			// is the name empty?
			if (!System::String::IsNullOrEmpty(eev->Name))
			{
				// add to the list
				comboBoxName->Items->Add(eev);
				comboBoxName->SelectedItem = eev;
			}
		}
	};
}

