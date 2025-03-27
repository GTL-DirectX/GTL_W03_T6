#pragma once

#include "GizmoBase.h"

class AGizmoRotate: public AGizmoBase
{
	DECLARE_CLASS(AGizmoRotate, AGizmoBase)
public:
	AGizmoRotate();

	void Init();

	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

	void OnClick(int mx, int my) override;
	void OnDragTick(FVector2 PosNDC, FVector2 DeltaNDC, const FMatrix& ViewProj, FVector Axis, AActor* Target) override;
	void OnRelease(int mx, int my) override;

	virtual bool Intersects(FRay ray, FVector& Axis, float& hitDistance) override;
};