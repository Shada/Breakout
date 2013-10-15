#include "resource.fx"
struct VSParticleDrawOut
{
    float3 pos : POSITION;
    float4 color : COLOR0;
    float radius : RADIUS;
};

struct VSParticleIn
{
    float3 pos              : POSITION;         //position of the particle
    float3 vel              : NORMAL;           //velocity of the particle
    float  timer            : TIMER;            //timer for the particle
    uint   type             : TYPE;             //particle type
};

VSParticleDrawOut VSScenemain(VSParticleIn input)
{
    VSParticleDrawOut output = (VSParticleDrawOut)0;
    
    //
    // Pass the point through
    //
    output.pos = input.pos;
    output.radius = 1.5;
	output.color = color;
    //  
    // calculate the color
    //
    if( input.type == 0 )
    {
        output.color = float4(0,0.1,0.1,0);
        output.radius = 1.0;
    }
    else
    {
        output.color = color*(input.timer/shellLife) + fadeColor*(0.5-input.timer/shellLife);
        output.radius = 1.0;

		if(output.color.x < 0.1f && output.color.y < 0.1f && output.color.z < 0.1f)
		{
			output.color.w = 1.0;
		}

    }

    
    return output;
}
