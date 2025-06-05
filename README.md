# Kinect Syphon Sender (Kinect v1)

An openFrameworks application that captures Kinect data and sends it via Syphon for use in other applications like MadMapper, Resolume, or other creative software.

## Requirements

- **openFrameworks 0.12.0** or later
- **macOS** (for Syphon support)
- **Kinect sensor** (Xbox 360 Kinect or Kinect v1)
- **libusb** - Low-level USB library for Kinect communication
- **libfreenect** - Driver/library for Kinect v1 hardware (you can build your own app using the libfreenect2 library)

## Dependencies

This project requires the following openFrameworks addons:
- `ofxKinect` - for Kinect sensor integration
- `ofxSyphon` - for sending video/texture data to other applications

## Installation

1. **Download openFrameworks 0.12.0**
   - Get it from [openframeworks.cc](https://openframeworks.cc/download/)

2. **Install system dependencies**
   ```bash
   # Install libusb and libfreenect via Homebrew
   brew install libusb
   brew install libfreenect
   
   # Alternative: MacPorts
   # sudo port install libusb +universal
   # sudo port install libfreenect +universal
   ```

3. **Check if dependencies are already included**
   ```bash
   # Check if already installed
   brew list | grep -E "(libusb|freenect)"
   
   # Check ofxKinect addon for bundled libraries
   ls path/to/openFrameworks/addons/ofxKinect/libs/
   ```

4. **Install required addons**
   ```bash
   cd path/to/openFrameworks/addons/
   
   # Install ofxKinect (usually included with openFrameworks)
   # If not included, get it from:
   # git clone https://github.com/ofTheo/ofxKinect.git
   
   # Install ofxSyphon
   git clone https://github.com/astellato/ofxSyphon.git
   ```

5. **Clone this project**
   ```bash
   cd path/to/openFrameworks/apps/myApps/
   git clone https://github.com/yourusername/kinectSyphonSender.git
   ```

6. **Build the project**
   ```bash
   cd kinectSyphonSender
   make release
   ```
   ⚠️ Important Build Note - The `make release` command will delete the Syphon framework from the app bundle**, causing the app to fail with a "Library not loaded" error. Check for solution in the next section.
   
8. **Run the app**
   ```bash
   make run
   ```
   or simply go to bin/kinectSyphonSender.app and double click on the app
   
## ⚠️ Important Build Note

**The `make release` command will delete the Syphon framework from the app bundle**, causing the app to fail with a "Library not loaded" error. Here are solutions:

### Solution 1: Use a Build Script (Recommended)
Create a file called `build_with_syphon.sh`:
```bash
#!/bin/bash
make release
mkdir -p bin/kinectSyphonSender.app/Contents/Frameworks
cp -R addons/ofxSyphon/libs/Syphon.framework bin/kinectSyphonSender.app/Contents/Frameworks/
```

Make it executable and use it instead of `make release`:
```bash
chmod +x build_with_syphon.sh
./build_with_syphon.sh
```

### Solution 2: Manual Copy After Each Build
After running `make release`, manually copy the framework:
```bash
mkdir -p bin/kinectSyphonSender.app/Contents/Frameworks
cp -R addons/ofxSyphon/libs/Syphon.framework bin/kinectSyphonSender.app/Contents/Frameworks/
```

### Solution 3: Modify Makefile
Add this to your `Makefile` after the existing content:
```make
after: 
	@mkdir -p bin/$(APPNAME).app/Contents/Frameworks
	@cp -R addons/ofxSyphon/libs/Syphon.framework bin/$(APPNAME).app/Contents/Frameworks/
```

## Usage

1. **Connect your Kinect sensor** to your Mac via USB
2. **Run the application:**
   ```bash
   make run
   ```
3. The app will start capturing Kinect data and broadcasting it via Syphon
4. **Receive the stream** in a Syphon client application:
   - MadMapper
   - Resolume Arena/Avenue
   - VDMX
   - Another openFrameworks app with Syphon client
   - Any application that supports Syphon input

## What It Does

This application:
- Connects to Xbox 360 Kinect sensor
- Captures depth and/or RGB camera data
- Processes the Kinect input
- Broadcasts the processed video via Syphon for real-time use in other applications

## Hardware Requirements

- **Kinect Sensor**: Xbox 360 Kinect (Kinect v1)
- **Mac** with USB port
- **Kinect Power Adapter** (if using standalone Kinect, not Xbox-connected)

## Kinect Setup Notes

- Make sure your Kinect is properly connected and powered
- On first run, you may need to install Kinect drivers
- The Kinect LED should be green when properly connected

## Troubleshooting

- **"Library not loaded: Syphon.framework"**: This happens when `make release` removes the Syphon framework. Use one of the solutions above to fix it.
- **Kinect not detected**: Check USB connections and ensure Kinect drivers are installed
- **"libusb not found" or "libfreenect not found"**: Install the system dependencies using the commands in the installation section
- **Build errors with USB/Kinect libraries**: 
  - Make sure libusb and libfreenect are installed
  - Check if ofxKinect addon includes its own libraries in `addons/ofxKinect/libs/`
  - Try rebuilding after installing dependencies
- **No Syphon output**: Check that other Syphon-compatible applications can see the server
- **Build errors**: Ensure both ofxKinect and ofxSyphon addons are properly installed

## Development Workflow

For development, you can use `make run` which doesn't seem to have the framework deletion issue. Use `make release` (with the framework copy solution) only when you need the optimized release build.


## Contributing

Feel free to submit issues and enhancement requests!
