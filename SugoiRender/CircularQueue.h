#pragma once

#include <array>
#include <GL/glew.h>
#include "Event.h"

namespace sr {
template<typename T, size_t MAX_PENDING = 16>
class CircularQueue {
public:
	CircularQueue() {}

	void Push(T obj) {
		if ((queueTail + 1) % MAX_PENDING != queueHead) {
			eventQueue[queueTail] = obj;
			queueTail = (queueTail + 1) % MAX_PENDING;
		}
	}

	GLboolean Pop(T& obj) {
		if (queueHead != queueTail) {
			obj = eventQueue[queueHead];
			queueHead = (queueHead + 1) % MAX_PENDING;
			return true;
		}
		return false;
	}

	GLboolean Peek(T& obj) {
		if (queueHead != queueTail) {
			obj = eventQueue[queueHead];
			return true;
		}
		return false;
	}

private:
	std::array<Event, MAX_PENDING> eventQueue;
	GLuint queueHead;
	GLuint queueTail;
};
}