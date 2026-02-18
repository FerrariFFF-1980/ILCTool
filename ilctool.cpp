#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
    MainFrame()
        : wxFrame(nullptr,
                  wxID_ANY,
                  "ILCTool",
                  wxDefaultPosition,
                  wxSize(900, 600))
    {
        BuildUi();
        Centre();
    }

private:
    void BuildUi()
    {
        // Create a simple vertical layout
        auto *rootSizer = new wxBoxSizer(wxVERTICAL);

        // Title label
        auto *title = new wxStaticText(this, wxID_ANY, "ILCTool");
        wxFont titleFont = title->GetFont();
        titleFont.SetPointSize(titleFont.GetPointSize() + 6);
        titleFont.SetWeight(wxFONTWEIGHT_BOLD);
        title->SetFont(titleFont);

        // Subtitle
        auto *subtitle = new wxStaticText(
            this,
            wxID_ANY,
            "Open-source software for Interlaboratory Studies");

        rootSizer->AddStretchSpacer(1);
        rootSizer->Add(title, 0, wxALIGN_CENTER | wxBOTTOM, 8);
        rootSizer->Add(subtitle, 0, wxALIGN_CENTER);
        rootSizer->AddStretchSpacer(1);

        SetSizer(rootSizer);

        // Status bar
        CreateStatusBar();
        SetStatusText("Ready");
    }
};

class IlcToolApp : public wxApp
{
public:
    bool OnInit() override
    {
        auto *frame = new MainFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(IlcToolApp);
