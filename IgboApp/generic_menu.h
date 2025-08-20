/*
* This is a generic menu frame from which all other menu frames inherit from.
* It gives each frames its own main panel and sizer, which makes formatting items
* vertically easy when desiging a new frame.
*/


#pragma once

#ifndef GENERIC_WIDGETS_LIBS
#define GENERIC_WIDGETS_LIBS

#include <wx/wx.h>

#define BUTTON_WIDTH (200)
#define BUTTON_HEIGHT ((BUTTON_WIDTH) / 4)

#define DEFAULT_WIDTH (600)
#define DEFAULT_HEIGHT (300)

class GenericMenuFrame : public wxFrame {

private: 
	wxPanel* main_panel;
	wxBoxSizer* box_sizer;

protected:

	// Used to access frame's main panel
	wxPanel* getPanel();

	// Used to access frame's main sizer
	wxBoxSizer* getBoxSizer();

	// TO BE REMOVED
	wxButton* makeMenuButton(wxPanel* panel, wxSizer* sizer, 
		int d, const char* label, wxSizerFlags flags);

	// Events
	void OnExit(wxCommandEvent& event);

public:

	/*
	* Creates a new menu frame given a name, a height, and a width.
	*/
	GenericMenuFrame(const char* frame_name,
		int width, int height);
};

#endif
