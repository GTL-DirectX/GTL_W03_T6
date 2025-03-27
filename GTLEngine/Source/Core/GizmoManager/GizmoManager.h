#pragma once

#include "Object.h"

#include "Gizmo/GizmoBase.h"

struct FRay;
class AGizmoTranslate;
class AGizmoRotate;
class AGizmoScale;



class FGizmoManager
{

public:
	FGizmoManager();

	void Init();

	void Tick(float DeltaTime);
	void Destroy();
	AGizmoBase* GetGizmoActor() { return CurrentGizmo; }
	void AttachTo(AActor* Target) { SelectedActor = Target; }
	AActor* CastRayAndPick(FRay Ray);
	void OnDrag(FVector2 PosNDC, FVector2 DeltaNDC, const FMatrix& ViewProj);


private:
	EGizmoModeIndex GizmoMode = EGizmoModeIndex::GMI_GizmoTranslate;

private:
	AActor* SelectedActor = nullptr;
	AGizmoBase* CurrentGizmo = nullptr;
	bool IsGizmoDragged = false;
	FVector AxisOnDrag = FVector(0,0,0);
	AGizmoTranslate* GizmoTranslate = nullptr;
	AGizmoRotate* GizmoRotate = nullptr;
	AGizmoScale* GizmoScale = nullptr;

public:
	AActor* PickActor(FRay ray) const;
	void ClearSelected();
	const TArray<AGizmoBase*> GetGizmo();
	inline AActor* GetSelected() const { return SelectedActor; }
	inline void SetSelected(AActor* actor) { SelectedActor = actor; }
};

