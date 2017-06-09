#include "QOpenCVHelpers.h"

QLineF cvline2qlinef(const cv::Vec2f& line, float width, float height)
{
    if((-CV_PI/4<line[1] && line[1]<CV_PI/4) || (3*CV_PI/4<line[1] && line[1]<5*CV_PI/4))
        return QLineF(line[0]/cos(line[1]),0, -sin(line[1])/cos(line[1])*height+line[0]/cos(line[1]),height);
    else
        return QLineF(0,line[0]/sin(line[1]), width ,-cos(line[1])/sin(line[1])*width +line[0]/sin(line[1]));

}
