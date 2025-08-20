//#include <wx/wx.h>
//
//// Main class that runs app:
//// OnInit: called on start up, starts program
//
//class MyApp : public wxApp 
//{
//public: virtual bool OnInit();
//};
//
//// Main window:
//// Constructor: sets up what the window will look like
//// Funcs: event handlers that take "event objects"
//
//enum // id identifier
//{
//	ID_Hello = 1
//};
//
//class MyFrame : public wxFrame {
//
//public: 
//	MyFrame(): wxFrame(NULL, wxID_ANY, "Hello World") { // frame base class
//
//		// Creates main menu:
//		wxMenu *menuFile = new wxMenu;
//		menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
//			"Help string shown in status bar for this menu item");
//		menuFile->AppendSeparator();
//		menuFile->Append(wxID_EXIT);
//
//		// Creates help menu:
//		wxMenu* menuHelp = new wxMenu;
//		menuHelp->Append(wxID_ABOUT);
//
//		// Creates status bar:
//		wxMenuBar* menuBar = new wxMenuBar;
//		menuBar->Append(menuFile, "&File");
//		menuBar->Append(menuHelp, "&Help");
//		
//		SetMenuBar(menuBar);
//		CreateStatusBar();
//		SetStatusText("Welcome to wxWidgets!");
//
//		// Event binding:
//		// type of event, function to call, window object, then optional id
//		Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
//		Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
//		Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
//}
//
//private: // Event table:
//	void OnHello(wxCommandEvent& event);
//	void OnExit(wxCommandEvent& event);
//	void OnAbout(wxCommandEvent& event);
//};
//
//
//wxIMPLEMENT_APP(MyApp); // runs the app you made (kind of like "main")
//
//bool MyApp::OnInit() {	
//	MyFrame* frame = new MyFrame(); // create window
//	
//	frame->Show(true); // set it to display
//	
//	return true; // true: keep processing; false: terminate program
//}
//
//
//// Event handlers (declared in class above, overloaded here)
//
//void MyFrame::OnExit(wxCommandEvent& event) {
//	Close(true);
//}
//
//void MyFrame::OnAbout(wxCommandEvent& event) {
//	wxMessageBox("This is a wxWidgets Hello World example",
//		"About Hello World", wxOK | wxICON_INFORMATION);
//}
//
//void MyFrame::OnHello(wxCommandEvent& event) {
//	wxLogMessage("Hello world from wxWidgets!");
//}