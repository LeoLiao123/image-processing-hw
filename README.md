
# Image Processing with OpenCV

This project consists of three separate tasks focused on image processing using OpenCV in C++. Each task demonstrates specific applications of computer vision techniques.

## Task 1: Line Drawing and Intersection Detection
- **Description**: Reads coordinates from a file and classifies them into categories A, B, and C. Each category of points is drawn on separate images.
- **Intersection Detection**: Identifies and outputs the intersection points between pairs of images.

## Task 2: Polygon Area and Overlap Calculation
- **Description**: Calculates the area of a polygon defined by a set of points and computes the overlap rate with a given polygon.
- **Overlap Rate**: Uses intersection points and calculates the overlap rate based on area percentages.

## Task 3: Camera Calibration and Image Undistortion
- **Description**: Calibrates the camera using chessboard images to correct lens distortion.
- **Undistortion**: Outputs the undistorted image based on the calculated camera parameters.

## Requirements
- OpenCV
- C++11 or later

## Compilation
Use the following command to compile each task:
```bash
g++ -o task1 task1.cpp `pkg-config --cflags --libs opencv4`
g++ -o task2 task2.cpp `pkg-config --cflags --libs opencv4`
g++ -o task3 task3.cpp `pkg-config --cflags --libs opencv4`
```

## Usage
Run each task with the required input files:
```bash
./task1 <input_file>
./task2 <input_file>
./task3 <input_file> <input_image> <output_image>
```
