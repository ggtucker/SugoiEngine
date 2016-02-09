#pragma once

#include <mutex>
#include <array>
#include <cassert>
#include <utility>

template<typename TObject, size_t PoolSize>
class TMemoryPool {
public:
	TMemoryPool() {

		init_pool_();
	}

	TMemoryPool(const TMemoryPool& other) :
		pool_{ other.pool_ } {

		init_pool_();
	}

	TMemoryPool(TMemoryPool&& other) :
		pool_{ std::move(other.pool_) },
		first_avail_{ other.first_avail_ } {

		other.first_avail_ = nullptr;
	}

	virtual ~TMemoryPool() {}

	TMemoryPool& operator=(const TMemoryPool& other) {
		if (this != &other) {
			std::lock(this->mutex_, other.mutex_);
			std::lock_guard<std::mutex> lhs_lock(this->mutex_, std::adopt_lock);
			std::lock_guard<std::mutex> rhs_lock(other.mutex_, std::adopt_lock);
			this->pool_ = other.pool_;
			init_pool_();
		}
		return *this;
	}

	TMemoryPool& operator=(TMemoryPool&& other) {
		if (this != &other) {
			pool_ = std::move(other.pool_);
			first_avail_ = other.first_avail_;
			other.first_avail_ = nullptr;
		}
		return *this;
	}

	TObject& operator[](unsigned int i) {
		return pool_[i].obj;
	}

	int create() {
		return create<>();
	}

	template<class... Args>
	int create(Args&&... args) {
		std::lock_guard<std::mutex> lock(mutex_);
		if (first_avail_) {
			int index = first_avail_ - pool_.data();
			TObject* obj = reinterpret_cast<TObject*>(first_avail_);
			first_avail_ = first_avail_->next;
			new(obj) TObject(std::forward<Args>(args)...);
			return index;
		}
		return -1;
	}

	void deallocate(int i) {
		std::lock_guard<std::mutex> lock(mutex_);
		if (i >= 0 && i < (int) pool_.size()) {
			TObject* obj = &pool_[i].obj;
			obj->~TObject();
			PoolNode* node = reinterpret_cast<PoolNode*>(obj);
			node->next = first_avail_;
			first_avail_ = node;
		}
	}

private:
	union PoolNode {
		TObject obj;
		PoolNode* next;
		PoolNode() {}
		~PoolNode() {}
	};
	std::array<PoolNode, PoolSize> pool_;
	PoolNode* first_avail_;
	std::mutex mutex_;

	void init_pool_() {
		first_avail_ = &pool_[0];
		for (size_t i = 0; i < PoolSize - 1; ++i) {
			pool_[i].next = &pool_[i + 1];
		}
		pool_[PoolSize - 1].next = nullptr;
	}
};