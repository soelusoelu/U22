Texture2D texDecal : register(t0);
SamplerState samLinear : register(s0);

struct VS_IN
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD0;
    matrix wp : MATRIX; //�C���X�^���X���Ƃɐݒ肳��郏�[���h*�v���W�F�N�V�����s��
    float4 color : COLOR; //�C���X�^���X���Ƃɐݒ肳���F
    float4 uv : TEXCOORD1; //�C���X�^���X���Ƃɐݒ肳���UV
    uint InstanceID : SV_InstanceID; //�C���X�^���XID
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 color : COLOR;
};

VS_OUTPUT VS(VS_IN input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = mul(input.wp, input.Pos);
    output.color = input.color;
    float4 uv = input.uv;
    output.UV = input.UV * float2(uv.z - uv.x, uv.w - uv.y) + float2(uv.x, uv.y);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 col = texDecal.Sample(samLinear, input.UV);
    col *= input.color;
    return col;
}