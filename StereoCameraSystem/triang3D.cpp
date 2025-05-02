#include "triang3D.h"

void triang3DPoints(
    const std::vector<cv::DMatch>& goodMatches,
    const std::vector<cv::KeyPoint>& keypoints1,
    const std::vector<cv::KeyPoint>& keypoints2,
    const cv::Mat& P1,
    const cv::Mat& P2
) {
    //Extrait les points 2D des bons matches
    std::vector<cv::Point2f> pts1, pts2;
    pts1.reserve(goodMatches.size());
    pts2.reserve(goodMatches.size());
    for (const auto& m : goodMatches) {
        pts1.push_back(keypoints1[m.queryIdx].pt);
        pts2.push_back(keypoints2[m.trainIdx].pt);
    }

    //Triangulation homogène (4xN)
    cv::Mat points4D;
    cv::triangulatePoints(P1, P2, pts1, pts2, points4D);

    // Conversion en points 3D
    std::vector<cv::Point3f> cloud;
    cloud.reserve(points4D.cols);
    for (int i = 0; i < points4D.cols; ++i) {
        cv::Mat col = points4D.col(i);
        col /= col.at<float>(3);
        cloud.emplace_back(
            col.at<float>(0),
            col.at<float>(1),
            col.at<float>(2)
        );
    }

    //Affichage du nuage
    //cv::viz::Viz3d vizWindow("Nuage de points 3D");
    //vizWindow.showWidget("Cloud", cv::viz::WCloud(cloud));
    //vizWindow.spin();
}