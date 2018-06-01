#include <wx/wx.h>
#include <wx/image.h>
#include <wx/event.h>
#include <portAudio.h>

#define WIDTH 640
#define HIGHT 400

#define TEXTDIFF 4

class MyApp: public wxApp {
  public:
	virtual bool OnInit();
};

class MyFrame: public wxFrame {
  public:
	MyFrame(const wxString&, const wxPoint&, const wxSize&);
	bool Show(bool = true);
  private:
	~MyFrame();
	wxBitmap imgBack;
	wxClientDC *dc;

	void OnKeyDown(wxKeyEvent&);
	void OnEsc(wxCommandEvent&);
	void OnEnterMenu(wxCommandEvent&);
	void OnEnterTown(wxCommandEvent&);

	wxDECLARE_EVENT_TABLE();
};

enum {
	ID_ENTER_D = 1,
	ID_ESC_D = 2
};

wxDECLARE_EVENT(EV_ENTER_DOWN, wxCommandEvent);
wxDECLARE_EVENT(EV_ESC_DOWN, wxCommandEvent);

wxIMPLEMENT_APP(MyApp);

wxDEFINE_EVENT(EV_ENTER_DOWN, wxCommandEvent);
wxDEFINE_EVENT(EV_ESC_DOWN, wxCommandEvent);


wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_KEY_DOWN(MyFrame::OnKeyDown)
wxEND_EVENT_TABLE()

bool MyApp::OnInit() {
	wxImage::AddHandler(new wxPNGHandler);
	wxImage::AddHandler(new wxJPEGHandler);
	MyFrame *frame = new MyFrame(wxString::FromUTF8("偽物の空"), wxDefaultPosition, wxSize(WIDTH, HIGHT));
	frame->Show(true);
	return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size): wxFrame(NULL, wxID_ANY, title, pos, size, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetClientSize(size);
	SetMinClientSize(size);
	SetMaxClientSize(size);
	dc = new wxClientDC(this);
	dc->SetFont(wxFont(64, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	wxImage image;
	if(!image.LoadFile("imgs/Lybia.png")) {
		fprintf(stderr, "Could not open a image from %s\n", "imgs/Lybia.png");
		fflush(stderr);
		return;
	}
	imgBack = wxBitmap(image);
}

bool MyFrame::Show(bool B) {
	bool trt = wxFrame::Show(B);
	dc->DrawBitmap(imgBack, 0, 0, false);
	wxString str = wxString::FromUTF8("Enter: はじめ\nEsc: おわり");
	int th;
	dc->GetTextExtent(str, NULL, &th);
	/*
	dc->SetTextForeground(*wxBLACK);
	dc->DrawText(str, 0, HIGHT - th * 2 - TEXTDIFF);
	dc->DrawText(str, TEXTDIFF * 2, HIGHT - th * 2 - TEXTDIFF);
	dc->DrawText(str, TEXTDIFF, HIGHT - th * 2);
	dc->DrawText(str, TEXTDIFF, HIGHT - th * 2 - TEXTDIFF * 2);
	dc->SetTextForeground(*wxWHITE);
	dc->DrawText(str, TEXTDIFF, HIGHT - th * 2 - TEXTDIFF);
	*/
	dc->SetTextForeground(*wxBLACK);
	dc->DrawText(str, TEXTDIFF * 2, HIGHT - th * 2);
	dc->SetTextForeground(*wxWHITE);
	dc->DrawText(str, 0, HIGHT - th * 2);
	Bind(EV_ENTER_DOWN, &MyFrame::OnEnterMenu, this, ID_ENTER_D);
	Bind(EV_ESC_DOWN, &MyFrame::OnEsc, this, ID_ESC_D);
	return trt;
}

MyFrame::~MyFrame() {
	delete dc;
}

void MyFrame::OnKeyDown(wxKeyEvent &ev) {
	fprintf(stderr, "A key has pressed.\n");
	fflush(stderr);
	wxCommandEvent *newEv;
	switch(ev.GetKeyCode()) {
		case WXK_RETURN:
			newEv = new wxCommandEvent(EV_ENTER_DOWN, ID_ENTER_D);
			newEv->SetEventObject(this);
			newEv->SetString("Enter");
			ProcessWindowEvent(*newEv);
			break;
		case WXK_ESCAPE:
			newEv = new wxCommandEvent(EV_ESC_DOWN, ID_ESC_D);
			newEv->SetEventObject(this);
			newEv->SetString("Esc");
			ProcessWindowEvent(*newEv);
			break;
		default:
			;
	}
	if(newEv != NULL) {
		delete newEv;
	}
}

void MyFrame::OnEsc(wxCommandEvent &ev) {
	Close(true);
}

void MyFrame::OnEnterMenu(wxCommandEvent &ev) {
	Unbind(EV_ENTER_DOWN, &MyFrame::OnEnterMenu, this, ID_ENTER_D);
	wxImage image;
	if(!image.LoadFile("imgs/town.jpg")) {
		fprintf(stderr, "Could not open a image from %s\n", "imgs/town.jpg");
		fflush(stderr);
	}
	imgBack = wxBitmap(image);
	dc->DrawBitmap(imgBack, 0, 0, false);
	wxString str = wxString::FromUTF8("ここは町。\nEnter: タワーへ\nEsc: おわり");
	int th;
	dc->GetTextExtent(str, NULL, &th);
	dc->SetTextForeground(*wxBLACK);
	dc->DrawText(str, TEXTDIFF * 2, HIGHT - th * 3);
	dc->SetTextForeground(*wxWHITE);
	dc->DrawText(str, 0, HIGHT - th * 3);
	Bind(EV_ENTER_DOWN, &MyFrame::OnEnterTown, this, ID_ENTER_D);
}

void MyFrame::OnEnterTown(wxCommandEvent &ev) {
	fprintf(stderr, "Enter pressed.\n");
	fflush(stderr);
}
