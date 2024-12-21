# An application that creates 100 points inside a circle and finds the shortest path
This is version 1.2 of the code:
It has advanced functionality with minimal search costs for all points. The interface for the A* algorithm has been updated. The code structure is divided into separate files.

The following improvements are possible: 
Having adjusted the price accounting, we returned to the search for new ways when we reached a dead end. Displaying additional information. Adding a new control.

**Note!:** To run it, you need to install the official software Qt "https://github.com/qt"

# Startup steps for Windows 10:
1. Launch Qt Creator 14.0.1 (Community).
2. Creating a Qt widget-based application that contains Qt Widgets Designer and C++.
3. The name of the project and the storage location are at your discretion.
4. Build system: Cmake.
5. Class name: MainWindow.
6. Package: Desktop Qt 6.7.2 MinGW 64-bit.
7. Copy the code provided above into your project: CMakeLists.txt , main.cpp and other folders.
8. Rebuild the project.
9. You can click on Analysis->Cmake Profiler.
10. Launch the project or press CTRL + R.

# The final result
![image](https://github.com/user-attachments/assets/338dd75c-33b7-4232-b932-5693cde8d0d0)
![image](https://github.com/user-attachments/assets/90b7ca76-9d0f-4301-89b6-b367e8317405)

# Usage example:
Select any of the methods.

# Checking Algorithm A*
![image](https://github.com/user-attachments/assets/a7c5796c-55f8-4ab4-8d69-5eacd1e261f3)
Select any two points and click on "Start calculation".
The green dots show the shortest path from the points you have selected.

# Checking Minimum cost
![image](https://github.com/user-attachments/assets/f365bcc5-2cbd-42f1-9ed9-a7bda19c028e)
Select any one point and click on "Start calculation".
The green dots show where your path started and where it ended. 
The yellow dots indicate how much you will spend on the way back, following the same route we started with. 

![image](https://github.com/user-attachments/assets/136fb03c-62ab-4a2a-aa21-551f641b0a0f)
Note: To view the prices in detail, you open the "Application Output" window in Qt by pressing Alt+3.

# Management:
1. Left-click on the dots.
2. Right-click to deselect.
3. The "Start calculation" button starts the A* algorithm.
4. The Reset button resets the path.
5. The "Go back" button returns us to the main menu.
6. In the upper-right corner of the window, write "Estimated fare".
