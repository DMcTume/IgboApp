#include "generic_menu.h"

GenericMenuFrame::GenericMenuFrame(const char* frame_name,
	int width, int height)
	: wxFrame(NULL, wxID_ANY, frame_name) {

	main_panel = new wxPanel(this, wxID_ANY);

	box_sizer = new wxBoxSizer(wxVERTICAL);

	main_panel->SetSizerAndFit(box_sizer);
	box_sizer->SetSizeHints(this);
	SetClientSize(width, height);
	SetMinSize(wxSize(width, height));
}

void GenericMenuFrame::OnExit(wxCommandEvent& event) {
	Close(true);
}

wxButton* GenericMenuFrame::makeMenuButton(wxPanel* panel, wxSizer *sizer,
			int id, const char* label, wxSizerFlags flags) {

	wxButton* new_button = new wxButton(panel,
		id,
		label,
		wxDefaultPosition,
		wxSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	if (sizer) {
		sizer->Add(new_button, flags);
		sizer->Layout();
	}

	return new_button;
}

wxPanel* GenericMenuFrame::getPanel() {
	return main_panel;
}

wxBoxSizer* GenericMenuFrame::getBoxSizer() {
	return box_sizer;
}
