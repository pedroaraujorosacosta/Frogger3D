#ifndef __CAMERA_H__
#define __CAMERA_H__

#define M_PI atan(1.0f)*4

class Game;

class Camera {
	
protected:

	enum{TOP, PERSPECTIVE, FPS} mode;

	Game *game;

	float eye[4];
	float up[4];
	float at[4];

	float aspectRatio;
	float t;
	float b;
	float n;
	float f;
	float l;
	float r;

	float FOV;
	float S;
	
	virtual void init();
public:
	Camera(Game *game, float top, float  bot, float n, float f, float left, float right, float FOV, float S);
	void topCameraMode();
	void topCameraPerspectiveMode();
	void FPSCameraMode();
	void setCamera();
	void reshape(int w, int h);
	void setEye(float eyex, float eyey, float eyez); 
	void setAt(float atx, float aty, float atz);
	void setUp(float upx, float upy, float upz);

};
#endif