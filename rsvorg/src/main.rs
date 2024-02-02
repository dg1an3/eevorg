use std::ffi::OsStr;
// use std::iter::once;
// use std::os::windows::ffi::OsStrExt;

use windows::core::*;
use windows::Win32::Foundation::*;
use windows::Win32::UI::WindowsAndMessaging::*;

mod EevorgOglApp;
use EevorgOglApp::*;

mod MainFrm;
mod CWnd;

mod EevorgViewGl;
mod Eevorg;
mod EevorgRenderer;

fn main() {
    unsafe {
        let hwnd = CreateWindowExW(
            windows::Win32::UI::WindowsAndMessaging::WINDOW_EX_STYLE(0),
            w!("BUTTON"),
            w!("Click me"),
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            10,
            10,
            300,
            100,
            None,
            None,
            None,
            None,
        );
        let mut client_rect: RECT = std::mem::zeroed();
        GetClientRect(hwnd, &mut client_rect).expect("GetClientRect failed");
        let msg = format!("client_rect: {:?}", client_rect);
        let wide = OsStr::new(&msg); //.encode_wide().chain(once(0)).collect();
        MessageBoxW(
            None,
            w!("wide"),
            w!("lpcaption"),
            MB_OK,
        );
    }
}
