# Assignment 3– Animation (OpenGL)

## Team Members
-  Roll No. 23B1073 - Marumamula Venkata Pranay
-  Roll No. 23B0970 - Priyanshu Kumar

---

## Declaration
We hereby declare that we have mostly written this assignment code ourselves, with a little help from ChatGPT.  
We have taken conceptual or technical help from the following sources:

### *Sources / References*
- OpenGL official documentation – [https://www.khronos.org/opengl/wiki](https://www.khronos.org/opengl/wiki)
- LearnOpenGL tutorial – [https://learnopengl.com](https://learnopengl.com)
- Some youtube videos

---

## Compilation and Execution

### *Dependencies*
- OpenGL (version ≥ 3.3)
- GLFW
- GLEW

### *Build Instructions*
make && make run

## Keymap

### Excavator Controls
| Key(s) | Action | Notes |
|:-------|:--------|:------|
| **W / S** | Move excavator forward / backward | Adjusts Z position |
| **A / D** | Move excavator left / right | Adjusts X position |
| **Q / E** | Rotate excavator cabin left / right | Changes `cabin->ry` |
| **U / J** | Raise / lower boom | Range: −10° to 50° |
| **I / K** | Raise / lower stick | Range: −10° to 120° |
| **O / L** | Curl / uncurl bucket | Range: −20° to 90° |
| **Y / H** | Twist stick clockwise / counterclockwise | Range: −45° to 45° |
| **T / G** | Roll boom clockwise / counterclockwise | Range: −30° to 30° |

### Lighting Controls
| Key | Action |
|:----|:--------|
| **1** | Toggle Light 1 |
| **2** | Toggle Light 2 |
| **3** | Toggle Excavator lights |

### Camera Controls
| Key(s) | Action | Notes |
|:-------|:--------|:------|
| **C** | Switch between Camera 1 and Camera 2 | Without modifiers |
| **Shift + C** | Save all camera keyframes to `camera.txt` | Writes to file |
| **X** | Save current camera parameters as keyframe | Pushes to vector |
| **V** | Toggle camera path visualization | Requires ≥ 4 keyframes |
| **↑ / ↓ / ← / →** | Move camera forward / backward / left / right | Moves both position and target |
| **, / .** | Move camera up / down | Vertical translation |
| **7 / 8** | Tilt camera up / down | Adjusts pitch |
| **9 / 0** | Pan camera left / right | Adjusts yaw |

### Scene Keyframes
| Key | Action |
|:----|:--------|
| **R** | Save current excavator + scene state as keyframe |
