//グローバル
matrix g_mWVP;		
Texture2D g_texDecal;

float4 g_UvSrc;	//切り取り位置
float4 g_Color;	//描画色

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

//バーテックスバッファー出力構造体
struct VS_OUTPUT
{	
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm : NORMAL;
	float4 Color : COLOR;
};

//
//バーテックスシェーダー
//
VS_OUTPUT VS( VS_IN Input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

	//位置
	output.Pos = mul(Input.Pos, g_mWVP);
	output.Norm = normalize(mul(Input.Norm, (float3x3)g_mWVP));
		
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
