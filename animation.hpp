#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "keyframe.hpp"
#include <cstring>

// // Forward declare - include stb_image_write in implementation
extern "C" {
    int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
}

class AnimationSystem {
private:
    KeyFrameMangager keyframeManager;
    std::vector<sceneKeyFrame> interpolatedFrames;
    
    int currentFrame = 0;
    bool isPlaying = false;
    bool isRecording = false;
    int frameCounter = 0;
    
    float fps;
    float frameTime; // in seconds
    double lastFrameTime = 0.0;
    
    int nextKeyframeIndex = 51;
    int defaultFrameIncrement= 5;
    
public:
    AnimationSystem(float framesPerSecond = 30.0f) 
        : fps(framesPerSecond)
    {
        frameTime = 1.0f / fps;
    }
    
    // Save current state as a keyframe
    bool saveKeyframe(const sceneKeyFrame& frame) {
        sceneKeyFrame newFrame = frame;
        newFrame.frame_num = nextKeyframeIndex;
        
        keyframeManager.sceneKeyFrames.push_back(newFrame);
        nextKeyframeIndex += defaultFrameIncrement;
        
        std::cout << "Keyframe saved at frame " << newFrame.frame_num << std::endl;
        return true;
    }
    
    // Save keyframes to file
    bool saveToFile(const std::string& filename) {
        return keyframeManager.saveSceneKeyFramesToFile(filename);
    }
    
    // Load keyframes from file
    bool loadFromFile(const std::string& filename) {
        bool success = keyframeManager.loadSceneKeyFramesFromFile(filename);
        cout << "Initially have " << keyframeManager.sceneKeyFrames.size() << " frames" << endl;
        if (success) {
            // Generate interpolated frames
            generateInterpolatedFrames();
        }
        return success;
    }
    
    // Generate interpolated frames from keyframes
    void generateInterpolatedFrames() {
        interpolatedFrames.clear();
        
        if (keyframeManager.sceneKeyFrames.size() < 2) {
            std::cerr << "Need at least 2 keyframes to interpolate" << std::endl;
            return;
        }
    
        for (float t = 0.0; t <= 1.0; t += 0.01){
            sceneKeyFrame skf = getBezierKeyFrame(keyframeManager.sceneKeyFrames, t);
            interpolatedFrames.push_back(skf);
        }
        
        std::cout << "Generated " << interpolatedFrames.size() << " interpolated frames" << std::endl;
    }
    
    // Start playback
    void startPlayback() {
        if (interpolatedFrames.empty()) {
            std::cerr << "No frames to play. Load keyframes first." << std::endl;
            return;
        }
        
        currentFrame = 0;
        isPlaying = true;
        isRecording = false;
        lastFrameTime = glfwGetTime();
        std::cout << "Starting playback..." << std::endl;
    }
    
    // Start recording
    void startRecording() {
        if (interpolatedFrames.empty()) {
            std::cerr << "No frames to record. Load keyframes first." << std::endl;
            return;
        }
        
        currentFrame = 0;
        isPlaying = true;
        isRecording = true;
        frameCounter = 0;
        lastFrameTime = glfwGetTime();
        std::cout << "Starting recording..." << std::endl;
    }
    
    // Update animation (call this every frame)
    bool update(double currentTime) {
        if (!isPlaying || interpolatedFrames.empty()) {
            return false;
        }

        cout << "Currently at frame " << currentFrame << endl;
        
        // Check if enough time has passed for next frame
        if (currentTime - lastFrameTime >= frameTime) {
            lastFrameTime = currentTime;
            currentFrame++;
            
            if (currentFrame >= (int)interpolatedFrames.size()) {
                std::cout << "Animation finished" << std::endl;
                isPlaying = false;
                isRecording = false;
                return false;
            }
            
            return true; // New frame available
        }
        
        return true;
    }
    
    // Get current frame data
    const sceneKeyFrame* getCurrentFrame() const {
        if (currentFrame < (int)interpolatedFrames.size()) {
            return &interpolatedFrames[currentFrame];
        }
        return nullptr;
    }
    
    // Capture frame to image file
    void captureFrame(int width, int height) {
        if (!isRecording) return;
        
        // Allocate buffer for pixels
        std::vector<unsigned char> pixels(width * height * 3);
        
        // Read pixels from framebuffer
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
        
        // Flip image vertically (OpenGL has origin at bottom-left)
        std::vector<unsigned char> flipped(width * height * 3);
        for (int y = 0; y < height; ++y) {
            memcpy(&flipped[y * width * 3], 
                   &pixels[(height - 1 - y) * width * 3], 
                   width * 3);
        }
        
        // Generate filename
        char filename[256];
        sprintf(filename, "frame-%05d.png", frameCounter);
        
        // Save to PNG
        if (stbi_write_png(filename, width, height, 3, flipped.data(), width * 3)) {
            std::cout << "Saved " << filename << std::endl;
        } else {
            std::cerr << "Failed to save " << filename << std::endl;
        }
        
        frameCounter++;
    }
    
    bool getIsPlaying() const { return isPlaying; }
    bool getIsRecording() const { return isRecording; }
    int getCurrentFrameNumber() const { return currentFrame; }
    float getFPS() const { return fps; }
    
    void clear() {
        keyframeManager.clear();
        interpolatedFrames.clear();
        currentFrame = 0;
        isPlaying = false;
        isRecording = false;
        nextKeyframeIndex = 51;
    }
};

#endif // ANIMATION_HPP