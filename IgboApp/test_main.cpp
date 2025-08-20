//#include <wx/wx.h>
//#include <windows.h>
//
//class TestFrame : public wxFrame {
//
//private:
//	wxPanel* panel;
//	wxBoxSizer* box_sizer;
//
//	void OnExit(wxCommandEvent& event);
//
//public:
//	TestFrame(const char *string);
//};
//
//TestFrame::TestFrame(const char *string) : wxFrame(NULL, wxID_ANY, string) {
//	panel = new wxPanel(this, wxID_ANY);
//
//	box_sizer = new wxBoxSizer(wxVERTICAL);
//	box_sizer->SetSizeHints(this);
//	SetClientSize(600, 300);
//
//	wxLogMessage(string);
//}
//
//void TestFrame::OnExit(wxCommandEvent& event) {
//	Close(true);
//}
//
//class TestApp : public wxApp {
//	virtual bool OnInit();
//};
//
//bool TestApp::OnInit() {
//	// Figure out how to print in order to see
//	// if the constructor is being called
//
//	OutputDebugString(L"this also works\n"); // has to be run in debug mode
//
//	TestFrame* frame = new TestFrame("This totally works");
//	frame->Show(true);
//	return true;
//}
//
//wxIMPLEMENT_APP(TestApp);