# Simple JUCE GUI Application

A basic example demonstrating JUCE GUI components and application structure.

## Features

- Window management with JUCE DocumentWindow
- GUI components (Button, Slider, Label)
- Event listeners and callbacks
- Cross-platform build with CMake

## Building

From the JUCE root directory:

```bash
cmake . -B cmake-build -DJUCE_BUILD_EXAMPLES=ON
cmake --build cmake-build --target SimpleGuiApp
```

## Running

After building, the executable will be located in `cmake-build/examples/SimpleGuiApp_artefacts/`

## Components

- **Main.cpp**: Application entry point and window setup
- **MainComponent.h/cpp**: Main GUI component with interactive elements
- **CMakeLists.txt**: CMake build configuration

## Interaction

- Click the "Click Me!" button to see an alert box
- Drag the slider to change the output value
- The output label updates in real-time as you move the slider
