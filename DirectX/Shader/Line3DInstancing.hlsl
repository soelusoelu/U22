struct VS_IN
{
    float4 Pos : POSITION;
    matrix wp : MATRIX; //�C���X�^���X���Ƃɐݒ肳��郏�[���h*�v���W�F�N�V�����s��
    float4 color : COLOR; //�C���X�^���X���Ƃɐݒ肳���F
    uint InstanceID : SV_InstanceID; //�C���X�^���XID
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT VS(VS_IN input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = mul(input.wp, input.Pos);
    output.color = input.color;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
}