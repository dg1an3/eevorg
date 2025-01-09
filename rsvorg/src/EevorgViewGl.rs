use std::collections::HashMap;

use windows::Win32::Foundation::*;
use windows::Win32::Graphics::Gdi::*;
use windows::Win32::Graphics::OpenGL::*;
use windows::Win32::UI::WindowsAndMessaging::*;


use crate::Eevorg::*;
use crate::EevorgRenderer::*;

pub struct CEevorgViewGl {
    m_oldRect: RECT,
    m_fRadius: f32,
    m_pDC: HDC,
    m_mapTextures: HashMap<Eevorg, GLuint>,
    // the renderer for performing the drawing
    _renderer: EevorgRenderer,
}
