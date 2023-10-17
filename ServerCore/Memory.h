#pragma once

#include "Allocator.h"

template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(Allocator::Alloc(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...); // 생성자 호출 (placement new)
	return memory;
}

template<typename Type>
void xdelete(Type* ptr)
{
	ptr->~Type(); // 소멸자 호출
	Allocator::Release(ptr);
}