// highlight.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <iostream>

#include "./include/cla_parse.hpp"
#include "./include/dir_func.hpp"
#include "./include/img_struct.hpp"
#include "./include/string_helper.hpp"


#define WINDOW_NAME "Highlight"

// CLA variable
std::string input_image;

const uint INTENSITY_VALUES = 256;

img_struct_t* og_image;
cv::Mat displayed_image;

// SelectionStage
struct SelectionState {
  cv::Point selection_top_left, selection_bottom_right, mouse_pos;
  bool started = false, done = false;

  cv::Rect to_rect() {
    return cv::Rect (
      std::min(this->selection_top_left.x, this->mouse_pos.x),
      std::min(this->selection_top_left.y, this->mouse_pos.y),
      std::abs(this->selection_top_left.x-this->mouse_pos.x),
      std::abs(this->selection_top_left.y-this->mouse_pos.y));
  }
} state;


// 'event loop' for keypresses
int
wait_key()
{
    char key_pressed = cv::waitKey(0) & 255;
    // 's' saves the current image
    if (cv::getWindowProperty(WINDOW_NAME, cv::WND_PROP_VISIBLE) < 1) {
        // this ends the program if window is closed
        return 0;
    }
    if (key_pressed == 's') {
        if (!displayed_image.empty()) {
            write_img_to_file(
                displayed_image,
                "./out",
                "output_" + std::to_string(state.selection_top_left.x) +
                    "_" + std::to_string(state.selection_bottom_right.y) +
                    input_image
            );
            cv::destroyAllWindows();
            return 0;
        }
    }
    // 'q' or  <escape> quits out
    if (key_pressed == 27 || key_pressed == 'q') {
        cv::destroyAllWindows();
        return 0;
    }
    return 1;
}


// draw_rectangle on image
void
draw_rectangle()
{
    // deep keep to displayed_image, to only keep one rectangle one screen
    og_image->image.copyTo(displayed_image); // comment this out to draw a bunch of rectangles!

    // draw the rectangle on screen
    cv::rectangle(displayed_image, state.to_rect(), cv::Scalar(6));

    // display the new image
    cv::imshow(WINDOW_NAME, displayed_image);
}


void
mouse_callback(int event, int x, int y, int, void* )
{
    // https://gist.github.com/guimeira/541e9056364b9491452b7027f12536cc
    switch(event) {
        case cv::EVENT_LBUTTONDOWN:
            state.selection_top_left.x = x;
            state.selection_top_left.y = y;
            state.mouse_pos.x = x;
            state.mouse_pos.y = y;
            state.started = true;
            break;

        case cv::EVENT_LBUTTONUP:
            state.started = false;
            state.selection_bottom_right.x = x;
            state.selection_bottom_right.y = y;
            state.done = true;
            // draw final rectangle
            draw_rectangle();
            break;

        case cv::EVENT_MOUSEMOVE:
            state.mouse_pos.x = x;
            state.mouse_pos.y = y;
            if (state.started) // draw rectangle in real time
                draw_rectangle();
            break;
    }
}


int
main(int argc, const char** argv)
{

    // parse and save command line args
    int parse_result = parse_arguments(
        argc, argv,
        &input_image
    );
    if (parse_result != 1) return parse_result;

    assert(input_image.length() > 0);

    // open image, grayscale = false
    og_image = open_image(input_image.c_str(), false);

    assert(og_image != NULL);

    // deep keep to displayed_image
    og_image->image.copyTo(displayed_image);

    // display the original image
    cv::imshow(WINDOW_NAME, displayed_image);

    cv::setMouseCallback(WINDOW_NAME, mouse_callback);

    // 'event loop' for keypresses
    while (wait_key());

    og_image->image.release();
    displayed_image.release();

    return 0;
}
