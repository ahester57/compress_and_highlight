// highlight.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <iostream>

#include "./include/cla_parse.hpp"
#include "./include/dim_image.hpp"
#include "./include/dir_func.hpp"
#include "./include/hsv_convert.hpp"
#include "./include/img_struct.hpp"


const std::string WINDOW_NAME = "Highlight";

// CLA variables
std::string input_image;
bool grayscale;

const float dim_array[] = { 1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, -111.11, -22.22, -11.11, 111.11, 22.22, 11.11, 0 };
float dim_factor = 1;
int slider_dim_value = 0;

img_struct_t* og_image;
cv::Mat displayed_image;
cv::Mat hsv_image;


// SelectionStage
struct SelectionState {
    cv::Point selection_top_left, selection_bottom_right, mouse_pos;
    bool started = false, done = false;
    uint step = 0;

    cv::Rect
    to_rect() {
        return cv::Rect (
            std::min(this->selection_top_left.x, this->mouse_pos.x),
            std::min(this->selection_top_left.y, this->mouse_pos.y),
            std::abs(this->selection_top_left.x - this->mouse_pos.x),
            std::abs(this->selection_top_left.y - this->mouse_pos.y)
        );
    }

    void
    reset() {
        this->selection_top_left = cv::Point(0, 0);
        this->selection_bottom_right = cv::Point(0, 0);
        this->mouse_pos = cv::Point(0, 0);
        this->started = false;
        this->done = false;
        this->step = 0;
    }

} state;


// 'event loop' for keypresses
int
wait_key()
{
    char key_pressed = cv::waitKey(0) & 255;
    if (cv::getWindowProperty(WINDOW_NAME, cv::WND_PROP_VISIBLE) < 1) {
        // this ends the program if window is closed
        return 0;
    }
    // 'o' displays the original image
    if (key_pressed == 'o') {
        og_image->image.copyTo(displayed_image);
        // reset selection state
        state.reset();
        cv::imshow(WINDOW_NAME, displayed_image);
    }
    // 's' saves the current image
    if (key_pressed == 's') {
        if (!displayed_image.empty()) {
            write_img_to_file(
                displayed_image,
                "./out",
                "output_" + std::string(grayscale ? "grayscale_" : "color_") +
                            std::to_string(state.selection_top_left.x) + "_" +
                            std::to_string(state.selection_bottom_right.y) +
                            "/" + input_image
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


// save the ROI to a cv::Mat
cv::Mat
extract_roi(cv::Mat src, cv::Rect rect)
{
    cv::Mat dst;
    src(rect).copyTo(dst);
    return dst;
}


// grayscale processing, return equalized ROI
cv::Mat
process_grayscale()
{
    // deep copy original to displayed_image
    og_image->image.copyTo(displayed_image); // comment this out to draw a bunch of rectangles!

    // save the region of interest
    cv::Mat roi = extract_roi(displayed_image, state.to_rect());

    dim_grayscale_image(displayed_image, dim_factor);

    // equalize region of interest
    cv::Mat equalized_roi;
    cv::equalizeHist(roi, equalized_roi);

    roi.release();
    return equalized_roi;
}

// color processing, return equalized ROI
cv::Mat
process_color()
{
    // deep copy original HSV to displayed_image
    hsv_image.copyTo(displayed_image);

    // save the region of interest in HSV
    cv::Mat roi = extract_roi(displayed_image, state.to_rect());

    // HSV dimming placeholder
    dim_hsv_image(displayed_image, dim_factor);

    // displayed image back to BGR
    hsv_to_bgr(displayed_image, &displayed_image);

    // HSV equalizer
    cv::Mat equalized_roi;

    // 1. split the original image into 3 hsv channels
    cv::Mat hsv_values[3];
    cv::split(roi, hsv_values);
    std::vector<cv::Mat> channels = { hsv_values[0], hsv_values[1], hsv_values[2] };

    // 2. equalize the image
    cv::Mat hue_equalized;
    cv::equalizeHist(channels[2], hue_equalized);
    channels[2] = hue_equalized;

    // 3. merge channels back together
    cv:merge(channels, equalized_roi);

    // 4. convert equalized ROI to BGR
    hsv_to_bgr(equalized_roi, &equalized_roi);

    hue_equalized.release();
    roi.release();
    return equalized_roi;
}


// post complete rectangle
void
on_rect_complete()
{
    if (state.to_rect().area() == 0) return;

    // dim the image (in real_time)
    cv::Mat equalized_roi;
    if (grayscale) {
        // grayscale processing
        equalized_roi = process_grayscale();
    } else {
        // color processing (BGR -> HSV)
        equalized_roi = process_color();
    }

    // insert ROI into displayed image
    equalized_roi.copyTo(displayed_image(state.to_rect()));
    equalized_roi.release();

    // show the final product
    cv::imshow(WINDOW_NAME, displayed_image);

}


// trackbar for dim level
static void
on_trackbar_dim_level(int, void*)
{
    dim_factor = dim_array[slider_dim_value];
    on_rect_complete();
}


void
mouse_callback(int event, int x, int y, int, void*)
{
    // https://gist.github.com/guimeira/541e9056364b9491452b7027f12536cc (modified move)
    switch (event) {
        case cv::EVENT_LBUTTONDOWN:
            // reset selection state
            state.reset();
            state.selection_top_left.x = x;
            state.selection_top_left.y = y;
            state.mouse_pos.x = x;
            state.mouse_pos.y = y;
            state.done = false;
            state.started = true;
            break;
        case cv::EVENT_LBUTTONUP:
            state.selection_bottom_right.x = x;
            state.selection_bottom_right.y = y;
            state.started = false;
            on_rect_complete();
            state.done = true;
            break;
        case cv::EVENT_MOUSEMOVE:
            // don't listen for anything if not started
            if (!state.started) break;
            // listen only every X times ( increase X if your PC is slow )
            if (++state.step % 2 == 0) break;

            // only allow mouse_pos to handle changes inside display
            if (x < 0) x = 0; else if (x >= displayed_image.cols) x = displayed_image.cols - 1;
            state.mouse_pos.x = x;

            if (y < 0) y = 0; else if (y >= displayed_image.rows) y = displayed_image.rows - 1;
            state.mouse_pos.y = y;

             // draw rectangle in real time
            on_rect_complete();
            break;
    }
}


int
main(int argc, const char** argv)
{
    // parse and save command line args
    int parse_result = parse_arguments(
        argc, argv,
        &input_image,
        &grayscale
    );
    if (parse_result != 1) return parse_result;

    assert(input_image.length() > 0);

    // open image, grayscale = true
    og_image = open_image(input_image.c_str(), grayscale);

    assert(og_image != NULL);

    // deep keep to displayed_image
    og_image->image.copyTo(displayed_image);

    // initialize HSV image if using color
    if (!grayscale) {
        bgr_to_hsv(displayed_image, &hsv_image);
    }

    // display the original image
    cv::imshow(WINDOW_NAME, displayed_image);

    cv::createTrackbar("Dim Level", WINDOW_NAME, &slider_dim_value, 16, on_trackbar_dim_level);
    cv::setMouseCallback(WINDOW_NAME, mouse_callback);

    // 'event loop' for keypresses
    while (wait_key());

    og_image->image.release();
    delete og_image;

    hsv_image.release();
    displayed_image.release();

    return 0;
}
