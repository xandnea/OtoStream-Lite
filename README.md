# OtoStream-Lite

A real-time clinical endoscopy monitor built entirely in standard C++17 and Qt 6. Designed to replicate high-performance medical imaging workflows, this application demonstrates thread-safe video generation and UI rendering with zero external media dependencies. 

## Features
* **Thread-Safe Video Streaming:** Utilizes a background `QThread` and `FrameWorker` to simulate an endoscopic tissue video stream at ~30 FPS.
* **Real-time Image Processing:** Applies CPU-based brightness/contrast adjustments and dynamic boundary thresholding.
* **Responsive UI:** Features a non-blocking `QMainWindow` with real-time FPS monitoring and filter toggles.
* **Memory Safety:** Leverages Qt's Signal/Slot QueuedConnections and deep-copied `QImage` buffers to prevent cross-thread memory corruption and GUI starvation.

## Build Instructions
1. Open `CMakeLists.txt` in **Qt Creator**.
2. Configure the project with a Desktop Qt 6 Kit (e.g., MinGW 64-bit, MSVC, or Clang).
3. Right-click the project -> **Run CMake**.
4. Build and Run.
