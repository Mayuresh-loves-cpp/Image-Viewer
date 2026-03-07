# Image Viewer

A lightweight and efficient image viewer utility written entirely in C. It uses SDL2 library to display images. This code is minimalistic while providing functionality for basic image viewing.

Currenty only supports `.png, .jpg, .jpeg` files. More support comming soon!

### Installing Dependencies

To build and run the Image Viewer, you need the following libraries installed on your system:

- A C compiler (e.g., `gcc` or `clang`)
- SDL2 library
- libpng (for PNG image support)
- libjpeg (for JPEG image support)

On Debian-based Linux distributions, you can install the required libraries using:

```bash
sudo apt-get install libsdl2-dev libpng-dev libjpeg-dev
```

For other platforms, refer to the respective installation guides:

- [SDL2 Installation Guide](https://wiki.libsdl.org/Installation)
- [libpng Home Page](http://www.libpng.org/pub/png/libpng.html)
- [libjpeg Documentation](http://libjpeg.sourceforge.net/)

## Building the Project

1. Clone the repository:

   ```bash
   git clone https://github.com/Mayuresh-loves-cpp/Image-Viewer.git
   cd Image-Viewer
   ```
2. Build the project using following command:

   ```bash
   make
   ```
3. The compiled binary will be generated in `"./bin"` directory.

## Running the Application

To run the Image Viewer, use the following command:

```bash
cd ./bin
./iv <path-to-image>
```

Replace `<path-to-image>` with the path to the image file you want to view.

## Project Structure

```
Image-Viewer/
├── iv.c                # Main source file containing the application logic
├── png_util.h          # Utility functions for handling PNG images
├── other_util.h        # Additional utility functions
├── Makefile            # Build script for compiling the project
├── LICENSE             # Project license file (GPL v2)
├── README.md           # Project documentation
├── assets/             # Directory for storing sample images
```

## License

This project is licensed under the GNU General Public License v2.0. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [SDL2](https://www.libsdl.org/) for providing a powerful and easy-to-use multimedia library.
- [libpng](http://www.libpng.org/pub/png/libpng.html) for enabling PNG image support.
- [libjpeg](http://libjpeg.sourceforge.net/) for enabling JPEG image support.
