#pragma once

#include "Object.h"
#include "Resource/Mesh/StaticMeshAsset.h"

class UStaticMesh : public UObject
{
	DECLARE_CLASS(UStaticMesh, UObject)

public:
	UStaticMesh();

	virtual void Destroy() override;

public:
	const FString& GetAssetPathFileName();
	void SetStaticMeshAsset(FStaticMesh* InStaticMeshAsset);

private:
	FStaticMesh* StaticMeshAsset;

};
