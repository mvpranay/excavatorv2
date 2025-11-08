// implement keyframe.hpp
#include "keyframe.hpp"
#include <fstream>
#include <iostream>

// for string manipulation
#include <sstream>

// for fixed precision
#include <iomanip>

using namespace std;

float Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

glm::vec3 lerpVec3(const glm::vec3 &a, const glm::vec3 &b, float t) {
    return glm::vec3(
        Lerp(a.x, b.x, t),
        Lerp(a.y, b.y, t),
        Lerp(a.z, b.z, t)
    );
}

bool KeyFrameMangager::saveSceneKeyFramesToFile(const string &filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return false;
    }

    // save scene key frames
    file << sceneKeyFrames.size() << "\n";
    for (const auto & skf : sceneKeyFrames) {
        file << fixed << setprecision(3)
            << skf.frame_num << " "
            << skf.cameraPos.x << " " << skf.cameraPos.y << " " << skf.cameraPos.z << " "
            << skf.cameraTarget.x << " " << skf.cameraTarget.y << " " << skf.cameraTarget.z << " "
            << skf.light1On << " " << skf.light2On << " "
            << skf.excavatorLightsOn << " "
            << skf.excavatorPos.x << " " << skf.excavatorPos.y << " " << skf.excavatorPos.z << " "
            << skf.cabinRotation << " " << skf.boomAngle << " " << skf.stickAngle << " " << skf.bucketAngle << " "
            << skf.boomRoll << " " << skf.stickTwist << "\n";
    }

    file.close();

    return true;
}

bool KeyFrameMangager::saveCameraKeyFramesToFile(const string &filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return false;
    }

    // save camera key points
    file << cameraKeyPoints.size() << "\n";
    for (const auto & ckp : cameraKeyPoints) {
        file << fixed << setprecision(3)
            << ckp.cameraPos.x << " " << ckp.cameraPos.y << " " << ckp.cameraPos.z << " "
            << ckp.cameraTarget.x << " " << ckp.cameraTarget.y << " " << ckp.cameraTarget.z << " "
            << ckp.num_frames << "\n";
    }

    file.close();

    return true;
}

bool KeyFrameMangager::loadSceneKeyFramesFromFile(const string &filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << filename << endl;
        return false;
    }

    // clear current key frames
    clear();

    // read scene key frames
    string line;
    int numSceneKeyFrames;
    getline(file, line);
    numSceneKeyFrames = stoi(line);

    for (int i = 0; i < numSceneKeyFrames; ++i) {
        getline(file, line);
        istringstream iss(line);
        sceneKeyFrame skf;
        iss >> skf.frame_num
            >> skf.cameraPos.x >> skf.cameraPos.y >> skf.cameraPos.z
            >> skf.cameraTarget.x >> skf.cameraTarget.y >> skf.cameraTarget.z
            >> skf.light1On >> skf.light2On >> skf.excavatorLightsOn
            >> skf.excavatorPos.x >> skf.excavatorPos.y >> skf.excavatorPos.z
            >> skf.cabinRotation >> skf.boomAngle >> skf.stickAngle >> skf.bucketAngle
            >> skf.boomRoll >> skf.stickTwist;
        sceneKeyFrames.push_back(skf);
    }

    file.close();

    return true;
}

bool KeyFrameMangager::loadCameraKeyFramesFromFile(const string &filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << filename << endl;
        return false;
    }

    // clear current key points
    clear();

    // read camera key points
    string line;
    int numCameraKeyPoints;
    getline(file, line);
    numCameraKeyPoints = stoi(line);

    for (int i = 0; i < numCameraKeyPoints; ++i) {
        getline(file, line);
        istringstream iss(line);
        cameraKeyPoint ckp;
        iss >> ckp.cameraPos.x >> ckp.cameraPos.y >> ckp.cameraPos.z
            >> ckp.cameraTarget.x >> ckp.cameraTarget.y >> ckp.cameraTarget.z
            >> ckp.num_frames;
        cameraKeyPoints.push_back(ckp);
    }

    file.close();

    return true;
}

sceneKeyFrame interpolateSceneKeyFrames(const sceneKeyFrame & f1, const sceneKeyFrame & f2, float t) {
    sceneKeyFrame result;

    result.frame_num = int(Lerp(float(f1.frame_num), float(f2.frame_num), t));
    result.light1On = (t < 0.5f) ? f1.light1On : f2.light1On;
    result.light2On = (t < 0.5f) ? f1.light2On : f2.light2On;
    result.excavatorLightsOn = (t < 0.5f) ? f1.excavatorLightsOn : f2.excavatorLightsOn;
    result.excavatorPos = lerpVec3(f1.excavatorPos, f2.excavatorPos, t);
    result.cabinRotation = Lerp(f1.cabinRotation, f2.cabinRotation, t);
    result.boomAngle = Lerp(f1.boomAngle, f2.boomAngle, t);
    result.stickAngle = Lerp(f1.stickAngle, f2.stickAngle, t);
    result.bucketAngle = Lerp(f1.bucketAngle, f2.bucketAngle, t);
    result.boomRoll = Lerp(f1.boomRoll, f2.boomRoll, t);
    result.stickTwist = Lerp(f1.stickTwist, f2.stickTwist, t);

    return result;
}

cameraKeyPoint interpolateCameraKeyPoints(const cameraKeyPoint & c1, const cameraKeyPoint & c2, float t) {
    cameraKeyPoint result;

    result.cameraPos = lerpVec3(c1.cameraPos, c2.cameraPos, t);
    result.cameraTarget = lerpVec3(c1.cameraTarget, c2.cameraTarget, t);
    result.num_frames = int(Lerp(float(c1.num_frames), float(c2.num_frames), t));

    return result;
}

// use De Casteljau's algorithm to compute interpolation of many frames
// use De Casteljau's algorithm to compute a point on the Bezier curve for sceneKeyFrames
sceneKeyFrame getBezierKeyFrame(vector<sceneKeyFrame> keyframes, float t) {
    int n = keyframes.size();
    
    // Create a working copy of keyframes
    vector<sceneKeyFrame> points = keyframes;

    for (int r = 1; r < n; ++r) {
        for (int i = 0; i < n - r; ++i) {
            // Interpolate numeric values
            points[i].cameraPos = (1.0f - t) * points[i].cameraPos + t * points[i + 1].cameraPos;
            points[i].cameraTarget = (1.0f - t) * points[i].cameraTarget + t * points[i + 1].cameraTarget;
            points[i].excavatorPos = (1.0f - t) * points[i].excavatorPos + t * points[i + 1].excavatorPos;
            points[i].cabinRotation = (1.0f - t) * points[i].cabinRotation + t * points[i + 1].cabinRotation;
            points[i].boomAngle = (1.0f - t) * points[i].boomAngle + t * points[i + 1].boomAngle;
            points[i].stickAngle = (1.0f - t) * points[i].stickAngle + t * points[i + 1].stickAngle;
            points[i].bucketAngle = (1.0f - t) * points[i].bucketAngle + t * points[i + 1].bucketAngle;
            points[i].boomRoll = (1.0f - t) * points[i].boomRoll + t * points[i + 1].boomRoll;
            points[i].stickTwist = (1.0f - t) * points[i].stickTwist + t * points[i + 1].stickTwist;
            
            // For boolean values, use the value from the first keyframe if t < 0.5, otherwise use second
            points[i].light1On = (t < 0.5f) ? points[i].light1On : points[i + 1].light1On;
            points[i].light2On = (t < 0.5f) ? points[i].light2On : points[i + 1].light2On;
            points[i].excavatorLightsOn = (t < 0.5f) ? points[i].excavatorLightsOn : points[i + 1].excavatorLightsOn;
            
            // Frame number doesn't need interpolation during De Casteljau
            points[i].frame_num = points[i].frame_num;
        }
    }

    return points[0];
}

// use De Casteljau's algorithm to compute a point on the Bezier curve
glm::vec3 getBezierPoint(vector<glm::vec3> points, float t) {
    int n = points.size();

    for (int r = 1; r < n; ++r) {
        for (int i = 0; i < n - r; ++i) {
            points[i] = (1.0f - t) * points[i] + t * points[i + 1];
        }
    }

    return points[0];
}

// derivative of Bezier curve
glm::vec3 getBezierDerivative(vector<glm::vec3> points, float t) {
    int n = points.size() - 1;
    vector<glm::vec3> derivativePoints;
    for (int i = 0; i < n; ++i) {
        derivativePoints.push_back(float(n) * (points[i + 1] - points[i]));
    }

    return getBezierPoint(derivativePoints, t);
}
        