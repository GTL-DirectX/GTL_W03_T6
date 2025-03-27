#include "pch.h"
#include "GizmoRotate.h"
#include "Asset/ObjManager.h"
#include "CoreUObject/Components/StaticMeshComponent.h"


AGizmoRotate::AGizmoRotate() : AGizmoBase() {
}

void AGizmoRotate::Init() {
	// Rendering용으로 사용할 StaticMeshComponent 생성.
	UStaticMeshComponent* GizmoRotateMeshX = AddComponent<UStaticMeshComponent>(this);
	GizmoRotateMeshX->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoRotate.obj")));
	GizmoRotateMeshX->SetRelativeRotation(FRotator(0, 0, 0));
	GizmoRotateMeshX->SetupAttachment(RootComponent);

	UStaticMeshComponent* GizmoRotateMeshY = AddComponent<UStaticMeshComponent>(this);
	GizmoRotateMeshY->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoRotate.obj")));
	GizmoRotateMeshY->SetRelativeRotation(FRotator(0, -90, 0));
	GizmoRotateMeshY->SetupAttachment(RootComponent);

	UStaticMeshComponent* GizmoRotateMeshZ = AddComponent<UStaticMeshComponent>(this);
	GizmoRotateMeshZ->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoRotate.obj")));
	GizmoRotateMeshZ->SetRelativeRotation(FRotator(-90, 0, 0));
	GizmoRotateMeshZ->SetupAttachment(RootComponent);
}

void AGizmoRotate::Tick(float TickTime)
{
	AGizmoBase::Tick(TickTime);
}

void AGizmoRotate::Destroy()
{
	AGizmoBase::Destroy();
}

void AGizmoRotate::OnClick(int mx, int my)
{
	startMouseX = mx;
	startMouseY = my;
}

void AGizmoRotate::OnDragTick(FVector2 PosNDC, FVector2 DeltaNDC, const FMatrix& ViewProj, FVector Axis, AActor* Target)
{
	// Rotation Gizmo의 방향으로 움직이게 수정
	FRotator ActorRotation = Target->GetActorRotation();
	const float sensitive = 1.f;

	// screen 상에서의 회전.
	FVector4 TargetPosNDC4((ViewProj).TransformVector4(FVector4(Target->GetActorLocation(), 1.f)));
	FVector TargetPosNDC = TargetPosNDC4 / TargetPosNDC4.W;
	TargetPosNDC.Z = 0;
	FVector Displacement = FVector(PosNDC.X, PosNDC.Y, 0) - TargetPosNDC;
	FVector RotationDirection = Displacement.Cross(FVector(DeltaNDC.X, DeltaNDC.Y, 0)); // z > 0 for clockwise, z < 0 for counter-clockwise.

	// direction of axis : toward or backward the screen.
	FVector AxisNDC = (ViewProj).TransformVector4(FVector4(Axis, 0.f)).xyz();
	RotationDirection.Z *= AxisNDC.Z;

	// slow rotation when the cursor is far from the gizmo.
	RotationDirection.Z /= Displacement.LengthSquared();

	// quaternions.
	Axis.Normalize();
	FVector v = Axis * sin(RotationDirection.Z * sensitive);
	FQuat RotationQuat(v.X, v.Y, v.Z, cos(RotationDirection.Z * sensitive));
	//RotationQuat.Normalize(); // already normalized.
	FQuat PreviousQuat = Target->GetActorRotation().Quaternion();
	//PreviousQuat.Normalize(); // already normalized.
	FQuat NewQuat = FQuat::MultiplyQuaternions(RotationQuat, PreviousQuat);
	//NewQuat.Normalize(); // already normalized.
	FRotator NewRotator(NewQuat);
	Target->SetActorRotation(NewRotator);
}

void AGizmoRotate::OnRelease(int mx, int my) {}

bool AGizmoRotate::Intersects(FRay ray, FVector& Axis, float& hitDistance)
{
	if (Geometry::IsRayIntersectWithSphere(ray, RootComponent->GetComponentLocation(), 1.5f, hitDistance))
	{
		if (!Geometry::IsRayIntersectWithSphere(ray, RootComponent->GetComponentLocation(), 1.2f, hitDistance))
		{
			// 기즈모 겹치면 앞에있는걸로 하기
			float Nearest = FLT_MAX;
			FVector NearestAxis = FVector(0, 0, 0);
			float distance;
			// X 방향 기즈모
			FMatrix tf = RootComponent->GetWorldMatrix();
			FVector NormalX = (FMatrix(FRotator(90, 0, 0)) * tf).TransformDirectionVector(FVector(0, 0, 1));
			if (Geometry::IsRayIntersectWithPlane(ray, NormalX, RootComponent->GetComponentLocation(), distance))
			{
				NearestAxis = distance < Nearest ? RootComponent->GetWorldMatrix().TransformDirectionVector(FVector(1, 0, 0)) : NearestAxis;
				distance = distance < Nearest ? distance : Nearest;
			}
			FVector NormalY = (FMatrix(FRotator(0, -90, 0)) * tf).TransformDirectionVector(FVector(0, 0, 1));
			if (Geometry::IsRayIntersectWithPlane(ray, NormalY, RootComponent->GetComponentLocation(), distance))
			{
				NearestAxis = distance < Nearest ? RootComponent->GetWorldMatrix().TransformDirectionVector(FVector(1, 0, 0)) : NearestAxis;
				distance = distance < Nearest ? distance : Nearest;
			}
			FVector NormalZ = (FMatrix(FRotator(-90, 0, 0)) * tf).TransformDirectionVector(FVector(0, 0, 1));
			if (Geometry::IsRayIntersectWithPlane(ray, NormalZ, RootComponent->GetComponentLocation(), distance))
			{
				NearestAxis = distance < Nearest ? RootComponent->GetWorldMatrix().TransformDirectionVector(FVector(1, 0, 0)) : NearestAxis;
				distance = distance < Nearest ? distance : Nearest;
			}

			if (Nearest < FLT_MAX - FLT_EPSILON)
			{
				Axis = NearestAxis;
				return true;
			}
		}
	}

	return false;

}