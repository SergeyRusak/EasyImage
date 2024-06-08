#include "MyFrame1.h"
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>
#include "file_utils.h"

MyFrame1::MyFrame1(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);
	m_button2 = new wxButton(this, wxID_ANY, wxT("Start analize"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer1->Add(m_button2, 0, wxALL, 5);
	m_button4 = new wxButton(this, wxID_ANY, wxT("Clustering images by groups"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer1->Add(m_button4, 0, wxALL, 5);
	m_button1 = new wxButton(this, wxID_ANY, wxT("Search by image"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer1->Add(m_button1, 0, wxALL, 5);
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxHORIZONTAL);
	textedit = new wxTextCtrl(this,wxID_ANY);
	m_button3 = new wxButton(this, wxID_ANY, wxT("Search by text"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_button3, 1);
	bSizer3->Add(textedit, 3);

	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_bitmap1 = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
	m_bitmap1->SetMinSize(wxSize(170, -1));
	bSizer2->Add(m_bitmap1, 0, wxALL, 5);

	m_bitmap2 = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
	m_bitmap2->SetMinSize(wxSize(170, -1));
	bSizer2->Add(m_bitmap2, 0, wxALL, 5);

	m_bitmap3 = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
	m_bitmap3->SetMinSize(wxSize(170, -1));
	bSizer2->Add(m_bitmap3, 0, wxALL, 5);
	

	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);
	

	m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("Preload complete with 0 errors"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	bSizer1->Add(m_staticText1, 0, wxALL, 5);

	m_gauge1 = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	m_gauge1->SetValue(0);
	m_gauge1->Hide();

	bSizer1->Add(m_gauge1, 0, wxALL, 5);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	m_button1->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::startimage), NULL, this);
	m_button3->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::starttext), NULL, this);
	m_button4->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::Ñlustering), NULL, this);
	m_button2->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::Search), NULL, this);

}

MyFrame1::~MyFrame1()
{
	// Disconnect Events
	m_button1->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::startimage), NULL, this);
	m_button3->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::starttext), NULL, this);
	m_button4->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::Ñlustering), NULL, this);
	m_button2->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::Search), NULL, this);

}
wxImage MyFrame1::rescale(wxImage img, int w, int h) {
	wxSize imsize = img.GetSize();

	float widthFactor = (float)w / (float)imsize.GetWidth();
	float heightFactor = (float)h / (float)imsize.GetHeight();

	float scaleFactor = 0;
	if (widthFactor < heightFactor)
		scaleFactor = widthFactor;
	else
		scaleFactor = heightFactor;

	int scaledWidth = imsize.GetWidth()* scaleFactor;
	int scaledHeight = imsize.GetHeight()* scaleFactor;
	return img.Rescale(scaledWidth, scaledHeight);
}

void MyFrame1::startimage(wxCommandEvent& event) {
	
	wxString filename = wxFileSelector("Choose a image as template");
	if (!filename.empty())
	{

		wxDirDialog dlg(nullptr, "Choose output directory", "",
			wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		dlg.ShowModal();
		wxString path;
		path = dlg.GetPath();

		int w, h;
		m_bitmap1->GetSize(&w, &h);
		
		auto result = image_analize(path);
		wxMoveFile(result, dirpath);
		m_staticText1->SetLabelText(wxString("Searching images completed. Top similar images moved to " + dirpath));
		wxExecute("explorer "+path, wxEXEC_ASYNC, NULL);
		//this->Update();
	}
}
void MyFrame1::starttext(wxCommandEvent& event) {

	wxString filename = textedit->GetLineText(0);
	if (!filename.empty())
	{

		wxDirDialog dlg(nullptr, "Choose output directory", "",
			wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
		dlg.ShowModal();
		wxString path;
		path = dlg.GetPath();
		
		auto result = text_analize(filename);
		wxMoveFile(result, dirpath);
		m_staticText1->SetLabelText(wxString("Searching images completed. Top similar images moved to " + path));
		wxExecute("explorer " + path, wxEXEC_ASYNC, NULL);
		//this->Update();
	}
}


void MyFrame1::Search(wxCommandEvent& event)
{
	wxDirDialog dlg(nullptr, "Choose input directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST | wxDD_MULTIPLE);
	dlg.ShowModal();
	wxArrayString paths;
	dlg.GetPaths(paths);
	m_gauge1->Show();
	file_utils dl;
	std::vector< std::filesystem::path> start_list;
	for (const wxString& path : paths) {
		start_list.push_back(path.ToStdWstring());
	}
	std::vector<std::filesystem::path> img_paths = dl.find_images(start_list);
	for(size_t i = 0; i < img_paths.size(); i++)
	{
		
		m_gauge1->SetValue((int)(((float)i)/img_paths.size() * 100));
		m_staticText1->SetLabelText(wxString(img_paths[i].wstring()));
		long long b = 0;
		for (size_t i = 0; i < std::filesystem::file_size(img_paths[i]); i++)
		{
			b++;
		}
		
	}
	
	wxString number = wxString::Format(wxT("%i"), img_paths.size());
	wxString text = wxString("Data successfully updated for ").Append(number).Append(wxString(" images"));
	wxMessageDialog dialog(NULL, text, wxT("Success"),wxOK |wxICON_INFORMATION);
	dialog.ShowModal();

	m_gauge1->Hide();
	m_staticText1->SetLabelText(text);
}

void MyFrame1::Ñlustering(wxCommandEvent& event)
{
	wxDirDialog dlg(nullptr, "Choose input directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST | wxDD_MULTIPLE);
	dlg.ShowModal();
	wxArrayString paths;
	dlg.GetPaths(paths);

	wxDirDialog dlg2(nullptr, "Choose output directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	dlg2.ShowModal();
	wxString path;
	path = dlg2.GetPath();


	m_gauge1->Show();
	file_utils dl;
	std::vector< std::filesystem::path> start_list;
	for (const wxString& path : paths) {
		start_list.push_back(path.ToStdWstring());
	}
	std::vector<std::filesystem::path> img_paths = dl.find_images(start_list);
	for (size_t i = 0; i < img_paths.size(); i++)
	{

		m_gauge1->SetValue((int)(((float)i) / img_paths.size() * 100));
		m_staticText1->SetLabelText(wxString(img_paths[i].wstring()));
		long long b = 0;
		for (size_t i = 0; i < std::filesystem::file_size(img_paths[i]); i++)
		{
			b++;
		}

	}

	wxString number = wxString::Format(wxT("%i"), img_paths.size());
	wxString text = wxString(number).append(wxString(" images have been successfully clustered into"+i+ " groups"));
	wxMessageDialog dialog(NULL, text, wxT("Success"), wxOK | wxICON_INFORMATION);
	dialog.ShowModal();
	m_gauge1->Hide();
	m_staticText1->SetLabelText(text);
	wxExecute("explorer " + path, wxEXEC_ASYNC, NULL);


}
