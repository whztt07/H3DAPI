///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TreeViewer__
#define __TreeViewer__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/frame.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/grid.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* MainMenuBar;
		wxMenu* FileMenu;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnIdle( wxIdleEvent& event ){ event.Skip(); }
		virtual void OnOpenFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("H3D TreeViewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class TreeViewDialog
///////////////////////////////////////////////////////////////////////////////
class TreeViewDialog : public wxDialog 
{
	private:
	
	protected:
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnNodeSelected( wxTreeEvent& event ){ event.Skip(); }
		
	
	public:
		wxTreeCtrl* TreeViewTree;
		TreeViewDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Tree view"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 356,426 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~TreeViewDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class FieldValuesDialog
///////////////////////////////////////////////////////////////////////////////
class FieldValuesDialog : public wxDialog 
{
	private:
	
	protected:
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnIdle( wxIdleEvent& event ){ event.Skip(); }
		virtual void OnCellEdit( wxGridEvent& event ){ event.Skip(); }
		
	
	public:
		wxGrid* FieldValuesGrid;
		FieldValuesDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 197,265 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~FieldValuesDialog();
	
};

#endif //__TreeViewer__