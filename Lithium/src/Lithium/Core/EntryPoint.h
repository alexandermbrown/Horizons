#pragma once

#define SDL_MAIN_HANDLED
#include "Lithium/Core/Application.h"
#include "Lithium/Core/Core.h"

extern li::Application* li::CreateApplication();
int main(int argc, char** argv);