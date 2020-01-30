#define MAX_BONE 255

//グローバル
matrix g_mWVP;		
Texture2D g_texDecal;

float4 g_UvSrc;	//切り取り位置
float4 g_Color;	//描画色

matrix g_matBone[MAX_BONE];	//ボーンのポーズ行列

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

//バーテックスバッファー出力構造体
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

	//ボーン1
	Weight = Input.Weight.x;

	//ウェイト0は処理しない
	if (Weight != 0.0f)
	{
		Bone = Input.Bone.x;
		matPose = g_matBone[Bone];
		Output.Pos += Weight * mul(Pos, matPose);
		Output.Norm += Weight * mul(Norm, (float3x3)matPose);
	}

	//ボーン2
	Weight = Input.Weight.y;

	//ウェイト0は処理しない
	if (Weight != 0.0f)
	{
		Bone = Input.Bone.y;
		matPose = g_matBone[Bone];
		Output.Pos += Weight * mul(Pos, matPose);
		Output.Norm += Weight * mul(Norm, (float3x3)matPose);
	}

	//ボーン3
	Weight = Input.Weight.z;

	//ウェイト0は処理しない
	if (Weight != 0.0f)
	{
		Bone = Input.Bone.z;
		matPose = g_matBone[Bone];
		Output.Pos += Weight * mul(Pos, matPose);
		Output.Norm += Weight * mul(Norm, (float3x3)matPose);
	}

	//ボーン4
	Weight = Input.Weight.w;

	//ウェイト0は処理しない
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
//バーテックスシェーダー
//
VS_OUTPUT VS( VS_IN Input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

	//スキンアニメーション
	SKIN_OUT skin = Skin(Input);

	//位置
	output.Pos = mul(skin.Pos, g_mWVP);
	output.Norm = normalize(mul(skin.Norm, (float3x3)g_mWVP));
	
	//テクスチャー座標
	//幅と高さでスケーリング→左上の点の差分ずらす
	output.Tex = Input.Tex*float2(g_UvSrc.z, g_UvSrc.w) + float2(g_UvSrc.x, g_UvSrc.y);

    return output;
}

//
//ピクセルシェーダー
//
float4 PS( VS_OUTPUT input ) : SV_Target
{
	//テクスチャ色
	
	float4 color = g_texDecal.Sample( samLinear, input.Tex );
	
	//完全透過
	if (color.a <= 0.0f)
		discard;
	else
	{
		//テクスチャ色に設定した描画色をかける
		color.r *= g_Color.r;
		color.g *= g_Color.g;
		color.b *= g_Color.b;
		color.a *= g_Color.a;
	}

	return color;
}

//
//テクニック
//
technique10 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
