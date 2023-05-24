#include "Mouse.h";
#include <iostream>

double Mouse::x = 0;
double Mouse::y = 0;

double Mouse::lastX = 0;
double Mouse::lastY = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

double Mouse::scrollDX = 0;
double Mouse::scrollDY = 0;

bool Mouse::firstMouse = true;
bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = {0};
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = {0};

void Mouse::cursorPosCallback(GLFWwindow *window, double _x, double _y)
{
    x = _x;
    y = _y;

    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    dx = x - lastX;
    dy = lastY - y;
    lastX = x;
    lastY = y;

    // glfwSetCursorPos(window, 0, 0);
}
void Mouse::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (action != GLFW_RELEASE)
    {
        if (!buttons[button])
        {
            buttons[button] = true;
        }
    }
    else
    {
        buttons[button] = false;
    }
    buttonsChanged[button] = action != GLFW_REPEAT;
}

void Mouse::mouseWheelCallback(GLFWwindow *window, double dx, double dy)
{
    scrollDX = dx;
    scrollDY = dy;
}

double Mouse::getMouseX()
{
    return x;
}
double Mouse::getMouseY()
{
    return y;
}

double Mouse::getDX()
{
    double ret = dx;
    dx = 0;
    return ret;
}
double Mouse::getDY()
{
    double ret = dy;
    dy = 0;
    return ret;
}

double Mouse::getScrollDX()
{

    double ret = scrollDX;
    scrollDX = 0;
    return ret;
}
double Mouse::getScrollDY()
{

    double ret = scrollDY;
    scrollDY = 0;
    return ret;
}

bool Mouse::button(int button)
{
    return buttons[button];
}
bool Mouse::buttonChanged(int button)
{
    bool ret = buttonsChanged[button];
    buttonsChanged[button] = false;
    return ret;
}
bool Mouse::buttonWentUp(int button)
{
    return !buttons[button] && buttonChanged(button);
}
bool Mouse::buttonWentDown(int button)
{

    return buttons[button] && buttonChanged(button);
}