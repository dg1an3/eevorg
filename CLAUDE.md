# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Eevorg is a visualization of 1D cellular automata with HSL-based color lookup tables. Organisms "evolve" through random mutation driven by user choices. The project is a Windows MFC + OpenGL application.

## Build Commands

Generate build solution (from repository root):
```powershell
mkdir build; cd build; cmake ..
```

Build the project (after generating):
```powershell
cmake --build build
```

The output is `EevorgOgl.exe`, a Windows GUI application.

## Architecture

### Core Classes

- **Eevorg** (`EevorgOgl/Eevorg.h`, `Eevorg.cpp`): The core organism model
  - Stores the cellular automaton rule as a "gene" (`GeneType` = `valimage<size_t>`)
  - Generates successive generations by applying box filter + gene lookup
  - Maintains HSL color table with hueSlope, hueOffset, saturationFrequency parameters
  - Supports parent-child tree structure with mutation on child creation
  - Each organism has a unique `pickName` (GLuint) for OpenGL selection

- **EevorgRenderer** (`EevorgOgl/EevorgRenderer.h`): Renders organisms to pixel buffer
  - `DrawEevorgAndChildren()`: Recursive rendering with bulb transformation
  - `DrawScale()`: Single organism at given origin/height/zoom
  - Uses histogram equalization (HE) for color mapping

- **valimage<T>** (`EevorgOgl/valimage.h`): Template 2D image container using `std::valarray`
  - `applyBoxFilter()`: Neighborhood sum operation used for CA generation

### MFC Components

- **CEevorgDoc** (`EevorgOgl/EevorgDoc.h`): Document holding root and current Eevorg pointers
- **CEevorgViewGl** (`EevorgOgl/EevorgViewGl.h`): OpenGL view with mouse interaction and texture mapping

### Key Algorithms

1. **Generation computation**: Box filter (sum of 3-neighbor for 1D) followed by gene lookup table
2. **Color mapping**: HSL sine wave coloring with optional histogram equalization
3. **Mutation**: Randomly increments one gene value, adjusts color parameters with variance

## Archive

The `archive/` directory contains historical implementations dating back to 1991, including:
- 1991 AUTOMATA: Original cellular automata experiments
- 1992 EEVORG: First eevorg implementation
- 1992 FRACTAL: Julia set, Mandelbrot, and dynamical systems code
- 2009 Eevorg.NET: .NET/CLR version

These are preserved for reference but not part of the active build.

## Sibling Projects & Merge Tasks

Related eevorg projects have been copied to `_staging/` (git-ignored) for consolidation.

### Related Repositories (with GitHub remotes)

| Repo | Remote | Description |
|------|--------|-------------|
| `C:\dev\eevorg2dsr` | dg1an3/eevorg2dsr | Python 2D CA with PyTorch, VGG perceptual loss, ESRGAN |
| `C:\dev\EevorgApi` | dg1an3/EevorgApi | .NET 8 Aspire REST API for eevorg |

### Staging Folder Contents

The `_staging/` folder (git-ignored) contains projects pending merge:

```
_staging/
├── 000 Eevorg Data.zip        # 605MB backup archive
├── 000_Eevorg_Data/           # Main ML experiments (moved from OneDrive)
│   ├── winorg/                # Python reimplementation + Streamlit
│   ├── data/                  # MNIST, CIFAR-10, etc.
│   └── eevorg_dataset_model_runs/
├── 000_Eevorg_Data_desktop/   # VAE training outputs (100MB model)
├── 32_EEVORG/                 # Docs + 2007 Eevorg.NET
├── eevorg2d_syllables/        # Older version of eevorg2dsr (can delete)
├── Winorg/                    # 1995 MFC version with CMake
└── winorg_python/             # Duplicate of 000_Eevorg_Data/winorg (can delete)
```

### Pending Merges

#### Priority 1: Historical Eevorg Specimens
- **Source**: `_staging/000_Eevorg_Data/winorg/data/historical_reformatted/`
- **Contents**: 578 historical eevorg specimens (dated 1991-present) in text format
- **Action**: Merge into `archive/` or new `data/` folder

#### Priority 2: Python Reimplementation
- **Source**: `_staging/000_Eevorg_Data/winorg/`
- **Contents**:
  - `eevorg.py` - Core Eevorg class in Python (rules, mutations, generations)
  - `winorg.py` - Streamlit web app for interactive exploration
  - `read_orgs.py` - Utilities for reading historical eevorg files
  - `gradio_app.py` - Gradio web interface
  - `eevorg_model.py` - ML model definition
  - `ebm_resnet50.py`, `ebm_tutorial.py` - Energy-based model training
- **Action**: Create `python/` folder or separate repo

#### Priority 3: Documentation
- **Source**: `_staging/32_EEVORG/`
- **Contents**:
  - `Eevorg and Friends.pdf` (4.2MB) - comprehensive documentation
  - `Eevorg Hand Notes 001.jpg` - hand-written design notes
- **Action**: Copy to `docs/` folder

#### Priority 4: 1995 Winorg Updates
- **Source**: `_staging/Winorg/`
- **Contents**:
  - Updated `EEVORG.CPP` with modern C++ fixes (loop variable scope)
  - `CMakeLists.txt` and `BUILD.md` for modern builds
  - `DAT/` folder with 36 eevorg specimens
- **Action**: Merge into `archive/1995 Winorg/`

### Cleanup

The following can be deleted from `_staging/` after merging relevant content:
- `winorg_python/` - duplicate of `000_Eevorg_Data/winorg/`
- `eevorg2d_syllables/` - older version, superseded by eevorg2dsr
- `000 Eevorg Data.zip` - backup, now redundant

All original locations (`C:\dev\Winorg`, `C:\dev\32 EEVORG`, `C:\dev\eevorg2d_syllables`, OneDrive folders) have been moved to `_staging/`.
