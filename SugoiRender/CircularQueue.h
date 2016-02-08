#pragma once

#include <array>

namespace sr {
	template<typename TObject, size_t MAX_SIZE = 16>
	class CircularQueue {
	public:
		CircularQueue() {}

		CircularQueue(const CircularQueue& other) :
			eventQueue{ other.eventQueue },
			queueHead{ other.queueHead },
			queueTail{ other.queueTail } {}

		CircularQueue(CircularQueue&& other) :
			eventQueue{ std::move(other.eventQueue) },
			queueHead{ std::move(other.queueHead) },
			queueTail{ std::move(other.queueTail) } {}

		CircularQueue& operator=(const CircularQueue& other) {
			if (this != &other) {
				eventQueue = other.eventQueue;
				queueHead = other.queueHead;
				queueTail = other.queueTail;
			}
			return *this;
		}

		CircularQueue& operator=(CircularQueue&& other) {
			if (this != &other) {
				eventQueue = std::move(other.eventQueue);
				queueHead = std::move(other.queueHead);
				queueTail = std::move(other.queueTail);
			}
			return *this;
		}

		void Push(TObject obj) {
			if ((queueTail + 1) % MAX_SIZE != queueHead) {
				eventQueue[queueTail] = obj;
				queueTail = (queueTail + 1) % MAX_SIZE;
			}
		}

		bool Pop(TObject& obj) {
			if (queueHead != queueTail) {
				obj = eventQueue[queueHead];
				queueHead = (queueHead + 1) % MAX_SIZE;
				return true;
			}
			return false;
		}

		bool Peek(TObject& obj) {
			if (queueHead != queueTail) {
				obj = eventQueue[queueHead];
				return true;
			}
			return false;
		}

	private:
		std::array<TObject, MAX_SIZE> eventQueue;
		unsigned int queueHead;
		unsigned int queueTail;
	};
}