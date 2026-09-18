float4 main( float3 pos : POSITION ) : SV_POSITION
{
    //float 3배열 받기 때문에 뒤에 1채워서 리턴
    //float4 벡터 맨뒤에 1붙으면 위치, 0이면 벡터
    return float4(pos, 1);
}