/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <array>

#include <nqlogger.h>

#include <nvector3D.h>
#include <npoint3D.h>
#include <ndirection3D.h>
#include <nplane3D.h>
#include <nline3D.h>

double imageScale(double focalLength)
{
  double p0 = -0.0240888;
  double p1 = 0.00148648;
  double p2 = -6.91494e-06;

  return p0 + p1 * focalLength + p2 * focalLength * focalLength;
}

int main(int argc, char ** argv)
{
  NQLogger::instance()->addActiveModule("*");
  NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);

  {
    double f = 50.0;
    double gamma = imageScale(f);
    double imageDistance = f * (gamma + 1.0);
    double objectDistance = imageDistance / gamma;

    NVector3D height1(0., 0., 1720.); // HEIGHT1
    NVector3D height2(0., 0., 150.);  // HEIGHT2
    NVector3D distance(0., -1.0*437., 0.); // DISTANCE
    double angle1 = 20.9 * M_PI / 180.;
    double angle2 = 16.2 * M_PI / 180.;
    double angle3 =  0.4 * M_PI / 180.;

    distance.print();
    distance.rotateX(angle2);
    distance.print();

    std::cout << std::endl;

    NPoint3D cameraPoint(0., 0., 0.);
    cameraPoint.print();
    cameraPoint.move(height1);
    cameraPoint.move(distance);
    cameraPoint.print();

    std::cout << std::endl;

    NPoint3D objectPoint(0., 0., 0.);
    objectPoint.move(height2);
    NDirection3D objectNormal(0., 0., 1.);
    NPlane3D objectPlane(objectPoint, objectNormal);

    NDirection3D centerRayDirection(0., 0., -1.);
    centerRayDirection.print();
    centerRayDirection.rotateX(angle2 + angle3);
    centerRayDirection.print();

    std::cout << std::endl;

    NLine3D centerRay(cameraPoint, centerRayDirection);
    objectPoint.print();
    centerRay.intersection(objectPlane, objectPoint);
    objectPoint.print();

    std::cout << std::endl;

    NVector3D imageDistanceVector(objectPoint, cameraPoint);
    imageDistanceVector.print();
    imageDistanceVector *= objectDistance / imageDistanceVector.length();
    imageDistanceVector.print();

    std::cout << std::endl;

    NPoint3D imagePoint(objectPoint);
    imagePoint.print();
    imagePoint.move(imageDistanceVector);
    imagePoint.print();

    std::cout << std::endl;

    NPoint3D gridPoint(0., 0., 0.);
    gridPoint.move(height1);

    NDirection3D gridNormal(0., 0., -1.);
    gridNormal.print();
    gridNormal.rotateX(-angle1);
    gridNormal.print();

    std::cout << std::endl;

    NPlane3D gridPlane(gridPoint, gridNormal);

    NPoint3D objectIntersection;
    NPoint3D gridIntersection;
    NDirection3D imageBeamDirection(0., 0., -1.);
    imageBeamDirection.rotateX(angle2 + angle3);
    NLine3D imageBeam(imagePoint, imageBeamDirection);
    imageBeam.intersection(objectPlane, objectIntersection);

    NDirection3D gridBeamDirection(imageBeamDirection);
    gridBeamDirection.rotateZ(M_PI);
    NLine3D gridBeam(objectIntersection, gridBeamDirection);
    gridBeam.intersection(gridPlane, gridIntersection);

    objectIntersection.print();
    gridIntersection.print();
  }

  return 0;
}
