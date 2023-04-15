#include <iostream>
#include <opencv2/opencv.hpp>

// Function to calculate the rotation error in degrees
double rotationError(const cv::Mat& rvec_ground_truth, const cv::Mat& rvec) {
    cv::Mat R_ground_truth, R;
    cv::Rodrigues(rvec_ground_truth, R_ground_truth);
    cv::Rodrigues(rvec, R);

    cv::Mat R_error = R_ground_truth.t() * R;
    cv::Vec3d rvec_error;
    cv::Rodrigues(R_error, rvec_error);

    return cv::norm(rvec_error) * 180.0 / CV_PI;
}

// Function to calculate the translation error in units (e.g., meters)
double translationError(const cv::Mat& tvec_ground_truth, const cv::Mat& tvec) {
    return cv::norm(tvec_ground_truth - tvec);
}

int main(int argc, char** argv) {
    // Define the 3D points of the object's corners (assuming a known 3D model)
    std::vector<cv::Point3f> object_points = {
        {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}
    };

    // Define the 2D points of the object's corners in the image (manually or via a feature detector)
    std::vector<cv::Point2f> image_points = {
        {100, 100}, {200, 100}, {200, 200}, {100, 200}
    };

    // Define camera parameters (use real values for your camera)
    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << 800, 0, 320, 0, 800, 240, 0, 0, 1);
    cv::Mat dist_coeffs = (cv::Mat_<double>(1, 5) << 0, 0, 0, 0, 0);

    // Define ground truth camera transformation
    cv::Mat rvec_ground_truth = (cv::Mat_<double>(3, 1) << 0.1, 0.2, 0.3);
    cv::Mat tvec_ground_truth = (cv::Mat_<double>(3, 1) << 0.5, 0.5, 1.0);

    std::vector<std::pair<int, std::string>> pnp_methods = {
        {cv::SOLVEPNP_ITERATIVE, "SOLVEPNP_ITERATIVE"},
        {cv::SOLVEPNP_EPNP, "SOLVEPNP_EPNP"},
        {cv::SOLVEPNP_P3P, "SOLVEPNP_P3P"},
        {cv::SOLVEPNP_DLS, "SOLVEPNP_DLS"},
        {cv::SOLVEPNP_UPNP, "SOLVEPNP_UPNP"}
    };

    cv::Mat rvec, tvec;

    for (auto& method : pnp_methods) {

        double start_time = static_cast<double>(cv::getTickCount());
        bool success = cv::solvePnP(object_points, image_points, camera_matrix, dist_coeffs, rvec, tvec, false, method.first);
        double end_time = static_cast<double>(cv::getTickCount());
        double time_elapsed = (end_time - start_time) / cv::getTickFrequency() * 1000;

        if (success) {
            std::cout << "PnP Method: " << method.second << std::endl;

            double rot_error = rotationError(rvec_ground_truth, rvec);
            double trans_error = translationError(tvec_ground_truth, tvec);

            // std::cout << "Rotation error: " << rot_error << " degrees" << std::endl;
            std::cout << "Rotation: " << rvec.t() << " degrees" << std::endl;
            // std::cout << "Translation error: " << rvec << " units" << std::endl;
            std::cout << "Translation: " << tvec.t() << " units" << std::endl;
            std::cout << "Time elapsed: " << time_elapsed << " ms" << std::endl << std::endl;
        } else {
            std::cout << "PnP Method: " << method.second << " failed." << std::endl << std::endl;
        }
    }

    return 0;
}
