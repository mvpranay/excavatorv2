#ifndef KEYFRAME_HPP
#define KEYFRAME_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace std;

struct cameraKeyPoint{
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    int num_frames; // number of frames to reach the next keypoint
};

struct sceneKeyFrame{
    int frame_num;
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    bool light1On, light2On, excavatorLightsOn;
    glm::vec3 excavatorPos;
    float cabinRotation;
    float boomAngle;
    float stickAngle;
    float bucketAngle;
    float boomRoll;
    float stickTwist;
};

sceneKeyFrame interpolateSceneKeyFrames(const sceneKeyFrame & f1, const sceneKeyFrame & f2, float t);
cameraKeyPoint interpolateCameraKeyPoints(const cameraKeyPoint & c1, const cameraKeyPoint & c2, float t);

glm::vec3 lerpVec3(const glm::vec3 &a, const glm::vec3 &b, float t);

// bezier interpolation
glm::vec3 getBezierPoint(vector<glm::vec3> points, float t);
glm::vec3 getBezierDerivative(vector<glm::vec3> points, float t);

sceneKeyFrame getBezierKeyFrame(vector<sceneKeyFrame> keyframes, float t);

class KeyFrameMangager {
public:
    vector<cameraKeyPoint> cameraKeyPoints;
    vector<sceneKeyFrame> sceneKeyFrames;

    bool saveCameraKeyFramesToFile(const std::string &filename);
    bool saveSceneKeyFramesToFile(const std::string &filename);

    bool loadCameraKeyFramesFromFile(const std::string &filename);
    bool loadSceneKeyFramesFromFile(const std::string &filename);

    vector<glm::vec3> getCameraPositions(){
        vector<glm::vec3> positions;
        for (const auto & ckp : cameraKeyPoints) {
            positions.push_back(ckp.cameraPos);
        }
        return positions;
    }

    void clear(){
        cameraKeyPoints.clear();
        sceneKeyFrames.clear();
    }
};

#endif 