#pragma once
#include "Matrix.h"

struct FMath
{
	inline static float RadToDeg(float radian)
	{
		return radian * (180.0f / PI);
	}

	inline static float DegToRad(float degree)
	{
		return degree * (PI / 180.0f);
	}

	/// <summary>
	/// scale ���� ����Ͽ� �����ϸ� ����� ����
	/// </summary>
	static FMatrix CreateScaleMatrix(float scale);

	/// <summary>
	/// scale ���͸� ����Ͽ� �����ϸ� ����� ����
	/// </summary>
	static FMatrix CreateScaleMatrix(const FVector& scale);

	/// <summary>
	/// pitch(X), yaw(Y), roll(Z) ������ ����Ͽ� ȸ�� ����� ����
	/// </summary>
	static FMatrix CreateRotationMatrix(float pitch, float yaw, float roll);

	/// <summary>
	/// rotation ���͸� ����Ͽ� ȸ�� ����� ����
	/// </summary>
	static FMatrix CreateRotationMatrix(const FVector& rotation);

	/// <summary>
	/// Location ���͸� ����Ͽ� �̵� ����� ����
	/// </summary>
	static FMatrix CreateTranslationMatrix(const FVector& location);


	/// <summary>
	/// X, Y, Z ���� ����Ͽ� �̵� ����� ����
	/// </summary>
	static FMatrix CreateTranslationMatrix(float x, float y, float z);


	/// <summary>
	/// position, rotation, scale ���͸� ����Ͽ� World ����� ����
	/// </summary>
	static FMatrix CreateWorldMatrix(const FVector& position, const FVector& rotation, const FVector& scale);
	 
	/// <summary>
	/// <para>�־��� �� ���� �� ����(X, Y, Z)�� ���� ���͸� ����Ͽ� ���� ���(���ο� ��ǥ��(����, Basis)�� ��Ÿ���� 4��4 ���)�� ����.</para>
	/// ��ǥ�迡�� ���ǵ� ��(����)�� ����(�Ǵ� ����) ��ǥ��� ��ȯ�� �� ����
	/// </summary>
	static FMatrix CreateBasisMatrix(const FVector& xAxis, const FVector& yAxis, const FVector& zAxis, const FVector& origin);

	/// <summary>
	/// View ����� ����
	/// </summary>
	/// <param name="eye">ī�޶� ��ġ</param>
	/// <param name="target">ī�޶� �ٶ󺸴� ��ġ ����</param>
	/// <param name="up">ī�޶� ���� upVector</param>
	/// <param name="isLeft">�޼� ��ǥ��� true, ������ ��ǥ��� false</param>
	//static FMatrix CreateViewMatrixByTargetPos(const FVector& eye, const FVector& target, const FVector& up, bool isLeft = true);

	/// <summary>
	/// View ����� ����
	/// </summary>
	/// <param name="eye">ī�޶� ��ġ</param>
	/// <param name="direction">ī�޶� �ٶ󺸴� ���� ����</param>
	/// <param name="up">ī�޶� ���� upVector</param>
	/// <param name="isLeft">�޼� ��ǥ��� true, ������ ��ǥ��� false</param>
	static FMatrix CreateViewMatrixByDirection(const FVector& eye, const FVector& direction, const FVector& up, bool isLeft = true);

	/// <summary>
	/// Orthographic Projection(���� ����) ����� ����
	/// </summary>
	/// <param name="screenWidth">ȭ�� ����</param>
	/// <param name="screenHeight">ȭ�� ����</param>
	/// <param name="zNear">ViewFrustum Near</param>
	/// <param name="zFar">ViewFrustum Far</param>
	/// <param name="isLeft">�޼� ��ǥ��� true, ������ ��ǥ��� false</param>
	/// <returns></returns>
	static FMatrix CreateOrthographicProjectionMatrix(float screenWidth, float screenHeight, float zNear, float zFar, bool isLeft = true);

	/// <summary>
	/// Perspective Projection(���� ����) ����� ����
	/// </summary>
	/// <param name="fov">Fov ����</param>
	/// <param name="width">ȭ�� ����</param>
	/// <param name="height">ȭ�� ����</param>
	/// <param name="zNear">ViewFrustum Near</param>
	/// <param name="zFar">ViewFrustum Far</param>
	/// <param name="isLeft">�޼� ��ǥ��� true, ������ ��ǥ��� false</param>
	/// <returns></returns>
	static FMatrix CreatePerspectiveProjectionMatrix(float fov, float width, float height, float zNear, float zFar, bool isLeft = true);

	/// <summary>
	/// Perspective Projection(���� ����) ����� ����
	/// </summary>
	/// <param name="fov">Fov ����</param>
	/// <param name="aspectRatio">ȭ�� ����</param>
	/// <param name="zNear">ViewFrustum Near</param>
	/// <param name="zFar">ViewFrustum Far</param>
	/// <param name="isLeft">�޼� ��ǥ��� true, ������ ��ǥ��� false</param>
	/// <returns></returns>
	static FMatrix CreatePerspectiveProjectionMatrix(float fov, float aspectRatio, float zNear, float zFar, bool isLeft = true);

	/// <summary>
	/// model->view->projection ��� ��ȯ�Ͽ� MVP ����� ����
	/// </summary>
	/// <param name="modelMat"></param>
	/// <param name="viewMat"></param>
	/// <param name="projMat"></param>
	/// <returns></returns>
	static FMatrix CreateMVP(const FMatrix& modelMat, const FMatrix& viewMat, const FMatrix& projMat);

	/// <summary>
	/// (���������� ����) �� (���) = ��ȯ�� ���� ������ �����ϰ�, ������� �� ���� ����(ȸ���������� �ݿ�, �����̵� ����)�� ��ȯ
	/// </summary>
	static FVector TransformNormal(const FVector& v, const FMatrix& m);
};

