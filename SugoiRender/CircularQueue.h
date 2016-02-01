#pragma once

#include <array>

namespace sr {
	template<typename TObject, size_t MAX_SIZE = 16>
	class CircularQueue {
	public:
		CircularQueue() {}

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