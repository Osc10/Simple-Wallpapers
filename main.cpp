#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>

using namespace std;

void watercolor(cv::Mat image, cv::Mat &result);
void sharpen(cv::Mat &image);
void display(cv::Mat &image);
void edge_mask(cv::Mat image, cv::Mat &edges);
void smooth_edges(cv::Mat &image);
int counter = 0;

int main()
{
    cv::Mat image;
    image = cv::imread("test.jpg", CV_LOAD_IMAGE_COLOR);

    if(!image.data)
    {
        cout << "Error: Could not find or open image." << endl;
        return -1;
    }

    cv::Mat layer2;
    cv::Mat edges;
    cv::Mat layer3;
    cv::Mat layer4;

    watercolor(image, layer2);
    edge_mask(layer2, edges);

    layer2.copyTo(layer3);
    sharpen(layer3);

    layer3.copyTo(layer2, edges);

    watercolor(layer2, layer4);

    layer3.copyTo(layer4, edges);

    sharpen(layer4);
    display(layer4);

    cv::waitKey(0);
    return 0;
}

void watercolor(cv::Mat image, cv::Mat &result)
{
    cv::bilateralFilter(image, result, 10, 90, 10);
}

void sharpen(cv::Mat &image)
{
    cv::Mat blur;
    cv::GaussianBlur(image, blur, cv::Size(0, 0), 3);
    cv::addWeighted(image, 1.8, blur, -0.8, 0, image);
}

int threshold1 = 15;

void edge_mask(cv::Mat image, cv::Mat &edges)
{
    cv::blur(image, edges, cv::Size(5, 5));
    cv::Canny(edges, edges, threshold1, 3 * threshold1);
}

void smooth_edges(cv::Mat &image)
{
    cv::Mat element = cv::getStructuringElement(0, cv::Size(2, 2));
    cv::dilate(image, image, element);
}

void display(cv::Mat &image)
{
    string name = "image" + to_string(counter);
    counter++;
    cv::namedWindow(name.c_str() , CV_WINDOW_NORMAL);
    cv::imshow(name.c_str(), image);
}
