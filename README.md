# Eevorg

An Eevorg is a funky presentation of a 1D cellular automaton, with color lookup tables derived the same way as the original FocalFusion pseudo-color lookup tables. Eevorg "evolve" through random mutation driven by user choices.

[Video demo](https://youtu.be/SQaSQhQwHgE)

## Build

This is an MFC + OpenGL Windows application. Requires Visual Studio with MFC components.

```powershell
mkdir build; cd build; cmake ..
cmake --build .
```

Output: `EevorgOgl.exe`

## Project History

| Year | Project | Description |
|------|---------|-------------|
| 1988 | TuringPas | Text GUI with machine table |
| 1991 | CA1D / AUTOMATA | First GUI for cellular automata |
| 1992 | JuliaB | Fractal visualization GUI |
| 1992 | EEVORG | First eevorg implementation (Borland C++) |
| 1995 | Winorg | Windows MFC port |
| 2007 | Eevorg.NET | .NET/CLR version |
| 2011 | EevorgBulbApp | MFC + OpenGL with 3D bulb rendering (this repo) |
| 2023 | eevorg2dsr | Python 2D CA with PyTorch + ESRGAN |
| 2025 | EevorgApi | .NET 8 Aspire REST API |
| 2025 | Winorg Python | Streamlit/Gradio web app reimplementation |

## Archive

The `archive/` directory contains historical implementations:
- `1991.10.15 AUTOMATA/` - Original cellular automata with DAT specimens
- `1992.02.12 EEVORG/` - First eevorg (Borland C++)
- `1992 FRACTAL/` - Julia set, Mandelbrot, dynamical systems
- `1993.07.12 TESTOST/` - Experimental version
- `1994-1997 Organism/` - Intermediate versions
- `1995 Winorg/` - Windows MFC port
- `2009.07 Eevorg.NET/` - .NET/CLR version with WinForms

## Related Projects

| Repository | Description |
|------------|-------------|
| [eevorg2dsr](https://github.com/dg1an3/eevorg2dsr) | Python 2D cellular automata with PyTorch, VGG perceptual loss, Real-ESRGAN super-resolution |
| [EevorgApi](https://github.com/dg1an3/EevorgApi) | .NET 8 Aspire REST API for eevorg generation and selection |

## ML & Generative Models

- **ESRGAN**: Super-resolution models in ONNX format (pinned on Pinata)
- **VAE**: AlexNet-based VAE trained on eevorg with 128 latent dimensions
- **Perceptual Search**: Novel eevorg discovery by maximizing perceptual distance
- **CycleGAN**: Style transfer experiments

## 3D Printing

Eevorg can be 3D printed as bas relief with color. Support removal is challenging for complex specimens.

## Data

- 578 historical eevorg specimens (1991-present) in various DAT formats
- Colab notebooks for TensorFlow generative models
- Google Sheets integration for trials/experiments
