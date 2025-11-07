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

bool KeyFrameMangager::saveKeyFramesToFile(const string &filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return false;
    }

    // save camera key points
    file << "CameraKeyPoints\n";
    file << cameraKeyPoints.size() << "\n";
    for (const auto & ckp : cameraKeyPoints) {
        file << fixed << setprecision(3)
             << ckp.cameraPos.x << " " << ckp.cameraPos.y << " " << ckp.cameraPos.z << " "
             << ckp.cameraTarget.x << " " << ckp.cameraTarget.y << " " << ckp.cameraTarget.z << " "
             << ckp.num_frames << "\n";
    }

    // save scene key frames
    file << "SceneKeyFrames\n";
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

bool KeyFrameMangager::loadKeyFramesFromFile(const string &filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << filename << endl;
        return false;
    }

    // clear current key frames
    clear();

    // read camera key points
    string line;
    getline(file, line);
    if (line != "CameraKeyPoints") {
        cerr << "Invalid file format: expected 'CameraKeyPoints'" << endl;
        return false;
    }

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

    // read scene key frames
    getline(file, line);
    if (line != "SceneKeyFrames") {
        cerr << "Invalid file format: expected 'SceneKeyFrames'" << endl;
        return false;
    }

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

sceneKeyFrame interpolateSceneKeyFrames(const sceneKeyFrame & f1, const sceneKeyFrame & f2, float t) {
    sceneKeyFrame result;

    result.frame_num = int(Lerp(float(f1.frame_num), float(f2.frame_num), t));
    result.cameraPos = lerpVec3(f1.cameraPos, f2.cameraPos, t);
    result.cameraTarget = lerpVec3(f1.cameraTarget, f2.cameraTarget, t);
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