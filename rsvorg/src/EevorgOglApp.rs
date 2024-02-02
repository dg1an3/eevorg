use windows::Win32::Foundation::*;

pub struct EevorgOglApp {
	// Startup args (do not change)

	// This module's hInstance.
	m_hInstance: HINSTANCE,

	// Pointer to the command-line.
    m_lpCmdLine: String,

	// Initial state of the application's window; normally,
	// this is an argument to ShowWindow().
	m_nCmdShow: i32,

	// Running args (can be changed in InitInstance)

	// Human-redable name of the application. Normally set in
	// constructor or retreived from AFX_IDS_APP_TITLE.
	m_pszAppName: String,

	// Pointer to CDocManager used to manage document templates
	// for this application instance.
	// m_pDocManager: *mut CDocManager
}

impl EevorgOglApp {
    fn InitApplication(&self) -> bool {
        // if CDocManager::pStaticDocManager != None {
        //     if self.m_pDocManager == None {
        //         self.m_pDocManager = CDocManager::pStaticDocManager;
        //     }
        //     CDocManager::pStaticDocManager = NULL;
        // }
    
        // if self.m_pDocManager != None {
        //     self.m_pDocManager->AddDocTemplate(NULL);
        // } else {
        //     CDocManager::bStaticInit = FALSE;    
        // }
        // LoadSysPolicies();
        true
    }

    pub fn InitInstance(&self) -> bool {
        true
    }
}