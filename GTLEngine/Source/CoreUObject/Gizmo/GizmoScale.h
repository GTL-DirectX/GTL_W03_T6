#pragma once

#include "GizmoBase.h"

class AGizmoScale : public AGizmoBase
{
	DECLARE_CLASS(AGizmoScale, AGizmoBase)
public:
	AGizmoScale();

	void Init();

	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

	void OnClick(int mx, int my) override;
	void OnDragTick(FVector2 PosNDC, FVector2 DeltaNDC, const FMatrix& ViewProj, FVector Axis, AActor* Target) override;
	void OnRelease(int mx, int my) override;

	virtual bool Intersects(FRay ray, FVector& Axis, float& hitDistance) override;

private:
	FVector LocalMin = FVector(0.000016f, -0.118037f, -0.117011f);	//하드코딩 min, max
	FVector LocalMax = FVector(1.920310f, 0.118037f, 0.117011f);
};