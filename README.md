# Introduce
A GUI engine on Windows or X

Using C++ and Code::Blocks IDE

# Depends
Google Skia (https://skia.org/, on both Windows and Linux)

XCB (on Linux)

Cairo (on Linux)

Code::Blocks IDE (to configure the project, on Linux)

Microsoft Visual Studio 2013 (to configure the project, on Windows)
# Build
Chckout and build Skia (https://skia.org/)

Install the depends (only on Linux: cairo, xcb)

Linux: Open Vanilla.cbp by Code::Blocks and replace Skia's path in the compiler parameters

Windows: Open Vanilla.sln by VS2013 and  replace Skia's path in the project settings

Click the compile button in IDE
