#include "pch.h"
#include "GizmoTranslate.h"
#include "Asset/ObjManager.h"
#include "CoreUObject/Components/StaticMeshComponent.h"

AGizmoTranslate::AGizmoTranslate(): AGizmoBase() {}

void AGizmoTranslate::Init()
{
	// Rendering용으로 사용할 StaticMeshComponent 생성.
	UStaticMeshComponent* GizmoTranslateMeshX = AddComponent<UStaticMeshComponent>(this);
	GizmoTranslateMeshX->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
	GizmoTranslateMeshX->SetRelativeRotation(FRotator(0, 0, 0));
	GizmoTranslateMeshX->SetupAttachment(RootComponent);

	UStaticMeshComponent* GizmoTranslateMeshY = AddComponent<UStaticMeshComponent>(this);
	GizmoTranslateMeshY->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
	GizmoTranslateMeshY->SetRelativeRotation(FRotator(0, 90, 0));
	GizmoTranslateMeshY->SetupAttachment(RootComponent);

	UStaticMeshComponent* GizmoTranslateMeshZ = AddComponent<UStaticMeshComponent>(this);
	GizmoTranslateMeshZ->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
	GizmoTranslateMeshZ->SetRelativeRotation(FRotator(90, 0, 0));
	GizmoTranslateMeshZ->SetupAttachment(RootComponent);


	//X축 기준
	OriginalAABB[0] = FVector(LocalMin.X, LocalMin.Y, LocalMin.Z);
	OriginalAABB[1] = FVector(LocalMax.X, LocalMin.Y, LocalMin.Z);
	OriginalAABB[2] = FVector(LocalMin.X, LocalMax.Y, LocalMin.Z);
	OriginalAABB[3] = FVector(LocalMax.X, LocalMax.Y, LocalMin.Z);
	OriginalAABB[4] = FVector(LocalMin.X, LocalMin.Y, LocalMax.Z);
	OriginalAABB[5] = FVector(LocalMax.X, LocalMin.Y, LocalMax.Z);
	OriginalAABB[6] = FVector(LocalMin.X, LocalMax.Y, LocalMax.Z);
	OriginalAABB[7] = FVector(LocalMax.X, LocalMax.Y, LocalMax.Z);

}

void AGizmoTranslate::Tick(float TickTime) {
	AGizmoBase::Tick(TickTime);
}

void AGizmoTranslate::Destroy() {
	AGizmoBase::Destroy();
}

void AGizmoTranslate::OnDragTick(FVector2 PosNDC, FVector2 DeltaNDC, const FMatrix& ViewProj, FVector Axis, AActor* Target)
{
	static const float Sensitivity = 1.f;
	FVector AxisOnScreen = ViewProj.TransformDirectionVector(Axis);
	AxisOnScreen.Z = 0;
	float DispAmt = -FVector(DeltaNDC.X, DeltaNDC.Y, 0.f).Dot(AxisOnScreen.GetSafeNormal());
	FVector Disp = Axis * DispAmt * Sensitivity;
	
	if (isnan(Disp.X) || isnan(Disp.Y) || isnan(Disp.Z))
		return;

	FVector Loc = Target->GetActorLocation();
	Loc += Disp;
	Target->SetActorLocation(Loc);
}

bool AGizmoTranslate::Intersects(FRay ray, FVector& Axis, float& hitDistance) {
	FMatrix World = RootComponent->GetWorldMatrix();
	FMatrix Inv = (FMatrix(FRotator(0, 0, 0)) * World).Inverse();

	FVector origin, direction;
	origin = Inv.TransformPositionVector(ray.Origin);
	direction = Inv.TransformDirectionVector(ray.Direction);

	FRay RayLocal = { origin, direction };

	if (Geometry::IsRayIntersectAABB(FBoundingBox(LocalMin, LocalMax), RayLocal, hitDistance))
	{
		Axis = (FMatrix(FRotator(0, 0, 0)) * World).TransformDirectionVector(FVector(1, 0, 0));
		return true;
	}

	Inv = (FMatrix(FRotator(0, 90, 0)) * World).Inverse();

	origin = Inv.TransformPositionVector(ray.Origin);
	direction = Inv.TransformDirectionVector(ray.Direction);

	RayLocal = { origin, direction };

	if (Geometry::IsRayIntersectAABB(FBoundingBox(LocalMin, LocalMax), RayLocal, hitDistance))
	{
		Axis = (FMatrix(FRotator(0, 90, 0)) * World).TransformDirectionVector(FVector(1, 0, 0));
		return true;
	}

	Inv = (FMatrix(FRotator(90, 0, 0)) * World).Inverse();

	origin = Inv.TransformPositionVector(ray.Origin);
	direction = Inv.TransformDirectionVector(ray.Direction);

	RayLocal = { origin, direction };

	if (Geometry::IsRayIntersectAABB(FBoundingBox(LocalMin, LocalMax), RayLocal, hitDistance))
	{
		Axis = (FMatrix(FRotator(90, 0, 0)) * World).TransformDirectionVector(FVector(1, 0, 0));
		return true;
	}
	return false;
}
