#define MAX_BONE 255

//�O���[�o��
matrix g_mWVP;		
Texture2D g_texDecal;

float4 g_UvSrc;	//�؂���ʒu
float4 g_Color;	//�`��F

matrix g_matBone[MAX_BONE];	//�{�[���̃|�[�Y�s��

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm:NORMAL;
	uint4 Bone:BONE_INDEX;
	float4 Weight:BONE_WEIGHT;
};

//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct VS_OUTPUT
{	
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm : NORMAL;
	float4 Color : COLOR;
};

struct SKIN_OUT
{
	float4 Pos;
	float3 Norm;
};

SKIN_OUT Skin(VS_IN Input)
{
	SKIN_OUT Output = (SKIN_OUT)0;

	float4 Pos = float4(Input.Pos, 1);
	float3 Norm = Input.Norm;

	uint Bone;
	float Weight;
	matrix matPose;

	//�{�[��1
	Weight = Input.Weight.x;

	//�E�F�C�g0�͏������Ȃ�
	if (Weight != 0.0f)
	{
		Bone = Input.Bone.x;
		matPose = g_matBone[Bone];
		Output.Pos += Weight * mul(Pos, matPose);
		Output.Norm += Weight * mul(Norm, (float3x3)matPose);
	}

	//�{�[��2
	Weight = Input.Weight.y;

	//�E�F�C�g0�͏������Ȃ�
	if (Weight != 0.0f)
	{
		Bone = Input.Bone.y;
		matPose = g_matBone[Bone];
		Output.Pos += Weight * mul(Pos, matPose);
		Output.Norm += Weight * mul(Norm, (float3x3)matPose);
	}

	//�{�[��3
	Weight = Input.Weight.z;

	//�E�F�C�g0�͏������Ȃ�
	if (Weight != 0.0f)
	{
		Bone = Input.Bone.z;
		matPose = g_matBone[Bone];
		Output.Pos += Weight * mul(Pos, matPose);
		Output.Norm += Weight * mul(Norm, (float3x3)matPose);
	}

	//�{�[��4
	Weight = Input.Weight.w;

	//�E�F�C�g0�͏������Ȃ�
	if (Weight != 0.0f)
	{
		Bone = Input.Bone.w;
		matPose = g_matBone[Bone];
		Output.Pos += Weight * mul(Pos, matPose);
		Output.Norm += Weight * mul(Norm, (float3x3)matPose);
	}

	return Output;
}

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS( VS_IN Input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

	//�X�L���A�j���[�V����
	SKIN_OUT skin = Skin(Input);

	//�ʒu
	output.Pos = mul(skin.Pos, g_mWVP);
	output.Norm = normalize(mul(skin.Norm, (float3x3)g_mWVP));
	
	//�e�N�X�`���[���W
	//���ƍ����ŃX�P�[�����O������̓_�̍������炷
	output.Tex = Input.Tex*float2(g_UvSrc.z, g_UvSrc.w) + float2(g_UvSrc.x, g_UvSrc.y);

    return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS( VS_OUTPUT input ) : SV_Target
{
	//�e�N�X�`���F
	
	float4 color = g_texDecal.Sample( samLinear, input.Tex );
	
	//���S����
	if (color.a <= 0.0f)
		discard;
	else
	{
		//�e�N�X�`���F�ɐݒ肵���`��F��������
		color.r *= g_Color.r;
		color.g *= g_Color.g;
		color.b *= g_Color.b;
		color.a *= g_Color.a;
	}

	return color;
}

//
//�e�N�j�b�N
//
technique10 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
