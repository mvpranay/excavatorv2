#ifndef KEYFRAME_HPP
#define KEYFRAME_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct cameraKeyPoint{
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    int num_frames; // number of frames to reach the next keypoint
};

struct sceneKeyFrame{
    int frame_num;
    // if camera moves 
    glm::vec3 cameraPos, cameraTarget;
    bool light1On, light2On, excavatorLightsOn;
    glm::vec3 excavatorPos;
    float cabinRotation;
    float boomAngle;
    float stickAngle;
    float bucketAngle;
    float boomRoll;
    float stickTwist;
};

class KeyFrameMangager {
public:
    std::vector<cameraKeyPoint> cameraKeyPoints;
    std::vector<sceneKeyFrame> sceneKeyFrames;

    bool saveKeyFramesToFile(const std::string &filename);
    bool loadKeyFramesFromFile(const std::string &filename);

    void clear(){
        cameraKeyPoints.clear();
        sceneKeyFrames.clear();
    }
};

#endif 