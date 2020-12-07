# Image Compression and Highlighting
### Austin Hester
### CS 5420 - Computer Vision
### UMSL FS2020, Prof. Sanjiv Bhatia

----
## Purpose

This project has two programs. One program requires you to highlight a specific area of a grayscale image. The second program
requires you to compute the Huffman code for a grayscale image and to compute the compression ratio achieved.

The main purpose of the first program is to get familiar with mouse-based operations in OpenCV.

----

## Image Highlighting

Image highlighting is generally used to bring attention of a viewer to a specific area of an image while dimming the rest of the
image to provide context.

#### Task

Write a function to display an image. On the image, select a rectangular area by using your mouse. You can start the selection
by using the left click of the mouse and keeping it pressed. Move the mouse to a different point and release the mouse button. It
will be nice if you can display the rectangle that is being used to select the area but do not show the rectangle once you release
the mouse.

Your program should capture the top left and bottom right coordinates of the rectangle. These points will be used
to select the region of interest (ROI) that needs to be highlighted.

After the rectangle is selected, make a copy of the ROI and dim the entire image by multiplying it by 0.75. Perform histogram
equalization on the ROI and insert the equalized image back into its position. Display the final image.

----

#### Usage

```
Usage: highlight.exe [params] filename

        -g, --grayscale
                read grayscale
        -h, --help
                show help message

        filename (value:<none>)
                input image

Shortcuts:
    s       - save image
    c       - clear selection
    q       - quit

```

----

## Image Compression

Compute the Huffman code for a given grayscale image. Output different code blocks (in binary) and determine the compression
ratio achieved.

#### Task

Your program will take a grayscale image as input. It will compute the histogram of the grayscale image and use the probabilities
to find the Huffman code in binary. You may use bitwise operations to compute the code or compute them as binary strings
(using std::string class). If you use bitwise operators, please create it as an ordered pair or a struct with one part
containing the length of the code and the second part containing the actual code. In either case, it will be useful to write a
function to display the code, especially taking care of preceding zeroes.

Compute the average length of code blocks in number of bits and the compression ratio achieved. Also display the average
code length achieved by Shannon’s theorem.

Note that you are not required to create/save the compressed image; your code will just create the lookup table for code blocks.
You should be able to count the number of bits in each code block.

----

#### Usage

```
Usage: huffman.exe [params] filename

        -h, --help
                show help message

        filename (value:<none>)
                input image

```
