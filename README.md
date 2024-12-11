# An application that creates 100 points inside a circle and finds the shortest path
This version of the code requires some improvements in the form of removing unnecessary parts of the code and creating a more beautiful class with the distribution of project files.

**Note!:** To run it, you need to install the official software Qt "https://github.com/qt"

# Startup steps for Windows 10:
1. Launch Qt Creator 14.0.1 (Community).
2. Creating a Qt widget-based application that contains Qt Widgets Designer and C++.
3. The name of the project and the storage location are at your discretion.
4. Build system: Cmake.
5. Class name: MainWindow.
6. Package: Desktop Qt 6.7.2 MinGW 64-bit.
7. Copy the code provided above into your project: CMakeLists.txt , main.cpp , mainwindow.cpp , mainwindow.h.
8. Rebuild the project.
9. You can click on Analysis->Cmake Profiler.
10. Launch the project or press CTRL + R.

# The final result
![image](https://github.com/user-attachments/assets/ef7baffe-3e74-4de2-a227-2cc1c463d2cc)

# Usage example:
1. Choose any two points.
2. Click "Start calculation".
![image](https://github.com/user-attachments/assets/a771d028-ea3d-4928-8559-b7d9bf2c95bf)

# Management:
1. Left-click on the dots.
2. Right-click to cancel the selection.
3. The "Start calculation" button starts the A* algorithm.
4. The "Reset" button resets the path.
5. In the upper right corner of the window, write "Estimated cost of the way".
