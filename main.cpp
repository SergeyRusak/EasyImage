#include <wx/wx.h>
#include <wx/dirdlg.h>
#include <wx/arrstr.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <cstdlib>
#include "file_utils.h"
#include "wxSPanel.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
//#include "include/cppflow/cppflow.h"
#include "MyFrame1.h"

class App : public wxApp {
public:
    bool OnInit() {

 





        wxFrame* window = new MyFrame1(NULL, wxID_ANY, "GUI Test", wxDefaultPosition, wxSize(600, 400));
      /*  file_utils dl;
        wxDirDialog dlg(nullptr, "Choose input directory", "",
            wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST | wxDD_MULTIPLE);
        dlg.ShowModal();
        wxArrayString paths;
        dlg.GetPaths(paths);
        std::vector< std::filesystem::path> start_list;
        for (const wxString& path : paths) {
            start_list.push_back(path.ToStdWstring());
        }
        std::vector<std::filesystem::path> img_paths = dl.find_images(start_list);
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        wxString count = img_paths.back().wstring();
        wxImage img;
        img.AddHandler(new wxJPEGHandler);
        img.AddHandler(new wxPNGHandler);
        img.AddHandler(new wxGIFHandler);
        if (!img.LoadFile(count, wxBITMAP_TYPE_ANY)) {
            wxLogError("error image failed");
        }
        wxSPanel* imgp = new wxSPanel;
        imgp->Create(window,wxID_ANY,wxDefaultPosition,wxSize(600,400));
         img.Rescale(600,400);
        sizer->Add(imgp, 1, wxALIGN_CENTER);
        imgp->SetBackgroundBitmap(wxBitmap(img));
        window->SetSizer(sizer);*/
        Sleep(1000);
        window->Show();
        return true;
    }
};

wxIMPLEMENT_APP(App);