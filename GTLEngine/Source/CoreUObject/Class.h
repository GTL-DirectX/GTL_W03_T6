﻿#pragma once

#include <concepts>
#include "Object.h"
#include "HAL/PlatformMemory.h"

/**
 * UObject의 RTTI를 가지고 있는 클래스
 */
class UClass : public UObject
{
public:
	UClass(const wchar_t* InClassName, uint32 InClassSize, uint32 InAlignment, UClass* InSuperClass);
	virtual ~UClass() = default;

	// 복사 & 이동 생성자 제거
	UClass(const UClass&) = delete;
	UClass& operator=(const UClass&) = delete;
	UClass(UClass&&) = delete;
	UClass& operator=(UClass&&) = delete;


	/** SomeBase의 자식 클래스인지 확인합니다. */
	bool IsChildOf(const UClass* SomeBase) const;

	template <typename T>
		requires std::derived_from<T, UObject>
	bool IsChildOf() const
	{
		return IsChildOf(T::StaticClass());
	}

	/**
	 * 부모의 UClass를 가져옵니다.
	 *
	 * @note AActor::StaticClass()->GetSuperClass() == UObject::StaticClass()
	 */
	UClass* GetSuperClass() const
	{
		return SuperClass;
	}

	UObject* GetDefaultObject() const
	{
		if (!ClassDefaultObject)
		{
			const_cast<UClass*>(this)->CreateDefaultObject();
		}
		return ClassDefaultObject;
	}

protected:
	virtual UObject* CreateDefaultObject();

private:
	[[maybe_unused]]
	uint32 ClassSize;

	[[maybe_unused]]
	uint32 ClassAlignment;

	UClass* SuperClass = nullptr;

	UObject* ClassDefaultObject = nullptr;
};


struct UClassDeleter
{
	void operator()(UClass* ClassPtr) const
	{
		ClassPtr->~UClass();
		FPlatformMemory::Free(ClassPtr, sizeof(UClass));
	}
};
