#pragma once

#include "GizmoBase.h"

class AGizmoTranslate : public AGizmoBase
{
	DECLARE_CLASS(AGizmoTranslate, AGizmoBase)
public:
	AGizmoTranslate();

	void Init();

	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

	void OnDragTick(FVector2 PosNDC, FVector2 DeltaNDC, const FMatrix& ViewProj, FVector Axis, AActor* Target) override;

	virtual bool Intersects(FRay ray, FVector& Axis, float& hitDistance) override;

private:
	FVector LocalMin = FVector(0.000016f, -0.118037f, -0.117011f);	//하드코딩 min, max
	FVector LocalMax = FVector(1.920310f, 0.118037f, 0.117011f);
};