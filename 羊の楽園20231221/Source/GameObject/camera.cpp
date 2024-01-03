#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\player.h"
#include "..\GameObject\follow.h"
#include "..\Scene\scene.h"
#include "..\App\input.h"
#include "..\GameObject\score.h"

#define CAMERA_POS_Y 8.0f		//�J�������WY
#define CAMERA_POS_Z -6.5f		//�J�������WZ
#define CAMERA_MOVE_VALUE 0.3f	//�A�C�e��1�ɂ��J�����̎���L���肷��l
#define CAMERA_MOVE_LIMIT 10.0f	//�J�����̎���L����̏��
#define CAMERA_SPEED 0.3f		//�J�����̑��x
#define SHAKE_SPEED  1.5f		//�U���̋���

void Camera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, CAMERA_POS_Y, CAMERA_POS_Z);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Camera::Update()
{
	switch (m_CameraState)
	{
	case CAMERA_STATE::TITLE:
		UpdateTitle();
		break;
	case CAMERA_STATE::GAME:
		UpdateGame();
		break;
	case CAMERA_STATE::RESULT:
		UpdateResult();
		break;
	}

	//�J�����V�F�C�N
	m_ShakeOffset = sinf(m_ShakeTime * SHAKE_SPEED) * m_ShakeAmplitude; //1.5f�͐U���̑�������
	m_ShakeTime++;
	m_ShakeAmplitude *= 0.9f;
}

void Camera::Draw()
{
	// �r���[�}�g���N�X�ݒ�
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 position = m_Position + D3DXVECTOR3(m_ShakeOffset, 0.0f, 0.0f);
	D3DXVECTOR3 target = m_Target + D3DXVECTOR3(m_ShakeOffset, 0.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &up);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMateix, 1.0f,
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&m_ProjectionMateix);
}

bool Camera::CheckView(D3DXVECTOR3 Position, D3DXVECTOR3 Scale)
{
	D3DXMATRIX vp, invvp;
	vp = m_ViewMatrix * m_ProjectionMateix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vpos[4];
	D3DXVECTOR3 wpos[4];

	// �X�P�[����K�p
	vpos[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vpos[1] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vpos[2] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vpos[3] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	// �r���[�t���X�^���̊e���_�𓧎��ϊ����ă��[���h���W�ɕϊ�
	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	D3DXVECTOR3 v, v1, v2, n;
	v = Position - m_Position;

	// ���ʔ���
	v1 = wpos[0] - m_Position;
	v2 = wpos[2] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);	
	float distance = D3DXVec3Dot(&n, &v);//�ǂ���̋���
	if (distance < -Scale.x) {return false;}
	
	// �E�ʔ���
	v1 = wpos[3] - m_Position;
	v2 = wpos[1] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);
	distance = D3DXVec3Dot(&n, &v);
	if (distance < -Scale.x) { return false; }

	// ��ʔ���
	v1 = wpos[1] - m_Position;
	v2 = wpos[0] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);
	distance = D3DXVec3Dot(&n, &v);
	if (distance < -Scale.z) { return false; }

	// ���ʔ���
	v1 = wpos[2] - m_Position;
	v2 = wpos[3] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);

	distance = D3DXVec3Dot(&n, &v);
	if (distance < -Scale.z) { return false; }

	return true;
}

void Camera::UpdateTitle()
{	//�J�����ʒu�̐ݒ�
	m_Target = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	m_Position = m_Target + D3DXVECTOR3(-1.0f, 2.0f, -3.0f);
}

void Camera::UpdateGame()
{
	//�O����W�ɒl��ێ�
	D3DXVECTOR3 OldTarget = m_Target;

	//���擾
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Score* score = scene->GetGameObject<Score>();
	auto follows = scene->GetGameObjects<Follow>();

	float  cameraMove = 0;	//�J�����̈ʒu����
	int  followmove = 1;	//���݂̒��Ԃ̐�

	for (Follow * follow : follows)
	{
		D3DXVECTOR3 position = follow->GetPosition();
		if (follow->GetState() != FOLLOW_STATE::FREE && follow->GetState() != FOLLOW_STATE::DEATH)
		{
			cameraMove += CAMERA_MOVE_VALUE;
			followmove++;
		}
	}
	if (followmove > 99) { followmove = 99; }
	score->SetCount(followmove);		//���݂̒��Ԃ̐��̃f�[�^�𑗂�

	//����͈͂̒���
	cameraMove += CAMERA_MOVE_VALUE * player->GetEye();		//���肵���A�C�e�����A����L����
	if (cameraMove >= CAMERA_MOVE_LIMIT) { cameraMove = CAMERA_MOVE_LIMIT; }

	//�g�b�v�r���[(�}���I3D���[���h)
	m_Target = player->GetPosition();
	m_Target += D3DXVECTOR3(0.0f, cameraMove, -cameraMove);

	//�O����W�Ɣ�r���Đ��`�⊮
	D3DXVECTOR3 distance = m_Target - OldTarget;
	float speed = CAMERA_SPEED;
	D3DXVECTOR3 velocity = distance * speed;
	D3DXVECTOR3 newCameraPosition = OldTarget + velocity;
	m_Target = newCameraPosition;

	//�J�����ʒu�̐ݒ�
	m_Position = m_Target + D3DXVECTOR3(0.0f, CAMERA_POS_Y, CAMERA_POS_Z);	//�ʏ�J����
}

void Camera::UpdateResult()
{
	//�J�����ʒu�̐ݒ�
	m_Target = D3DXVECTOR3(-3.5f, 0.0f, 6.0f);
	m_Position = m_Target + D3DXVECTOR3(0.0f, 5.5f, -15.0f);
}
