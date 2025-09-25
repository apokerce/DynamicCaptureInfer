# Dynamic Capture Infer (YOLOv3 Object Detection)

A real-time object detection application using YOLOv3 implemented in C++ with OpenCV. This application can process live camera feeds or video files and detect objects in real-time.

## Features

- Real-time object detection using YOLOv3
- Support for both camera input and video file input
- Multi-threaded processing for better performance
- Video recording capability
- Screenshot functionality
- Filter detection by specific object types

## Requirements

- C++11 or higher
- OpenCV 4.x with DNN module
- pthread library

## Installation

### Install OpenCV

On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install libopencv-dev python3-opencv
```

On macOS with Homebrew:
```bash
brew install opencv
```

On Windows:
Download and install OpenCV from [opencv.org](https://opencv.org/releases/)

## Compilation

Compile the program with the following command:

```bash
g++ main.cpp src/*.cpp -o output -std=c++11 `pkg-config --cflags --libs opencv` -pthread
```

Or if you're using a Makefile approach:

```bash
make
```

## Usage

### Basic Usage

#### For all labels
```bash
./output --camera 0 --label --save_image true
```

#### For only one label
```bash
./output --camera 0 --label "person" --save_image true
```

### Camera Options
- Change camera by changing the number after `--camera` (e.g., `--camera 1`)
- During streaming, press 0/1/2 to switch cameras

### Video Input
```bash
./output --video "video_name.extension" --label --save_image true
```

### Controls
- Press `ESC` to stop the program
- Press `a` to save the current frame as an image
- Press `0`, `1`, or `2` to switch cameras during live streaming

## Project Structure

```
.
├── main.cpp                 # Main application entry point
├── include/
│   ├── Queue.h              # Thread-safe queue implementation
│   ├── ObjectDetector.h     # Object detection functionality
│   ├── VideoCapture.h       # Video capture functionality
│   ├── VideoWriter.h        # Video writing functionality
│   └── Utils.h              # Utility functions
├── src/
│   ├── Queue.cpp            # Queue implementation
│   ├── ObjectDetector.cpp   # Object detection implementation
│   ├── VideoCapture.cpp     # Video capture implementation
│   ├── VideoWriter.cpp      # Video writing implementation
│   └── Utils.cpp            # Utility functions implementation
├── yolov3.cfg               # YOLOv3 configuration file
├── yolov3.weights           # YOLOv3 pre-trained weights
├── coco.names               # COCO dataset class names
└── README.md                # This file
```

## How It Works

1. **Video Capture**: Captures frames from camera or video file
2. **Object Detection**: Processes frames using YOLOv3 neural network
3. **Post-processing**: Applies Non-Maximum Suppression to remove duplicate detections
4. **Visualization**: Draws bounding boxes and labels on detected objects
5. **Output**: Displays results in real-time and optionally saves video/images

## Configuration

### Model Files
- `yolov3.cfg`: YOLOv3 configuration file
- `yolov3.weights`: Pre-trained YOLOv3 weights (download separately)
- `coco.names`: Class names for COCO dataset

### Download Required Files

Download the YOLOv3 weights from [here](https://pjreddie.com/media/files/yolov3.weights) and place them in the project root directory.

## Troubleshooting

### Compilation Issues
If you encounter compilation errors related to OpenCV headers:
1. Make sure OpenCV is properly installed
2. Try using pkg-config to get the correct flags:
   ```bash
   g++ main.cpp src/*.cpp -o output -std=c++11 `pkg-config --cflags --libs opencv4` -pthread
   ```

### Runtime Issues
- Ensure the `coco.names` file is in the working directory
- Verify that the model files (`yolov3.cfg` and `yolov3.weights`) are present
- Check that camera devices are accessible

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- YOLOv3 implementation based on the original Darknet model
- OpenCV for computer vision capabilities
- COCO dataset for training data
