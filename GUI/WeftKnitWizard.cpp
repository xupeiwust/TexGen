#include "PrecompiledHeaders.h"
#include "WeftKnitWizard.h"
#include "WindowIDs.h"

#include "WeftKnit.xpm"

using namespace TexGen;

BEGIN_EVENT_TABLE(CWeftKnitWizard, wxWizard)
EVT_WIZARD_PAGE_CHANGING(wxID_ANY, CWeftKnitWizard::OnWizardPageChanging)
EVT_INIT_DIALOG(CWeftKnitWizard::OnInit)
EVT_CHECKBOX(ID_Refine, CWeftKnitWizard::OnRefine)
EVT_CHECKBOX(ID_DefaultDomain, CWeftKnitWizard::OnDomain)
EVT_TEXT(ID_WaleWidth, CWeftKnitWizard::OnWaleWidthChanged)
EVT_TEXT(ID_CourseHeight, CWeftKnitWizard::OnCourseHeightChanged)
EVT_TEXT(ID_Thickness, CWeftKnitWizard::OnThicknessChanged)
END_EVENT_TABLE()

CWeftKnitWizard::CWeftKnitWizard(wxWindow* parent, wxWindowID id)
	: wxWizard(parent, id, wxT("Weft Knit Wizard"), wxBitmap(WeftKnit_xpm))
	, m_bCreateDomain(true)
	, m_pFirstPage(NULL)
	, m_WaleWidth(wxT("1"))
	, m_CourseHeight(wxT("1"))
	, m_YarnThickness(wxT("0.2"))
	, m_GapSize(wxT("0"))
	, m_bRefine(true)
	, m_bWaleWidthChanged(false)
	, m_bCourseHeightChanged(false)
	, m_bThicknessChanged(false)
{
	BuildPages();
	GetPageAreaSizer()->Add(m_pFirstPage);
}

CWeftKnitWizard::~CWeftKnitWizard(void)
{
	
}

void CWeftKnitWizard::OnWizardPageChanging(wxWizardEvent& event)
{

}

void CWeftKnitWizard::BuildPages()
{
	m_pFirstPage = BuildFirstPage();
}


wxWizardPageSimple* CWeftKnitWizard::BuildFirstPage()
{
	wxWizardPageSimple *pPage = new wxWizardPageSimple(this);

	wxBoxSizer *pMainSizer = new wxBoxSizer(wxVERTICAL);
	wxSizerFlags SizerFlags(0);

	SizerFlags.Border();
	SizerFlags.Expand();

	wxSizer *pSubSizer;

	pMainSizer->Add(new wxStaticText(pPage, wxID_ANY, wxT("This wizard will create a weft knit model for you.")), SizerFlags);

	SizerFlags.Align(wxALIGN_CENTER_VERTICAL);
	pSubSizer = new wxFlexGridSizer(2);

	{
		pSubSizer->Add(new wxStaticText(pPage, wxID_ANY, wxT("Wales:")), SizerFlags);
		pSubSizer->Add(m_pWalesSpin = new wxSpinCtrl(pPage, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100), SizerFlags);
		m_pWalesSpin->SetToolTip(wxT("Controls the number of wales contained in the unit cell."));

		pSubSizer->Add(new wxStaticText(pPage, wxID_ANY, wxT("Courses:")), SizerFlags);
		pSubSizer->Add(m_pCoursesSpin = new wxSpinCtrl(pPage, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100), SizerFlags);
		m_pCoursesSpin->SetToolTip(wxT("Controls the number of courses contained in the unit cell."));

		wxTextCtrl* pControl;

		pSubSizer->Add(new wxStaticText(pPage, wxID_ANY, wxT("Wale Width:")), SizerFlags);
		pSubSizer->Add(pControl = new wxTextCtrl(pPage, ID_Spacing, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC, &m_WaleWidth)), SizerFlags);
		pControl->SetToolTip(wxT("Sets the width of one wale."));

		pSubSizer->Add(new wxStaticText(pPage, wxID_ANY, wxT("Course Height:")), SizerFlags);
		pSubSizer->Add(pControl = new wxTextCtrl(pPage, ID_Width, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC, &m_CourseHeight)), SizerFlags);
		pControl->SetToolTip(wxT("Sets the height of one course."));

		pSubSizer->Add(new wxStaticText(pPage, wxID_ANY, wxT("Yarn Thickness:")), SizerFlags);
		pSubSizer->Add(pControl = new wxTextCtrl(pPage, ID_Thickness, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC, &m_YarnThickness)), SizerFlags);
		pControl->SetToolTip(wxT("Sets the thickness of the yarn."));
	}

	pMainSizer->Add(pSubSizer, SizerFlags);
	SizerFlags.Align(0);

	SizerFlags.Align(wxALIGN_CENTER_VERTICAL);
	pSubSizer = new wxFlexGridSizer(3);
	{
		wxCheckBox* pDomainBox;
		pSubSizer->Add(pDomainBox = new wxCheckBox(pPage, ID_DefaultDomain, wxT("Create default domain"), wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_bCreateDomain)), SizerFlags);
		pDomainBox->SetToolTip(wxT("Assign the default domain to the textile. The default domain is the same size as the unit cell in the x and y directions and and corresponds to the fabric thickness in the z direction."));

		pSubSizer->AddSpacer(0);
		pSubSizer->AddSpacer(0);

		wxCheckBox* pRefineBox;
		pSubSizer->Add(pRefineBox = new wxCheckBox(pPage, ID_Refine, wxT("Refine model"), wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_bRefine)), SizerFlags);
		pRefineBox->SetToolTip(wxT("Refine the model cross sections such that yarn volumes don't intersect.\nThe resulting geometry always contains a minimum gap size between two yarns."));
		
		pSubSizer->Add(new wxStaticText(pPage, wxID_ANY, wxT("Gap size:")), SizerFlags);
		wxTextCtrl* pGapSize;
		pSubSizer->Add(pGapSize = new wxTextCtrl(pPage, ID_GapSize, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC, &m_GapSize)), SizerFlags);
		pGapSize->SetToolTip(wxT("Adjusts the minimum gap size between two yarns when refine model is enabled."));

		if (!m_bRefine)
			pGapSize->Disable();
	}

	pMainSizer->Add(pSubSizer, SizerFlags);
	SizerFlags.Align(0);

	pPage->SetSizer(pMainSizer);
	pMainSizer->Fit(pPage);

	m_pWalesSpin->SetValue(2);
	m_pCoursesSpin->SetValue(2);

	return pPage;
}

bool CWeftKnitWizard::RunIt()
{
	return RunWizard(m_pFirstPage);
}

string CWeftKnitWizard::GetCreateTextileCommand(string ExistingTextile)
{
	string testReturn = "";
	return testReturn;
}

void CWeftKnitWizard::LoadSettings(const CTextileWeftKnit &WeftKnit)
{

}



void CWeftKnitWizard::OnRefine(wxCommandEvent& event)
{
	RefreshGapTextBox();
}


void CWeftKnitWizard::OnDomain(wxCommandEvent& event)
{

}

void CWeftKnitWizard::RefreshGapTextBox()
{
	wxCheckBox* pRefine = (wxCheckBox*)FindWindowById(ID_Refine, this);
	wxTextCtrl* pGapSize = (wxTextCtrl*)FindWindowById(ID_GapSize, this);

	if (pRefine)
	{
		pRefine->Enable();

		if (pRefine->GetValue())
			pGapSize->Enable();
		else
			pGapSize->Disable();
	}
}