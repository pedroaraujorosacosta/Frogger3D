#include <iostream>
#include <string>
#include <assert.h>

#include "Object.h"
#include "Matrix.h"
#include "Game.h"

#include <gl\glew.h>
#include <GL\freeglut.h>
#include "vsShaderLib.h"

Object::Object(float *position, Game *game) {
	for (int i = 0; i < 3; i++)
		this->position[i] = position[i];
	this->game = game;
}

Object::~Object() {

	glDisableVertexAttribArray(MY_VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(MY_NORMAL_ATTRIB);
	glDisableVertexAttribArray(MY_TEXTURE_COORD_ATTRIB);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(4, VboId);
	glDeleteVertexArrays(1, &VaoId);
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");

}

void Object::draw(GLuint programID) {
	
}

void Object::sendMaterials()
{
	GLint loc;
	VSShaderLib *shader = game->getShader();
	
	//glUseProgram(shader->getProgramIndex());
	loc = glGetUniformLocation(shader->getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mat.ambient);
	loc = glGetUniformLocation(shader->getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mat.diffuse);
	loc = glGetUniformLocation(shader->getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mat.specular);
	loc = glGetUniformLocation(shader->getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mat.shininess);
	//glUseProgram(0);
}

void Object::sendDataToShader(GLuint programID)
{
	/*Matrix top;
	top = game->getPVM();

	GLfloat PVM[16];

	for (int i = 0; i < top.sizeofMatrix(); i++)
		PVM[i] = top.m[i];*/
	
	glBindVertexArray(VaoId);
	//glUseProgram(programID);
	GLuint pvmID = game->getPVMid();
	GLuint vmID = game->getVMid();
	GLuint iVmID = game->getIVMid();

	// send the VM matrix
	Matrix vm = game->getVM();
	float vmFloat[16];
	for (int i = 0; i < 16; i++)
		vmFloat[i] = vm.m[i];
	VSShaderLib *shader = game->getShader();
	//shader->setUniform("m_viewModel", vmFloat);
	glUniformMatrix4fv(vmID, 1, GL_TRUE, vmFloat);

	checkOpenGLError("ERROR: Could not draw scene.");

	// compute and send the normal matrix
	Matrix iVm = vm.invertMatrix();
	float iVmFloat[9];
	for (int i = 0; i < 9; i++)
		iVmFloat[i] = iVm.m[i];
	glUniformMatrix3fv(iVmID, 1, GL_TRUE, iVmFloat);
	//shader->setUniform("m_normal", iVmFloat);


	// send the PVM matrix
	Matrix pvmM = game->getPVM();
	float pvmFloat[16];
	for (int i = 0; i < 16; i++)
		pvmFloat[i] = pvmM.m[i];
	glUniformMatrix4fv(pvmID, 1, GL_TRUE, pvmFloat);
	//shader->setUniform("m_pvm", pvmFloat);


	//glUniformMatrix4fv(matID, 1, GL_TRUE, PVM);
	glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, (GLvoid*)0);

	//glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

void Object::init()
{
	
	
	
}

void Object::reset() {


}

void Object::update() {


}

void Object::createBufferObjects() 
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(4, VboId);

	//vertex coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	// in this case, the buffer is allocated and nothing is transferred to it until bufferSubData is called
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verticeCount*4, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*verticeCount*4, vertices);
	glEnableVertexAttribArray(MY_VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(MY_VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);

	//normals buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verticeCount*3, normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(MY_NORMAL_ATTRIB);
	glVertexAttribPointer(MY_NORMAL_ATTRIB, 3, GL_FLOAT, 0, 0, 0);

	//texture coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verticeCount*2, texCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(MY_TEXTURE_COORD_ATTRIB);
	glVertexAttribPointer(MY_TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);

	//index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*faceCount * 3, faceIndex, GL_STATIC_DRAW);

	// unbind the VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(MY_VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(MY_NORMAL_ATTRIB);
	glDisableVertexAttribArray(MY_TEXTURE_COORD_ATTRIB);

	this->checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

float * Object::circularProfile(float minAngle, float maxAngle, float radius, int divisions, float transX, float transY) {

	float *p = (float *)malloc(sizeof(float) * 2 * (divisions + 3));
	float step = (maxAngle - minAngle) / divisions;

	for (int i = 0, k = -1; i < divisions + 3; ++i, ++k) {

		p[i * 2] = radius * cos(minAngle + k * step) + transX;
		p[i * 2 + 1] = radius * sin(minAngle + k * step) + transY;
		//		printf("%f %f\n", p[i*2], p[i * 2 + 1]);
	}


	return p;
}

int Object::revSmoothNormal2(float *p, float *nx, float *ny, float smoothCos, int beginEnd) {

	float v1x, v1y, v2x, v2y, x, y, norm;
	float auxX, auxY, angle;

	auxX = p[0] - p[2];
	auxY = p[1] - p[3];
	v1x = -auxY;
	v1y = auxX;
	norm = sqrt((v1x*v1x) + (v1y*v1y));
	v1x /= norm;
	v1y /= norm;

	auxX = p[2] - p[4];
	auxY = p[3] - p[5];
	v2x = -auxY;
	v2y = auxX;
	norm = sqrt((v2x*v2x) + (v2y*v2y));
	v2x /= norm;
	v2y /= norm;

	angle = v1x * v2x + v1y * v2y;

	if (angle > smoothCos) {
		x = v1x + v2x;
		y = v1y + v2y;
	}
	else if (beginEnd == 0) {
		x = v2x;
		y = v2y;
	}
	else  {
		x = v1x;
		y = v1y;

	}

	norm = sqrt(x*x + y*y);

	assert(norm != 0);
	x /= norm;
	y /= norm;

	*nx = x;
	*ny = y;
	if (angle > smoothCos)
		return 1;
	else
		return 0;
}


void
Object::computeVAO(int numP, float *p, float *points, int sides, float smoothCos) {
	// Compute and store vertices

	int numSides = sides;
	int numPoints = numP + 2;

	float *vertex = (float *)malloc(sizeof(float)*numP * 2 * 4 * (numSides + 1));
	float *normal = (float *)malloc(sizeof(float)*numP * 2 * 4 * (numSides + 1));
	float *textco = (float *)malloc(sizeof(float)*numP * 2 * 4 * (numSides + 1));


	float inc = 2 * 3.14159f / (numSides);
	float nx, ny;
	float delta;
	int smooth;
	std::vector<int> smoothness;
	int k = 0;
	for (int i = 0; i < numP; i++) {
		revSmoothNormal2(points + (i * 2), &nx, &ny, smoothCos, 0);
		for (int j = 0; j <= numSides; j++) {

			if ((i == 0 && p[0] == 0.0f) || (i == numP - 1 && p[(i + 1) * 2] == 0.0))
				delta = inc * 0.5f;
			else
				delta = 0.0f;

			normal[((k)*(numSides + 1) + j) * 4] = nx * cos(j*inc + delta);
			normal[((k)*(numSides + 1) + j) * 4 + 1] = ny;
			normal[((k)*(numSides + 1) + j) * 4 + 2] = nx * sin(-j*inc + delta);
			normal[((k)*(numSides + 1) + j) * 4 + 3] = 0.0f;

			vertex[((k)*(numSides + 1) + j) * 4] = p[i * 2] * cos(j*inc);
			vertex[((k)*(numSides + 1) + j) * 4 + 1] = p[(i * 2) + 1];
			vertex[((k)*(numSides + 1) + j) * 4 + 2] = p[i * 2] * sin(-j*inc);
			vertex[((k)*(numSides + 1) + j) * 4 + 3] = 1.0f;

			textco[((k)*(numSides + 1) + j) * 4] = ((j + 0.0f) / numSides);
			textco[((k)*(numSides + 1) + j) * 4 + 1] = (i + 0.0f) / (numP - 1);
			textco[((k)*(numSides + 1) + j) * 4 + 2] = 0;
			textco[((k)*(numSides + 1) + j) * 4 + 3] = 1.0f;
		}
		k++;
		if (i < numP - 1) {
			smooth = revSmoothNormal2(points + ((i + 1) * 2), &nx, &ny, smoothCos, 1);

			if (!smooth) {
				smoothness.push_back(1);
				for (int j = 0; j <= numSides; j++) {

					normal[((k)*(numSides + 1) + j) * 4] = nx * cos(j*inc);
					normal[((k)*(numSides + 1) + j) * 4 + 1] = ny;
					normal[((k)*(numSides + 1) + j) * 4 + 2] = nx * sin(-j*inc);
					normal[((k)*(numSides + 1) + j) * 4 + 3] = 0.0f;

					vertex[((k)*(numSides + 1) + j) * 4] = p[(i + 1) * 2] * cos(j*inc);
					vertex[((k)*(numSides + 1) + j) * 4 + 1] = p[((i + 1) * 2) + 1];
					vertex[((k)*(numSides + 1) + j) * 4 + 2] = p[(i + 1) * 2] * sin(-j*inc);
					vertex[((k)*(numSides + 1) + j) * 4 + 3] = 1.0f;

					textco[((k)*(numSides + 1) + j) * 4] = ((j + 0.0f) / numSides);
					textco[((k)*(numSides + 1) + j) * 4 + 1] = (i + 1 + 0.0f) / (numP - 1);
					textco[((k)*(numSides + 1) + j) * 4 + 2] = 0;
					textco[((k)*(numSides + 1) + j) * 4 + 3] = 1.0f;
				}
				k++;
			}
			else
				smoothness.push_back(0);
		}
	}

	unsigned int *faceIndex = (unsigned int *)malloc(sizeof(unsigned int) * (numP - 1) * (numSides + 1) * 6);
	unsigned int count = 0;
	k = 0;
	for (int i = 0; i < numP - 1; ++i) {
		for (int j = 0; j < numSides; ++j) {

			/*if (i != 0 || p[0] != 0.0)*/ {
				faceIndex[count++] = k * (numSides + 1) + j;
				faceIndex[count++] = (k + 1) * (numSides + 1) + j + 1;
				faceIndex[count++] = (k + 1) * (numSides + 1) + j;
				}
			/*if (i != numP-2 || p[(numP-1)*2] != 0.0)*/ {
				faceIndex[count++] = k * (numSides + 1) + j;
				faceIndex[count++] = k * (numSides + 1) + j + 1;
				faceIndex[count++] = (k + 1) * (numSides + 1) + j + 1;
			}

		}
		k++;
		k += smoothness[i];
	}

	verticeCount = numP * 2 * (numSides + 1);
	faceCount = count;

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(4, VboId);

	//vertex coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticeCount * 4, vertex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(MY_VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(MY_VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);

	//texture coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticeCount * 4, textco, GL_STATIC_DRAW);
	glEnableVertexAttribArray(MY_TEXTURE_COORD_ATTRIB);
	glVertexAttribPointer(MY_TEXTURE_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);

	//normals buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticeCount * 4, normal, GL_STATIC_DRAW);
	glEnableVertexAttribArray(MY_NORMAL_ATTRIB);
	glVertexAttribPointer(MY_NORMAL_ATTRIB, 4, GL_FLOAT, 0, 0, 0);

	//index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faceCount, faceIndex, GL_STATIC_DRAW);

	// unbind the VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(MY_VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(MY_NORMAL_ATTRIB);
	glDisableVertexAttribArray(MY_TEXTURE_COORD_ATTRIB);

}

void Object::setAmbient(float *amb)
{
	memcpy(mat.ambient, amb, 4 * sizeof(float));
}

void Object::setDiffuse(float *diff)
{
	memcpy(mat.diffuse, diff, 4 * sizeof(float));
}

void Object::setSpecular(float *spec)
{
	memcpy(mat.specular, spec, 4 * sizeof(float));
}

void Object::setEmissive(float *emissive)
{
	memcpy(mat.emissive, emissive, 4 * sizeof(float));
}

void Object::setShininess(float shininess)
{
	mat.shininess = shininess;
}

void Object::setTexCount(int texCount)
{
	mat.texCount = texCount;
}