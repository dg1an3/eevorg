use windows::Win32::{Foundation::*, UI::WindowsAndMessaging::*};

pub type DWORD = u32;

pub type UINT = u32;

pub trait CWnd {
    fn PreCreateWindow(&self, cs: &CREATESTRUCTW) -> BOOL;
    fn Create(
        &self,
        lpszClassName: String,
        lpszWindowName: String,
        dwStyle: DWORD,
        rect: RECT,
        pParentWnd: HWND,
        nID: UINT,
        // pContext: CCreateContext,
    ) -> BOOL;
    fn CreateEx(&self);
    fn OnCreate(&self, lpCreateStruct: &CREATESTRUCTW);
}

pub struct CWndStruct {
    pub(crate) hWnd: HWND,
}

impl CWnd for CWndStruct {
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
        TRUE
    }

    fn PreCreateWindow(&self, cs: &CREATESTRUCTW) -> BOOL {
        todo!()
    }

    fn CreateEx(&self) {
        todo!()
    }

    fn OnCreate(&self, lpCreateStruct: &CREATESTRUCTW) {
        todo!()
    }
}
