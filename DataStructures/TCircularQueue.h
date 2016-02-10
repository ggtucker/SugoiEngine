#pragma once

#include <array>

namespace sr {
	template<typename TObject, size_t MAX_SIZE = 16>
	class TCircularQueue {
	public:
		TCircularQueue() {}

		TCircularQueue(const TCircularQueue& other) :
			queue{ other.queue },
			queueHead{ other.queueHead },
			queueTail{ other.queueTail } {}

		TCircularQueue(TCircularQueue&& other) :
			queue{ std::move(other.queue) },
			queueHead{ std::move(other.queueHead) },
			queueTail{ std::move(other.queueTail) } {}

		TCircularQueue& operator=(const TCircularQueue& other) {
			if (this != &other) {
				queue = other.queue;
				queueHead = other.queueHead;
				queueTail = other.queueTail;
			}
			return *this;
		}

		TCircularQueue& operator=(TCircularQueue&& other) {
			if (this != &other) {
				queue = std::move(other.queue);
				queueHead = std::move(other.queueHead);
				queueTail = std::move(other.queueTail);
			}
			return *this;
		}

		void Push(TObject obj) {
			if ((queueTail + 1) % MAX_SIZE != queueHead) {
				queue[queueTail] = obj;
				queueTail = (queueTail + 1) % MAX_SIZE;
			}
		}

		bool Pop(TObject& obj) {
			if (queueHead != queueTail) {
				obj = queue[queueHead];
				queueHead = (queueHead + 1) % MAX_SIZE;
				return true;
			}
			return false;
		}

		bool Peek(TObject& obj) {
			if (queueHead != queueTail) {
				obj = queue[queueHead];
				return true;
			}
			return false;
		}

	private:
		std::array<TObject, MAX_SIZE> queue;
		unsigned int queueHead;
		unsigned int queueTail;
	};
}