/*
 * Program to demonstrate
 * CREATION and USE of CUSTOM ALLOCATORS
 * in C++
 */

#include<limits>
#include<iostream>
#include<vector>
#include<memory>
#include<conio.h>

template<class T>
class TrackingAllocator
{
public:
	using value_type = T;

	//The aliases below are optional
	//std::allocator_traits has a list of all the references which are used by defaults by the allocator
	using pointer = T* ;
	using const_pointer = const T*;

	using void_pointer = void*;
	using const_void_pointer = const void*;

	using size_type = size_t;

	using difference_type = std::ptrdiff_t;

	template<class U>
	struct rebind
	{
		using other = TrackingAllocator<U>;
	};

	TrackingAllocator() = default;

	template<class U>
	TrackingAllocator(const TrackingAllocator<U>& other)
	{

	}

	~TrackingAllocator() = default;

	//Allocates memory only (DOES NOT initialize objects)
	pointer allocate(size_type numObjects)
	{
		mAllocations += numObjects;
		std::cout << "Allocate " << numObjects << " element(s) of size " << sizeof(T) << std::endl;
		pointer ret = static_cast<pointer>(operator new(sizeof(T) * numObjects));
		std::cout << "allocated at: " << (void*)ret << std::endl;

		return ret;
	}

	void deallocate(pointer p, size_type num)
	{
		std::cout << "Deallocate " << num << " element(s) of size " << sizeof(T) << " at: " << (void*)p << std::endl;

		operator delete(p);
	}

	size_type max_size() const
	{
		return std::numeric_limits< size_type>::max();
	}

	template<class U, class... Args>
	void construct(U* p, Args&& ...args)
	{
		new(p) U(std::forward<Args>(args)...);
	}

	template<class U>
	void destroy(U *p)
	{
		p->~U();
	}

	size_type get_allocations() const
	{
		return mAllocations;
	}

private:
	static size_type mAllocations;
};

template<class T>
typename TrackingAllocator<T>::size_type TrackingAllocator<T>::mAllocations = 0;

int main()
{
	std::vector<double, TrackingAllocator<double>> v(5);
	std::cout << v.get_allocator().get_allocations() << std::endl;

	_getch();
	return 0;
}