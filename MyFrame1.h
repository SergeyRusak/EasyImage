#pragma once
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/frame.h>
#include <wx/statbmp.h>
#include <wx/gauge.h>
#include <wx/textctrl.h>
#include <wx/filefn.h>


class MyFrame1 : public wxFrame
{
private:

protected:
	wxButton* m_button1;
	wxButton* m_button2;
	wxButton* m_button3;
	wxButton* m_button4;
	wxStaticBitmap* m_bitmap1;
	wxStaticBitmap* m_bitmap2;
	wxStaticBitmap* m_bitmap3;
	wxStaticText* m_staticText1;
	wxBoxSizer* bSizer2;
	wxGauge* m_gauge1;
	wxTextCtrl* textedit;

	// Virtual event handlers, override them in your derived class
	void starttext(wxCommandEvent& event);
	void startimage(wxCommandEvent& event);
	void Search(wxCommandEvent& event);
	void Ñlustering(wxCommandEvent& event);
	wxImage rescale(wxImage img, int w, int h);

public:

	MyFrame1(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~MyFrame1();

};