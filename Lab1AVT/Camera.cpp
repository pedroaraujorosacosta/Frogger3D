#include "Camera.h"
#include "Stack.h"
#include "Game.h"
#include "Frog.h"

#include <gl\glew.h>
#include <GL/freeglut.h>

#include <math.h>

#define degToRad(x) ((x/180) * M_PI)

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

	// direccao da camara ditada pelo sist. coord. do sapo e angulos de visao theta e phi.
	float frogBasis1[3] = { frog->getDirX(), frog->getDirY(), frog->getDirZ() };
	float frogBasis2[3] = { frog->getDirY(), -frog->getDirX(), frog->getDirZ() };
	float frogBasis3[3] = { 0.0f, 0.0f, 1.0f };

	float xx = -cos(degToRad(phi)) * sin(degToRad(theta));
	float yy = cos(degToRad(phi)) * cos(degToRad(theta));
	float zz = sin(degToRad(phi));
	float dirX = xx * frogBasis2[0] + yy * frogBasis1[0] + zz * frogBasis3[0];
	float dirY = xx * frogBasis2[1] + yy * frogBasis1[1] + zz * frogBasis3[1];
	float dirZ = xx * frogBasis2[2] + yy * frogBasis1[2] + zz * frogBasis3[2];
	
	float eyePosX = frog->getPositionXXs() - 2*xx;/*frog->getX() - 2 * frog->getDirX();*/
	float eyePosY = frog->getPositionYYs() - 2*yy;/*frog->getY() - frog->getDirY();*/
	float eyePosZ = frog->getPositionZZs() - 2*zz;/*frog->getZ()+1 - 2 * frog->getDirZ();*/
	float atX = frog->getPositionXXs(); /*eyePosX + dirX;*/
	float atY = frog->getPositionYYs(); /*eyePosY + dirY;*/
	float atZ = frog->getPositionZZs(); /*eyePosZ + dirZ;*/

	switch (mode)
	{
	case Camera::TOP:

		setFOV(90);
		setEye(0.0f, 0.0f, 1.0f);
		setAt(0.0f, 0.0f, -1.0f);
		setUp(0.0f, 1.0f, 0.0f);

		game->getProjectionStack()->orthogonal(-20, 20, -14, 14, 0.1, 10);
		break;
	case Camera::PERSPECTIVE:

		setFOV(60);
		setEye(0.0f, 0.0f, 24.0f);
		setAt(0.0f, 0.0f, -1.0f);
		setUp(0.0f, 1.0f, 0.0f);

		game->getProjectionStack()->perspective(l, r, b, t, n, f);
		break;
	case Camera::FPS:

		setFOV(90);
		setEye(eyePosX, eyePosY, eyePosZ);
		setAt(atX, atY, atZ);
		setUp(0.0f, 0.0f, 1.0f);

		game->getProjectionStack()->perspective(l, r, b, t, n, f);
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

void Camera::setFOV(float fov)
{
	FOV = fov;
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

bool Camera::isInFPSMode()
{
	return mode == FPS;
}

void Camera::updateDirection(int dx, int dy)
{
	if (mode == FPS)
	{
		theta -= dx / 10;
		theta = fmod(theta, 360);

		phi += dy / 10;
		if (phi > 90 || phi < -90)
			phi -= dy / 10;
	}
}

bool Camera::isFacingLeft()
{
	float left[] = { -1.0f, 0.0f, 0.0f }; 
	return true;
}

bool Camera::isFacingRight()
{
	float right[] = { 1.0f, 0.0f, 0.0f };
	return true;
}

bool Camera::isFacingFront()
{
	float front[] = { 0.0f, 1.0f, 0.0f };
	return true;
}

bool Camera::isFacingBack()
{
	float back[] = { 0.0f, -1.0f, 0.0f };
	return true;
}

Vector Camera::getEye()
{
	return Vector(eye, 3);
}