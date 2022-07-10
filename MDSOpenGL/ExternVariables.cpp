#include "ExternVariables.h"

//Window
unsigned int e_uViewPortW = 800, e_uViewPortH = 800;

//Time
float e_fPreviousTimestep = 0.0f;
float e_fDeltatime = 0.0f;

void UpdateDeltatime()
{
	float fCurrentTimestep = (float)glfwGetTime();
	e_fDeltatime = fCurrentTimestep - e_fPreviousTimestep;
	e_fPreviousTimestep = fCurrentTimestep;
}

//Input
int e_iKey = 0;
int e_iKeyScanCode = 0;
int e_iKeyAction = 0;
int e_iKeyMods = 0;
bool e_bKeyPressed = false;
char e_charCodePoint = 0;
bool e_bCodePointFound = false;

glm::vec2 e_v2MousePosition;
int e_iMouseButton = 0;
int e_iMouseAction = 0;
int e_iMouseMods = 0;
bool e_bMousePressed = false;

bool e_bTextInputEnabled = false;

void KeyFunction(GLFWwindow* _pWindow, int _iKey, int _iScanCode, int _iAction, int _iMods)
{
	e_iKey = _iKey;
	e_iKeyScanCode = _iScanCode;
	e_iKeyAction = _iAction;
	e_iKeyMods = _iMods;
}

void UpdateMousePosition(GLFWwindow* _pWindow)
{
	double XPos, YPos;
	glfwGetCursorPos(_pWindow, &XPos, &YPos);

	e_v2MousePosition.x = (float)XPos;
	e_v2MousePosition.y = (float)YPos;
}

void MouseButtonFunction(GLFWwindow* _pWindow, int _iButton, int _iAction, int _iMods)
{
	e_iMouseButton = _iButton;
	e_iMouseAction = _iAction;
	e_iMouseMods = _iMods;
}

void UpdateInputPressed()
{
	if (e_iKeyAction == GLFW_RELEASE) e_bKeyPressed = false;
	else e_bKeyPressed = true;

	e_bCodePointFound = false;

	if (e_iMouseAction == GLFW_RELEASE) e_bMousePressed = false;
	else e_bMousePressed = true;
}

void TextInput(GLFWwindow* _pWindow, unsigned int _iCodePoint)
{
	e_bCodePointFound = true;
	e_charCodePoint = _iCodePoint;
}

//Physics
unsigned int e_uiSubsteps = 1U;
float e_fGravity = 9.18f;