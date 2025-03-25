#pragma once

#include "Object.h"

class FSceneManager;
class FObjManager;

/*
	프로그램에서 사용할 모든 에셋을 관리할 매니저.


*/
class UAssetManager : public UObject
{
	DECLARE_CLASS(UAssetManager, UObject)



public:
	UAssetManager();
	virtual void Destroy() override;

public:
	UAssetManager& GetInstance()
	{
		static UAssetManager Instance;
		return Instance;
	}

	FSceneManager* GetSceneManager() const { return SceneManager; }

private:
	FSceneManager* SceneManager;
	FObjManager* ObjManager;
};