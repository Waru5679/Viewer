//�O���[�o��
matrix g_mWVP;		
Texture2D g_texDecal;

float4 g_UvSrc;	//�؂���ʒu
float4 g_Color;	//�`��F

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_IN
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm:NORMAL;
};

//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct VS_OUTPUT
{	
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm : NORMAL;
	float4 Color : COLOR;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS( VS_IN Input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

	//�ʒu
	output.Pos = mul(Input.Pos, g_mWVP);
	output.Norm = normalize(mul(Input.Norm, (float3x3)g_mWVP));
		
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
