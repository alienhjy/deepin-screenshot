#include "calculaterect.h"

const int padding = 2;
const int ROTATEPOINT_PADDING = 30;
const int TANT_EDGEVALUE = 0.78539;
const int TANT2_EDGEVALUE = 2.35619;
const int MIN_PADDING = 3;
/* get a rect by diagPoints */
QRect diagPointsRect(DiagPoints diagPoints) {
    return QRect(std::min(diagPoints.masterPoint.x(), diagPoints.deputyPoint.x()),
                 std::min(diagPoints.masterPoint.y(), diagPoints.deputyPoint.y()),
                 std::abs(diagPoints.masterPoint.x() - diagPoints.deputyPoint.x()),
                 std::abs(diagPoints.masterPoint.y() - diagPoints.deputyPoint.y()));
}

/* judge whether the point1 is on the point2 or not */
bool pointClickIn(QPointF point2, QPointF point1, int padding) {
    if (point2.x() >= point1.x() - padding && point2.x() <= point1.x() + padding &&
            point2.y() >= point1.y() - padding && point2.y() <= point1.y() + padding) {
        return true;
    } else {
        return false;
    }
}

/* judge whether the point3 is on the segment*/
bool pointOnLine(QPointF point1, QPointF point2, QPointF point3) {
    if (point1.x() == point2.x()) {
           if (point3.x() >= point1.x() - padding && point3.x() <= point1.x() + padding &&
           point3.y() >= std::min(point1.y(), point2.y()) - padding && point3.y() <= std::max(point1.y(), point2.y()) + padding) {
               return true;
           }
       } else {
           qreal k =  (point2.y() - point1.y()) / (point2.x() - point1.x());
           qreal b = point1.y() - point1.x()*k;

           if ( point3.x() >= std::min(point1.x(), point2.x()) -padding && point3.x() <= std::max(point1.x(), point2.x() + padding)
           && point3.y() >= k * point3.x() + b - padding && point3.y() <= k * point3.x() + b + padding) {
               return true;
           }
       }
       return false;
}

/* To determine whether a point on the rectangle*/
bool pointOnRect(DiagPoints diagPoints, QPointF pos) {
    QPointF point1 = diagPoints.masterPoint;
    QPointF point3 = diagPoints.deputyPoint;
    QPointF point2 = QPointF(diagPoints.masterPoint.x(), diagPoints.deputyPoint.y());
    QPointF point4 = QPointF(diagPoints.deputyPoint.x(), diagPoints.masterPoint.y());

    if (pointOnLine(point1, point2, pos) || pointOnLine(point1, point4, pos) ||
            pointOnLine(point2, point3, pos) || pointOnLine(point3, point4, pos)) {
        return true;
    } else {
        return false;
    }
}

/* get the point who splid a distance on a line */
QPointF pointSplid(QPointF point1, QPointF point2, qreal padding) {
    if (point1.x() == point2.x()) {
        return QPointF(0, padding);
    } else {
        qreal tmpX = padding*std::cos(std::atan2(std::abs(point1.y() - point2.y()), std::abs(point1.x() - point2.x())));
        qreal tmpY = padding*std::sin(std::atan2(std::abs(point1.y() - point2.y()), std::abs(point1.x() - point2.x())));
        return QPointF(tmpX, tmpY);
    }
}

/* get the rotate point by four points in a rectangle*/
QPointF getRotatePoint(QPointF point1, QPointF point2, QPointF point3, QPointF point4) {
    QPointF leftPoint = QPointF(0, 0);
    QPointF rightPoint = QPointF(0, 0);
    QPointF rotatePoint = QPointF(0, 0);

    QPointF leftSplidPoint = pointSplid(point1, point2, ROTATEPOINT_PADDING);
    QPointF rightSplidPoint = pointSplid(point3, point4, ROTATEPOINT_PADDING);

    /* first position*/
    if (point2.x() - point4.x() <= 0 && point2.y() - point4.y() >= 0) {
        leftPoint = QPointF(point1.x() - leftSplidPoint.x(), point1.y() - leftSplidPoint.y());
        rightPoint = QPointF(point3.x() - rightSplidPoint.x(), point3.y() - rightSplidPoint.y());
        rotatePoint = QPointF((leftPoint.x() + rightPoint.x())/2, (leftPoint.y() + rightPoint.y())/2);
        return rotatePoint;
    }

    /* second position*/
    if (point2.x() - point4.x() > 0 && point2.y() - point4.y() > 0) {
        leftPoint = QPointF(point1.x() - leftSplidPoint.x(), point1.y() + leftSplidPoint.y());
        rightPoint = QPointF(point3.x() - rightSplidPoint.x(), point3.y() + rightSplidPoint.y());
        rotatePoint = QPointF((leftPoint.x() + rightPoint.x())/2, (leftPoint.y() + rightPoint.y())/2);
        return rotatePoint;
    }

    /* third position*/
    if (point2.x() - point4.x() < 0 && point2.y() - point4.y() < 0) {
        leftPoint = QPointF(point1.x() + leftSplidPoint.x(), point1.y() - leftSplidPoint.y());
        rightPoint = QPointF(point3.x() + rightSplidPoint.x(), point3.y() - rightSplidPoint.y());
        rotatePoint = QPointF((leftPoint.x() + rightPoint.x())/2, (leftPoint.y() + rightPoint.y())/2);
        return rotatePoint;
    }

    /* fourth position*/
    if (point2.x() - point4.x() > 0 && point2.y() - point4.y() < 0) {
        leftPoint = QPointF(point1.x() + leftSplidPoint.x(), point1.y() + leftSplidPoint.y());
        rightPoint = QPointF(point3.x() + rightSplidPoint.x(), point3.y() + rightSplidPoint.y());
        rotatePoint = QPointF((leftPoint.x() + rightPoint.x())/2, (leftPoint.y() + rightPoint.y())/2);
        return rotatePoint;
    }

    return rotatePoint;
}

FourPoints fourPointsOnRect(DiagPoints diagPoints) {
    FourPoints  fourPoints;
    QPointF point1 = QPointF(std::min(diagPoints.masterPoint.x(), diagPoints.deputyPoint.x()),
                           std::min(diagPoints.masterPoint.y(), diagPoints.deputyPoint.y()));
    QPointF point2 = QPointF(std::min(diagPoints.masterPoint.x(), diagPoints.deputyPoint.x()),
                           std::max(diagPoints.masterPoint.y(), diagPoints.deputyPoint.y()));
    QPointF point3 = QPointF(std::max(diagPoints.masterPoint.x(), diagPoints.deputyPoint.x()),
                           std::min(diagPoints.masterPoint.y(), diagPoints.deputyPoint.y()));
    QPointF point4 = QPointF(std::max(diagPoints.masterPoint.x(), diagPoints.deputyPoint.x()),
                           std::max(diagPoints.masterPoint.y(), diagPoints.deputyPoint.y()));

    fourPoints.point1 = point1;
    fourPoints.point2 = point2;
    fourPoints.point3 = point3;
    fourPoints.point4 = point4;

    return fourPoints;
}

/*
 *  this function is get the angle of the mouse'moving*/
/* the angle in point3 */

qreal calculateAngle(QPointF point1, QPointF point2, QPointF point3) {
    if (point1 == point2) {
        return 0;
    }

    qreal a = std::pow(point1.x() - point3.x(), 2) + std::pow(point1.y() - point3.y(), 2);
    qreal b = std::pow(point2.x() - point3.x(), 2) + std::pow(point2.y() - point3.y(), 2);
    qreal c = std::pow(point1.x() - point2.x(), 2) + std::pow(point1.y() - point2.y(), 2);

    qreal angle =std::cos(( a + b - c)/(2*std::sqrt(a)*std::sqrt(b)));
    if (point1.x() <= point3.x() && point1.y() < point3.y()) {
        if (point2.x() < point1.x() || point2.y() > point1.y()) {
            angle = - angle;
        }
    }
    if (point1.x() < point3.x() && point1.y() >= point3.y()) {
        if (point2.x() > point1.x() || point2.y() > point1.y()) {
            angle = - angle;
        }
    }
    if (point1.x() >= point3.x() && point1.y() > point3.y()) {
        if (point2.x() > point1.x() || point2.y() < point1.y()) {
            angle = - angle;
        }
    }
    if (point1.x() > point3.x() && point1.y() <= point3.y()) {
        if (point2.x() < point1.x() || point2.y() < point1.y()) {
            angle = - angle;
        }
    }
    return angle;
}

/* point2 is the rotatePoint, point1 is the centerPoint, point3 is point2 who rotate */
QPointF pointRotate(QPointF point1, QPointF point2, qreal angle) {
    QPointF middlePoint = QPointF(point2.x() - point1.x(), point2.y() - point1.y());
    QPointF tmpPoint = QPointF(middlePoint.x() * std::cos(angle) - middlePoint.y() * std::sin(angle),
                             middlePoint.x() * std::sin(angle) + middlePoint.y() * std::cos(angle));
    QPointF point3 = QPointF(tmpPoint.x() + point1.x(), tmpPoint.y() + point1.y());

    return point3;
}

/* the distance from a point(point3) to a line(point1, point2) */
qreal pointToLineDistance(QPointF point1, QPointF point2, QPointF point3) {
    if (point1.x() == point2.x()) {
        return std::abs(point3.x() - point1.x());
    } else {
        qreal k = (point1.y() - point2.y())/(point1.x() - point2.x());
        qreal b = point1.y() - point1.x() * k;
        return std::abs(point3.x() * k + b - point3.y()) / std::sqrt(std::pow(k, 2) + 1);
    }
}

/* judge the direction of point3 of line(point1, point2) */
qreal pointLineDir(QPointF point1, QPointF point2, QPointF point3) {
    if (point1.x() == point2.x()) {
        if (point3.x() <= point1.x()) {
            return -1;
        } else {
            return 1;
        }
    } else {
        qreal k = (point1.y() - point2.y()) / (point1.x() - point2.x());
        qreal b = point1.y() - point1.x() * k;
        if (point3.y() <= point3.x() * k + b) {
            return -1;
        } else {
            return 1;
        }
    }
 }

/* handle resize of eight points in rectangle */
FourPoints resizePointPosition(QPointF point1, QPointF point2, QPointF point3, QPointF point4,
                          QPointF pos, int key,  bool isShift) {
    FourPoints resizeFPoints;
    qDebug() << "****key" << key;
    if (point1.x() - point2.x() < 0 && point1.y() - point2.y() < 0 &&
    point1.x() - point3.x() < 0 && point1.y() - point3.y() > 0) {
        qDebug() << "first *";
        switch (key) {
        case 0: { resizeFPoints = point1Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 5: { resizeFPoints = point5Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 6: { resizeFPoints = point6Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 7: { resizeFPoints = point7Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 8: { resizeFPoints = point8Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() - point2.x() < 0 && point1.y() - point2.y() > 0 &&
    point1.x() - point3.x() > 0 && point1.y() - point3.y() > 0) {
        qDebug() << "second*";
        switch(key) {
        case 0: { resizeFPoints = point1Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 5: { resizeFPoints = point5Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 6: { resizeFPoints = point6Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 7: { resizeFPoints = point7Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 8: { resizeFPoints = point8Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() - point2.x() > 0 && point1.y() - point2.y() < 0 &&
    point1.x() - point3.x() < 0 && point1.y() - point3.y()) {
        qDebug() << "third*";
        switch(key) {
        case 0: { resizeFPoints = point1Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 5: { resizeFPoints = point5Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 6: { resizeFPoints = point6Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 7: { resizeFPoints = point7Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 8: { resizeFPoints = point8Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() - point2.x() > 0 && point1.y() - point2.y() > 0 &&
    point1.x() - point3.x() > 0 && point1.y() - point3.y() < 0) {
        qDebug() << "fourth*";
        switch(key) {
        case 0: { resizeFPoints = point1Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 5: { resizeFPoints = point5Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 6: { resizeFPoints = point6Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 7: { resizeFPoints = point7Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 8: { resizeFPoints = point8Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() == point2.x() && point1.y() < point2.y() &&
    point1.x() < point3.x() && point1.y() == point3.y()) {
        qDebug() << "fifth *";
        switch(key) {
        qDebug() << "## resize key" << key;
        case 0: { resizeFPoints = point1Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 5: { resizeFPoints = point5Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 6: { resizeFPoints = point6Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 7: { resizeFPoints = point7Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 8: { resizeFPoints = point8Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() < point2.x() && point1.y() == point2.y() &&
    point1.x() == point3.x() && point1.y() < point3.y()) {
        qDebug() << "sixth *";
        switch(key) {
        case 0: { resizeFPoints = point1Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 5: { resizeFPoints = point5Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 6: { resizeFPoints = point6Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 7: { resizeFPoints = point7Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 8: { resizeFPoints = point8Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() < point2.x() && point1.y() == point2.y() &&
    point1.x() == point3.x() && point1.y() > point3.y()) {
        qDebug() << "seventh*";
        switch(key) {
        case 0: { resizeFPoints = point1Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 5: { resizeFPoints = point5Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 6: { resizeFPoints = point6Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 7: { resizeFPoints = point7Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
//        case 8: { resizeFPoints = point8Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
}

/* first point1 */
/* point1 in the first position*/
FourPoints point1Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoint;
    newResizeFPoint.point1 = point1;
    newResizeFPoint.point2 = point2;
    newResizeFPoint.point3 = point3;
    newResizeFPoint.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE &&
    (pos.y() + MIN_PADDING > point2.y() || pos.x() + MIN_PADDING > point3.x() ||
     pointLineDir(point3, point4, pos) == -1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoint;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE &&
      (pos.x() + MIN_PADDING > point2.x() || pos.y() - MIN_PADDING < point3.y() ||
       pointLineDir(point3, point4, pos) == -1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoint;
    }  else {
        if (pointToLineDistance(point4, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoint;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance= pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance= pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance= pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance= pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                newResizeFPoint.point1 = point1;
                newResizeFPoint.point2 = point2;
                newResizeFPoint.point3 = point3;
                newResizeFPoint.point4 = point4;
                return newResizeFPoint;
            } else {
                 qreal distance1 = pointToLineDistance(point1, point2, pos);
                 qreal distance2 = pointToLineDistance(point1, point3, pos);
                 qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {         
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
                } else {
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
                }
                newResizeFPoint.point1 = point1;
                newResizeFPoint.point2 = point2;
                newResizeFPoint.point3 = point3;
                newResizeFPoint.point4 = point4;
                return newResizeFPoint;
            }
            return newResizeFPoint;
        }
    }
}

/* point1 in the second position */
FourPoints point1Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE
    && (pos.y() - MIN_PADDING < point2.y() || pos.x() - MIN_PADDING < point3.x() ||
        pointLineDir(point3, point4, pos) == -1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE
    && pos.y() - MIN_PADDING < point3.y() || pos.x() + MIN_PADDING > point2.x() ||
    pointLineDir(point3, point4, pos) == -1 || pointLineDir(point2, point4, pos) == -1)  {
        return newResizeFPoints;
    } else {
        if (!isShift) {
            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                qreal distance = pointToLineDistance(point1, point2, pos);
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                distance = pointToLineDistance(point1, point3, pos);
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {
                qreal distance = pointToLineDistance(point1, point2, pos);
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                distance = pointToLineDistance(point1, point3, pos);
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                qreal distance = pointToLineDistance(point1, point2, pos);
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                distance = pointToLineDistance(point1, point3, pos);
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == 1) {
                qreal distance = pointToLineDistance(point1, point2, pos);
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                distance = pointToLineDistance(point1, point3, pos);
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                point1 = pos;
            }
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        } else {
            qreal distance1 = pointToLineDistance(point1, point2, pos);
            qreal distance2 = pointToLineDistance(point1, point3, pos);
            qreal distance = std::min(distance1, distance2);
            if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
            } else {
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
            }
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
}

/* point1 in the third position*/
FourPoints point1Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE
    && (pos.x() - MIN_PADDING < point2.x() || pos.y() + MIN_PADDING > point3.y() ||
        pointLineDir(point3, point4, pos) == 1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT2_EDGEVALUE
    && (pos.x() + MIN_PADDING > point3.x() || pos.y() + MIN_PADDING > point2.y()) ||
       pointLineDir(point3, point4, pos) == 1 || pointLineDir(point2, point4, pos) == 1) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point4, point2, pos) < MIN_PADDING || pointToLineDistance(point3,
            point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point1, point3, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                } else {
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                }
                point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
        return newResizeFPoints;
    }
}

/* point1 in the fourth position */
FourPoints point1Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE
    && (pos.y() - MIN_PADDING < point2.y() || pos.x() - MIN_PADDING < point3.x() ||
    pointLineDir(point3, point4, pos) == 1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < - TANT2_EDGEVALUE
    && (pos.y() + MIN_PADDING > point3.y() || pos.y() - MIN_PADDING < point2.x() ||
    pointLineDir(point3, point4, pos) == 1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point4, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add  = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance  = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add  = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance  = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add  = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance  = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add  = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance  = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 =  point2;
                newResizeFPoints.point3 =  point3;
                newResizeFPoints.point4 =  point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point1, point3, pos);
                qreal distance = std::min(distance1, distance2);
               if (pointLineDir(point1, point3, pos) == -1 &&pointLineDir(point1, point2, pos) == 1) {
                   QPointF add = pointSplid(point2, point4, distance);
                   point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                   add = pointSplid(point3, point4, distance);
                   point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
               } else {
                   QPointF add = pointSplid(point2, point4, distance);
                   point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                   add = pointSplid(point3, point4, distance);
                   point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
               }
               point1 = QPointF(point2.x() + point3.x() - point4.x(),
                               point2.y() + point3.y() - point4.y());
               newResizeFPoints.point1 = point1;
               newResizeFPoints.point2 = point2;
               newResizeFPoints.point3 = point3;
               newResizeFPoints.point4 = point4;
               return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point1 in the fifth position */
FourPoints point1Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (pos.y() + MIN_PADDING > point2.y() || pos.x() + MIN_PADDING > point3.x()) {
        return newResizeFPoints;
    } else {
        qreal distanceLeft = pointToLineDistance(point1, point2, pos);
        QPointF addLeft = pointSplid(point2, point4, distanceLeft);
        qreal distanceRight = pointToLineDistance(point1, point3, pos);
        QPointF addRight = pointSplid(point3, point4, distanceRight);
        if (isShift) {
            if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                point2 = QPointF(point2.x() - addLeft.x(), point2.y() + addLeft.y());
                point3 = QPointF(point3.x() - addRight.x(), point3.y() - addRight.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == 1) {
                point2 = QPointF(point2.x() + addLeft.x(), point2.y() - addLeft.y());
                point3 = QPointF(point3.x() - addRight.x(), point3.y() - addRight.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                point2 = QPointF(point2.x() + addLeft.x(), point2.y() - addLeft.y());
                point3 = QPointF(point3.x() + addRight.x(), point3.y() + addRight.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {
                point2 = QPointF(point2.x() + addLeft.x(), point2.y() - addLeft.y());
                point3 = QPointF(point3.x() - addRight.x(), point3.y() - addRight.y());
                point1 = pos;
            }
        } else {
            qreal distance = std::min(distanceLeft, distanceRight);
            QPointF addLeft = pointSplid(point2, point4, distance);
            QPointF addRight = pointSplid(point3, point4, distance);
            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                point2 = QPointF(point2.x() + addLeft.x(), point2.y() +  addLeft.y());
                point3 = QPointF(point3.x() + addRight.x(), point3.y() + addRight.y());
            } else {
                point2 = QPointF(point2.x() - addLeft.x(), point2.y() -  addLeft.y());
                point3 = QPointF(point3.x() - addRight.x(), point3.y() - addRight.y());
            }
            point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}

/* point1 in the sixth position */
FourPoints point1Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/* point1 in the seventh position */
FourPoints point1Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/***************** second point2 *******************/
/* point2 in the first position */
FourPoints point2Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE
    && (pos.x() + MIN_PADDING > point4.x() || pos.y() - MIN_PADDING < point1.y() ||
    pointLineDir(point1, point3, pos) == -1 || pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE
    && (pos.x() - MIN_PADDING < point1.x() || pos.y() - MIN_PADDING < point4.y() ||
        pointLineDir(point1, point3, pos) == -1 || pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point3, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2,point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point2, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                } else {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point2 in the second position */
FourPoints point2Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE
    && (pos.y() + MIN_PADDING > point1.x() || pos.x() - MIN_PADDING < point4.x() ||
    pointLineDir(point1, point3, pos) == 1 || pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE
    && (pos.y() - MIN_PADDING < point4.y() || pos.x() - MIN_PADDING < point1.x() ||
        pointLineDir(point1, point3, pos) == 1 || pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point3, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {         
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add= pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add= pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add= pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add= pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point2, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                }
                point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point2 in the third position */
FourPoints point2Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE
    && (pos.y() + MIN_PADDING > point4.y() || pos.x() + MIN_PADDING > point1.x() ||
    pointLineDir(point1, point3, pos) == -1 || pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT2_EDGEVALUE
    && (pos.y() - MIN_PADDING < point1.y() || pos.x() + MIN_PADDING > point4.x() ||
        pointLineDir(point1, point3, pos) == -1 || pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point3, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == 1&& pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point2, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                } else {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                }
                point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point2 in the fourth position */
FourPoints point2Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE
    && (pos.y() + MIN_PADDING > point1.y() || pos.x() - MIN_PADDING < point4.x() ||
    pointLineDir(point1, point3, pos) == 1 || pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < -TANT2_EDGEVALUE
    && (pos.y() + MIN_PADDING > point4.y() || pos.x() + MIN_PADDING > point1.x() ||
        pointLineDir(point1, point3, pos) == 1 || pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point3, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point2, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                }
                point2 = QPointF(point1.x() + point4.x() - point3.x(),
                                point1.y() + point4.y() - point3.y());
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point2 in the fifth position*/
FourPoints point2Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (pos.y() - MIN_PADDING < point1.y() || pos.x() + MIN_PADDING > point4.x()) {
        return newResizeFPoints;
    } else {
        qreal distanceLeft = pointToLineDistance(point1, point2, pos);
        QPointF addLeft = pointSplid(point1, point3, distanceLeft);
        qreal distanceRight = pointToLineDistance(point2, point4, pos);
        QPointF addRight = pointSplid(point3, point4, distanceRight);
        if (!isShift) {
            if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                point1.setX(pos.x());
                point4.setY(point4.y() + addRight.y());
                point2 = pos;
            }
            if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                point1.setX(pos.x());
                point4.setY(pos.y());
                point2 = pos;
            }
            if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                point1.setX(pos.x());
                point4.setY(pos.y());
                point2 = pos;
            }
            if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                point1.setX(pos.x());
                point4.setY(pos.y());
                point2 = pos;
          }
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        } else {
            qreal distance = std::min(distanceLeft, distanceRight);
            QPointF addLeft = pointSplid(point1, point3, distance);
            QPointF addRight = pointSplid(point3, point4, distance);
            if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                point1 = QPointF(point1.x() + addLeft.x(), point1.y() - addLeft.y());
                point4 = QPointF(point4.x() - addRight.x(), point4.y() - addRight.y());
            } else {
                point1 = QPointF(point1.x() - addLeft.x(), point1.y() + addLeft.y());
                point4 = QPointF(point4.x() + addRight.x(), point4.y() + addRight.y());
            }
            point2 = QPointF(point1.x() + point4.x() - point3.x(),
                             point1.y() + point4.y() - point3.y());
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}

/* point2 in the sixth position */
FourPoints point2Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/* point2 in the seventh position */
FourPoints point2Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/***************** third point3 *******************/
/* point3 in the first position */
FourPoints point3Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE &&
    (pos.x() - MIN_PADDING < point1.x() || pos.y() + MIN_PADDING > point4.y() ||
     pointLineDir(point1, point2, pos) == 1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE &&
     (pos.x() + MIN_PADDING > point4.x() || pos.y() + MIN_PADDING > point1.y() ||
      pointLineDir(point1, point2, pos) == 1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING || pointToLineDistance(point1, point2, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point3, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                    point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                }
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point3 in the second position */
FourPoints point3Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE &&
    (pos.x() + MIN_PADDING > point1.x() || pos.y() - MIN_PADDING < point4.y() ||
     pointLineDir(point1, point2, pos) == 1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE &&
     (pos.x() + MIN_PADDING > point4.x() || pos.y() + MIN_PADDING > point1.y() ||
      pointLineDir(point1, point2, pos) == 1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING || pointToLineDistance(point1, point2, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {     
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point3, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                }
                point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point3 in the third position */
FourPoints point3Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE &&
    (pos.x() - MIN_PADDING < point4.x() || pos.y() - MIN_PADDING < point1.y() ||
     pointLineDir(point1, point2, pos) == -1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT2_EDGEVALUE &&
     (pos.x() - MIN_PADDING < point1.x() || pos.y() + MIN_PADDING > point4.y() ||
      pointLineDir(point1, point2, pos) == -1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point2, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point3, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                //TODO: need to be checked.
                QPointF addLeft = pointSplid(point1, point2, distance);
                QPointF addRight = pointSplid(point2, point4, distance);
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    point1 = QPointF(point1.x() - addLeft.x(), point2.y() + addLeft.y());
                    point4 = QPointF(point4.x() - addRight.x(), point4.y() - addRight.y());
                } else {
                    point1 = QPointF(point1.x() + addLeft.x(), point1.y() - addLeft.y());
                    point4 = QPointF(point4.x() + addRight.x(), point4.y() + addRight.y());
                }
                point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point3 in the fourth position */
FourPoints point3Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE &&
    (pos.y() - MIN_PADDING < point4.y() || pos.x() + MIN_PADDING > point1.x() ||
     pointLineDir(point1, point2, pos) == -1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < -TANT2_EDGEVALUE &&
     (pos.y() - MIN_PADDING < point1.y() || pos.x() - MIN_PADDING < point4.x() ||
      pointLineDir(point1, point2, pos) == -1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point2, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add  = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add  = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add  = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add  = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point3, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);

                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point2, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point2, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                }
                point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point3 in the fifth position */
FourPoints point3Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (pos.y() + MIN_PADDING > point4.y() || pos.x() - MIN_PADDING < point1.x()) {
        return newResizeFPoints;
    } else {
        qreal distanceLeft = pointToLineDistance(point3, point4, pos);
        QPointF addLeft = pointSplid(point2, point4, distanceLeft);
        qreal distanceRight = pointToLineDistance(point1, point3, pos);
        QPointF addRight = pointSplid(point1, point3, distanceRight);
        if (!isShift) {
            if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point1, point3, pos) == -1) {
                point4.setX(pos.x());
                point1.setY(pos.y());
                point3 = pos;
            }
            if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point1, point3, pos) == -1) {
                point4.setX(pos.x());
                point1.setY(pos.y());
                point3 = pos;
            }
            if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point1, point3, pos) == 1) {
                point4.setX(pos.x());
                point1.setY(pos.y());
                point3 = pos;
            }
            if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point1, point3, pos) == 1) {
                point4.setX(pos.x());
                point1.setY(pos.y());
                point3 = pos;
            }
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        } else {
            qreal distance = std::min(distanceLeft, distanceRight);
            QPointF addLeft = pointSplid(point2, point4, distance);
            QPointF addRight = pointSplid(point1, point2, distance);
            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                point4 = QPointF(point4.x() - addLeft.x(), point4.y() - addLeft.y());
                point1 = QPointF(point1.x() - addRight.x(), point1.y() + addRight.y());
            } else {
                point4 = QPointF(point4.x() + addLeft.x(), point4.y() + addLeft.y());
                point1 = QPoint(point1.x() + addRight.x(), point1.y() - addRight.y());
            }
            point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}
/* point3 in the sixth position */
FourPoints point3Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}
/* point3 in the seventh position */
FourPoints point3Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/***************** fourth point4 *******************/
/* point4 in the first position */
FourPoints point4Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE &&
    (pos.x() - MIN_PADDING < point2.x() || pos.y() - MIN_PADDING < point3.y() ||
     pointLineDir(point1, point2, pos) == 1 || pointLineDir(point1, point3, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE &&
     (pos.x() - MIN_PADDING < point3.x() || pos.y() + MIN_PADDING > point2.y() ||
      pointLineDir(point1, point2, pos) == 1 || pointLineDir(point1, point3, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point2, point4, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance =  std::min(distance1, distance2);
                if (pointLineDir(point2, point4, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point1.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point1.y());
                }
                point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point4 in the second position */
FourPoints point4Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE &&
    (pos.y() + MIN_PADDING > point3.y() || pos.x() + MIN_PADDING > point2.x() ||
     pointLineDir(point1, point2, pos) == 1 || pointLineDir(point1, point3, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE &&
     (pos.y() + MIN_PADDING > point2.y() || pos.x() - MIN_PADDING < point3.x() ||
      pointLineDir(point1, point2, pos) == 1 || pointLineDir(point1, point3, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {  
            if (!isShift) {
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point2, point4, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point2, point4, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                }
                point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point4 in the third position */
FourPoints point4Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE &&
    (pos.y() - MIN_PADDING < point2.y() || pos.x() + MIN_PADDING > point3.x() ||
     pointLineDir(point1, point2, pos) == -1 || pointLineDir(point1, point3, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT2_EDGEVALUE &&
     (pos.y() - MIN_PADDING < point3.y() || pos.x() - MIN_PADDING < point2.x() ||
      pointLineDir(point1, point2, pos) == -1 || pointLineDir(point1, point3, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point2, point4, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point2, point4, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                }
                point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point4 in the third position */
FourPoints point4Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE &&
    (pos.x() + MIN_PADDING > point2.x() || pos.y() + MIN_PADDING > point3.y() ||
     pointLineDir(point1, point2, pos) == -1 || pointLineDir(point1, point3, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < -TANT2_EDGEVALUE &&
     (pos.x() + MIN_PADDING > point3.x() || pos.y() - MIN_PADDING < point2.y() ||
      pointLineDir(point1, point2, pos) == -1 || pointLineDir(point1, point3, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point2,  point4, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point2, point4, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                }
                point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point4 in the fifth position */
FourPoints point4Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (pos.y() - MIN_PADDING < point3.y() || pos.x() - MIN_PADDING < point2.x()) {
        return newResizeFPoints;
    } else {
        if (!isShift) {
            qreal distanceLeft = pointToLineDistance(point2, point4, pos);
            QPointF addLeft = pointSplid(point1, point2, distanceLeft);
            qreal distanceRight = pointToLineDistance(point3, point4, pos);
            QPointF addRight = pointSplid(point1, point3, distanceRight);
            if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                point2.setY(pos.y());
                point3.setX(pos.x());
                point4 = pos;
            }
            if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                point2.setY(pos.y());
                point3.setX(pos.x());
                point4 = pos;
            }
            if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                point2.setY(pos.y());
                point3.setX(pos.x());
                point4 = pos;
            }
            if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                point2.setY(pos.y());
                point3.setX(pos.x());
                point4 = pos;
            }
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}
/* point4 in the sixth position */
FourPoints point4Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}
/* point4 in the seventh position */
FourPoints point4Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}
