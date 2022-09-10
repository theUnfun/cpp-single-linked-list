#pragma once

template <typename Type>
class SingleLinkedList {
	struct Node {
		Node() = default;
		Node(const Type& val, Node* next)
			: value(val)
			, next_node(next) {
		}
		Type value;
		Node* next_node = nullptr;
	};

	template <typename ValueType>
	class BasicIterator {
		friend class SingleLinkedList;

		explicit BasicIterator(Node* node) : node_(node) {
		}

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = Type;
		using difference_type = std::ptrdiff_t;
		using pointer = ValueType*;
		using reference = ValueType&;

		BasicIterator() = default;

		BasicIterator(const BasicIterator<Type>& other) noexcept {
			this->node_ = other.node_;
		}

		BasicIterator& operator=(const BasicIterator& rhs) = default;

		[[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
			return this->node_ == rhs.node_;
		}

		[[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
			return this->node_ != rhs.node_;
		}

		[[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
			return this->node_ == rhs.node_;
		}

		[[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
			return this->node_ != rhs.node_;
		}

		BasicIterator& operator++() noexcept {
			if (node_ != nullptr) {
				node_ = node_->next_node;
			}
			return *this;
		}

		BasicIterator operator++(int) noexcept {
			auto old_value(*this);
			++(*this);
			return old_value;
		}

		[[nodiscard]] reference operator*() const noexcept {
			return node_->value;
		}

		[[nodiscard]] pointer operator->() const noexcept {
			return &(node_->value);
		}

	private:
		Node* node_ = nullptr;
	};

public:
	using value_type = Type;
	using reference = value_type&;
	using const_reference = const value_type&;
	using Iterator = BasicIterator<Type>;
	using ConstIterator = BasicIterator<const Type>;

	[[nodiscard]] Iterator begin() noexcept {
		return Iterator(head_.next_node);
	}

	[[nodiscard]] Iterator end() noexcept {
		return Iterator(nullptr);
	}

	[[nodiscard]] ConstIterator begin() const noexcept {
		return ConstIterator(head_.next_node);
	}

	[[nodiscard]] ConstIterator end() const noexcept {
		return ConstIterator(nullptr);
	}

	[[nodiscard]] ConstIterator cbegin() const noexcept {
		return ConstIterator(head_.next_node);
	}

	[[nodiscard]] ConstIterator cend() const noexcept {
		return ConstIterator(nullptr);
	}

	SingleLinkedList() = default;

	SingleLinkedList(std::initializer_list<Type> values) {
		Initialization(values.begin(), values.end());
	}

	SingleLinkedList(const SingleLinkedList& other) {
		SingleLinkedList temp;
		temp.Initialization(other.begin(), other.end());
		swap(temp);
	}

	SingleLinkedList& operator=(const SingleLinkedList& rhs) {
		if (this != &rhs) {
			auto temp(rhs);
			swap(temp);
		}
		return *this;
	}

	void swap(SingleLinkedList& other) noexcept {
		std::swap(head_.next_node, other.head_.next_node);
		std::swap(size_, other.size_);
	}

	~SingleLinkedList() {
		Clear();
	};

	[[nodiscard]] size_t GetSize() const noexcept {
		return size_;
	}

	[[nodiscard]] bool IsEmpty() const noexcept {
		if (size_ == 0) {
			return true;
		}
		return false;
	}

	void PushFront(const Type& value) {
		head_.next_node = new Node(value, head_.next_node);
		++size_;
	}

	void Clear() noexcept {
		while (head_.next_node != nullptr) {
			auto temp = head_.next_node;
			head_.next_node = head_.next_node->next_node;
			delete temp;
		}
		size_ = 0;
	}

	[[nodiscard]] Iterator before_begin() noexcept {
		return Iterator(&head_);
	}

	[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
		return ConstIterator(const_cast<Node*>(&head_));
	}

	[[nodiscard]] ConstIterator before_begin() const noexcept {
		return ConstIterator(const_cast<Node*>(&head_));
	}

	Iterator InsertAfter(ConstIterator pos, const Type& value) {
		auto& prev_node = pos.node_;
		prev_node->next_node = new Node(value, prev_node->next_node);
		++size_;
		return Iterator(prev_node->next_node);
	}

	void PopFront() noexcept {
		if (IsEmpty()) {
			return;
		}
		Node* temp = head_.next_node->next_node;
		delete head_.next_node;
		head_.next_node = temp;
		--size_;
	}

	Iterator EraseAfter(ConstIterator pos) noexcept {
		if (IsEmpty()) {
			return Iterator(nullptr);
		}
		Node* const node_to_erase = pos.node_->next_node;
		--size_;
		Node* const node_after_erased = node_to_erase->next_node;
		delete node_to_erase;
		pos.node_->next_node = node_after_erased;
		return Iterator(node_after_erased);
	}

private:
	template <typename It>
	void Initialization(It begin, It end) {
		Node* node = &head_;
		for (auto it = begin; it != end; it++) {
			node->next_node = new Node(*it, nullptr);
			node = node->next_node;
			size_++;
		}
	}

	Node head_ = {};
	size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
	lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return std::equal(lhs.begin(), lhs.end(), rhs.begin()) && lhs.GetSize() == rhs.GetSize();
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), lhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !(lhs < rhs);
}