#include "fmatching.h"

void featureMatching(const cv::Mat& img1Rectified, const cv::Mat& img2Rectified) {
    // Detecter les points d'interet et calculer les descripteurs ORB
    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;

    orb->detectAndCompute(img1Rectified, cv::Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(img2Rectified, cv::Mat(), keypoints2, descriptors2);

    // Correspondance des descripteurs avec BFMatcher
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    // Extraire les points correspondant aux matches
    std::vector<cv::Point2f> pts1, pts2;
    for (const auto& match : matches) {
        pts1.push_back(keypoints1[match.queryIdx].pt);
        pts2.push_back(keypoints2[match.trainIdx].pt);
    }

    // Appliquer RANSAC pour trier
    std::vector<uchar> inliersMask;
    cv::Mat F = cv::findFundamentalMat(pts1, pts2, cv::FM_RANSAC, 3.0, 0.99, inliersMask);

    // Conserver uniquement les bons matches (inliers)
    std::vector<cv::DMatch> goodMatches;
    for (size_t i = 0; i < matches.size(); i++) {
        if (inliersMask[i]) {
            goodMatches.push_back(matches[i]);
        }
    }

    std::cout << "Nombre de correspondances retenues apres RANSAC : " << goodMatches.size() << std::endl;

    // Affichage des matches
    cv::Mat imgMatches;
    cv::drawMatches(img1Rectified, keypoints1, img2Rectified, keypoints2, goodMatches, imgMatches,
        cv::Scalar::all(-1), cv::Scalar::all(-1),
        std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    cv::imshow("Matching avec RANSAC", imgMatches);
    cv::waitKey(0);
}