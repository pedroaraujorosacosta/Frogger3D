#include "Camera.h"
#include "Stack.h"
#include "Game.h"
#include "Frog.h"

#include <gl\glew.h>
#include <GL/freeglut.h>

#include <math.h>

Camera::Camera(Game *game, float top, float  bot, float n, float f, float left, float right, float FOV, float S) 
{
	this->t = top;
	this->b = bot;
	this->n = n;
	this->f = f;
	this->l = left;
	this->r = right;
	this->FOV = FOV;
	this->S = S;
	this->game = game;
	this->theta = 0;
	this->phi = 0;
	mode = TOP;
}

void Camera::init()
{

}

void Camera::topCameraMode() 
{
	mode = TOP;
}

void Camera::topCameraPerspectiveMode()
{
	mode = PERSPECTIVE;
}

void Camera::FPSCameraMode()
{
	mode = FPS;
}

void Camera::setCamera() {
	Frog *frog = game->getFrog();

	// actualiza o aspect ratio
	S = tan(FOV*0.5*(M_PI / 180)) * n;
	r = aspectRatio * S, l = -r;
	t = S, b = -t;

	/*GLfloat eye[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat right[4] = { 1.0f, 0.0f, 0.0f, 0.0f }; //u 
	GLfloat up[4] = { 0.0f, 1.0f, 0.0f, 0.0f }; //n
	GLfloat lookPoint[4] = { 0.0f, 0.0f, -1.0f, 1.0f };*/

	switch (mode)
	{
	case Camera::TOP:

		setEye(0.0f, 0.0f, 1.0f);
		setAt(0.0f, 0.0f, -1.0f);
		setUp(0.0f, 1.0f, 0.0f);

		game->getProjectionStack()->orthogonal(-10, 10, -7, 7, 0.1, 10);
		break;
	case Camera::PERSPECTIVE:

		setEye(0.0f, 0.0f, 10.0f);
		setAt(0.0f, 0.0f, -1.0f);
		setUp(0.0f, 1.0f, 0.0f);

		game->getProjectionStack()->perspective(l, r, b, t, n, f);
		break;
	case Camera::FPS:

		setEye(frog->getX(), frog->getY() - 2, frog->getZ() + 1);
		setAt(frog->getX() + cos(theta + M_PI / 2) * cos(phi), 
			  frog->getY() + -2 + sin(theta + M_PI / 2) * cos(phi),
			  frog->getZ() + 1 + sin(phi));
		setUp(0.0f, 0.0f, 1.0f);

		game->getProjectionStack()->perspective(l, r, b, t, n, f); //TODO
		break;
	default:
		break;
	}

	game->getModelViewStack()->lookAt(eye[0], eye[1], eye[2],
		at[0], at[1], at[2],
		up[0], up[1], up[2]);
}

void Camera::reshape(int w, int h)
{
	aspectRatio = (double)w / h;
	glViewport(0, 0, w, h);
}

void Camera::setEye(float eyex, float eyey, float eyez)
{
	eye[0] = eyex;
	eye[1] = eyey;
	eye[2] = eyez;
	eye[3] = 1.0;
}

void Camera::setAt(float atx, float aty, float atz)
{
	at[0] = atx;
	at[1] = aty;
	at[2] = atz;
	at[3] = 1.0;
}

void Camera::setUp(float upx, float upy, float upz)
{
	up[0] = upx;
	up[1] = upy;
	up[2] = upz;
	up[3] = 0.0;
}

void Camera::updateDirection(int dx, int dy)
{
	if (mode == FPS)
	{
		theta -= dx / 10;
		if (theta > M_PI / 2 || theta < M_PI / 2)
			theta += dx / 10;
		phi -= dy / 10;
		if (phi > M_PI / 2 || phi < M_PI / 2)
			phi += dy / 10;
	}
}