#ifndef QOPENCVHELPERS_H
#define QOPENCVHELPERS_H

#include <QLineF>

#include <opencv2/opencv.hpp>

/**
 * @brief Convert OpenCV line represented by radius,angle to Qt line represented by starting and ending points
 *
 * The starting and ending points are defined to lie on an rectangle with origin (0,0) and size (width,height).
 *
 * @param line OpenCV line in radius,angle representation
 * @param width Width of the bounding box on which the end points lie
 * @param height Height of the bounding box on which the end points lie
 * @return QLineF with the calculated starting and ending points
 */
QLineF cvline2qlinef(const cv::Vec2f& line, float width, float height);

#endif // QOPENCVHELPERS_H
