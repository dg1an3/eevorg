use windows::Win32::{Foundation::*, UI::WindowsAndMessaging::CREATESTRUCTW};

use crate::CWnd::*;

pub struct CMainFrm {
    base: CWndStruct,
}

impl CWnd for CMainFrm {
    fn Create(
        &self,
        lpszClassName: String,
        lpszWindowName: String,
        dwStyle: DWORD,
        rect: RECT,
        pParentWnd: HWND,
        nID: UINT,
        // pContext: CCreateContext,
    ) -> BOOL {
        self.base.Create(
            lpszClassName,
            lpszWindowName,
            dwStyle,
            rect,
            pParentWnd,
            nID,
        )
    }

    fn PreCreateWindow(&self, cs: &CREATESTRUCTW) -> BOOL {
        self.base.PreCreateWindow(cs)
    }

    fn CreateEx(&self) {
        self.base.CreateEx()
    }

    fn OnCreate(&self, lpCreateStruct: &CREATESTRUCTW) {
        self.base.OnCreate(lpCreateStruct)
    }
}
