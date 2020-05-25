#pragma once

#include "Lithium.h"
#include "readerwriterqueue/readerwriterqueue.h"

int TickThreadEntryPoint(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue);