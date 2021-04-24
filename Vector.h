//Vector, very simillar to the one (std::vector), written by Karen
//Very hard and research demanding this was.

#ifndef VECTOR_INCLUDED
#define VECTOR_INCLUDED


#include <functional>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <utility>


template <class T>
class vector {


	char* memory_all;
	int _size;
	int _capacity;
	T* _ptr;
	void alloc(int new_size, const T& new_elems) {
		for (int i = _size; i < new_size; ++i)
			new(memory_all + i * sizeof(T)) T(new_elems);
	}
	void change_capacity(int new_capacity) {
		char* memory_all_new = new T[new_capacity];
		for (int i = 0; i < _size; ++i)
			new(memory_all_new + i * sizeof(T)) T(_ptr[i]);
		delete[] memory_all;
		memory_all = memory_all_new;
		_ptr = reinterpret_cast<T*>(memory_all);
	}


public:


	using iterator = T*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using value_type = T;



	vector() :
		memory_all(new char[0]),
		_size(0),
		_capacity(0),
		_ptr(reinterpret_cast<T*>(memory_all))
	{};
	explicit vector(int size) :
		memory_all(new char[size * sizeof(T)]),
		_size(size),
		_capacity(size),
		_ptr(reinterpret_cast<T*>(memory_all))
	{};
	vector(int size, const T& val) :
		memory_all(new char[size * sizeof(T)]),
		_size(size),
		_capacity(size),
		_ptr(reinterpret_cast<T*>(memory_all)) {
		for (int i = 0; i < _size; ++i)
			new(memory_all + i * sizeof(T)) T(val);
	};
	template<class Iter>
	vector(Iter begin, Iter end) :
		memory_all(new char[(end - begin - 1) * sizeof(T)]),
		_size(end - begin - 1),
		_capacity(_size),
		_ptr(reinterpret_cast<T*>(memory_all)) {
		int i = 0;
		for (auto it = begin; it != end; ++it, ++i)
			new(memory_all + i * sizeof(T)) T(*it);
	};
	vector(const vector& v) :							//copy constructor
		memory_all(new char[(v.size()) * sizeof(T)]),
		_size(v.size()),
		_capacity(_size),
		_ptr(reinterpret_cast<T*>(memory_all)) {
		for (int i = 0; i < _size; ++i)
			new(memory_all + i * sizeof(T)) T(v[i]);
	};
	vector(const std::initializer_list<T>& lst) :
		memory_all(new char[lst.size() * sizeof(T)]),
		_size(lst.size()),
		_capacity(_size),
		_ptr(reinterpret_cast<T*>(memory_all)) {
		auto temp = lst.begin();
		for (int i = 0; i < _size; ++i, ++temp)
			new(memory_all + i * sizeof(T)) T(*temp);
	};
	vector& operator=(const vector& v) {
			delete[] memory_all;
			memory_all = new char[(v.size()) * sizeof(T)];
			_size = v.size();
			_capacity = _size;
			_ptr(reinterpret_cast<T*>(memory_all));
			for (int i = 0; i < _size; ++i)
				new(memory_all + i * sizeof(T)) T(v[i]);
		}			//assighnment operator
	~vector(){
		delete[] memory_all;
	}



	iterator begin() {
		return _ptr;
	}
	const_iterator begin() const {
		return _ptr;
	}
	iterator end() {
		return _ptr + _size;
	}
	const_iterator end() const {
		return _ptr + _size;
	}
	reverse_iterator rbegin() {
		return std::reverse_iterator<iterator>(this->begin());
	}
	const_reverse_iterator rbegin() const {
		return std::reverse_iterator<const_iterator>(this->begin());
	}
	reverse_iterator rend() {
		return std::reverse_iterator<iterator>(this->end());
	}
	const_reverse_iterator rend() const {
		return std::reverse_iterator<const_iterator>(this->end());
	}
	const_iterator cbegin() const {
		return _ptr;
	}
	const_reverse_iterator crbegin() const {
		return std::reverse_iterator<const_iterator>(this->cbegin());
	}



	int size() const {
		return _size;
	}
	void resize(int new_size, const T& new_elems = T()){
		if (new_size < _capacity) {
			if (new_size <= _size)
				std::for_each(_ptr + new_size, _ptr + _size, std::bind(&T::~T, std::placeholders::_1));
			else
				alloc(new_size, new_elems);
		}
		else {
			this->reserve(new_size * 2);
			alloc(new_size, new_elems);
		}
		_size = new_size;
	}
	int capacity() const {
		return _capacity;
	}
	void reserve(int new_capacity) {
		if (new_capacity > _capacity)
			change_capacity(new_capacity);
	}
	bool empty() const {
		return _size == 0;
	}
	void shrink_to_fit() {
		change_capacity(_size);
	}



	T& operator[](int i) {
		return _ptr[i];
	}
	const T& operator[](int i) const {
		return _ptr[i];
	}
	T& at(int i) {
		assert(i < _size);
		return _ptr[i];
	}
	const T& at(int i) const {
		assert(i < _size);
		return _ptr[i];
	}
	T& front() {
		return this->at(0);
	}
	const T& front() const {
		return this->at(0);
	}
	T& back() {
		return this->at(_size - 1);
	}
	const T& back() const {
		return this->at(_size - 1);
	}
	T* data() {
		return _size == 0 ? nullptr : _ptr;
	}
	const T* data() const {
		return _size == 0 ? nullptr : _ptr;
	}



	void push_back(const T& val) {
		if (_size == capacity)
			reserve(_capacity * 2);
		_ptr[_size++] = val;
	}
	void pop_back() {
		_ptr[_size--].~T();
	}
	void clear() {
		_size = 0;
		reserve(_capacity);
	}
	iterator erase(const_iterator erase_pos) {
		for (auto it = erase_pos; erase_pos != this->end() - 1; ++it)
			std::swap(*it, *(it + 1));
		this->pop_back();
		return erase_pos;
	}
	iterator erase(const_iterator first, const_iterator last) {
		vector<T> temp;
		for (auto it = this->begin(); it != this->end(); ++it) {
			if (it == first)
				it = last;
			temp.push_back(*it);
		}
		return *this = temp;
	}	
	iterator insert(const_iterator insert_pos, const T& val) {
		this->push_back(val);
		for (auto it = this->rbegin(); it != insert_pos; ++it)
			std::swap(*it, *(it + 1));
	}
	iterator insert(const_iterator insert_pos, const std::initializer_list<T>& lst) {
		_size += lst.size();
		if (_size > _capacity)
			reserve(_size * 2);
		int i = 0;
		for (auto it = insert_pos; it != insert_pos + lst.size(); ++it, ++i) {
			auto temp = it + lst.size();
			*temp = *it;
			*it = lst[i];
		}
	}


};

template<class T>
bool operator<(const vector<T>& v1, const vector<T> v2) {
	auto comp = std::mismatch(v1.begin(), v1.end(), v2.begin());
	return *(comp.first) < *(comp.second);
}
template<class T>
bool operator>(const vector<T>& v1, const vector<T> v2){
	return v2 < v1;
}
template<class T>
bool operator>=(const vector<T>& v1, const vector<T>& v2) {
	return !(v1 < v2);
}
template<class T>
bool operator<=(const vector<T>& v1, const vector<T>& v2) {
	return !(v1 > v2);
}
template<class T>
bool operator==(const vector<T>& v1, const vector<T>& v2) {
	return !(v1 < v2) && !(v1 > v2);
}
template<class T>
bool operator!=(const vector<T>& v1, const vector<T>& v2) {
	return !(v1 == v2);
}


#endif // !VECTOR_INCLUDED

