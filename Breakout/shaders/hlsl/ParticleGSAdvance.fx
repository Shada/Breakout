#include "resource.fx"
Texture1D texRand	: register( t0 );

struct VSParticleIn
{
    float3 pos              : POSITION;         //position of the particle
    float3 vel              : NORMAL;           //velocity of the particle
    float  timer            : TIMER;            //timer for the particle
    uint   type             : TYPE;             //particle type
};

float3 RandomDir(float fOffset)
{
    float tCoord = ((globalTime + fOffset)) / 30;
    return texRand.SampleLevel( samPoint, tCoord, 0 );
}


float3 RandomLaunchDir(float fOffset)
{
    float tCoord = ((globalTime + fOffset));
    return texRand.SampleLevel( samPoint, tCoord, 0 );
}


//
// Generic particle motion handler
//

void GSGenericHandler( VSParticleIn input, inout PointStream<VSParticleIn> ParticleOutputStream )
{
    input.pos += input.vel*elapsedTime;
    input.vel += frameGravity*elapsedTime;
    input.timer -= elapsedTime;
    ParticleOutputStream.Append( input );
}

//
// Launcher type particle handler
//
void GSLauncherHandler( VSParticleIn input, inout PointStream<VSParticleIn> ParticleOutputStream )
{
    //for(int i=0; i<particlesPerStage; i++)
	//if(input.Timer < 0)
    {
        float3 vRandom = normalize( RandomLaunchDir( (input.type /*+ i */)) );

        VSParticleIn output;

        output.pos = startPos + radie*RandomLaunchDir(input.type)*radieDirections;

		if(randDir == 1)
			output.vel = startSpeed * vRandom;//input.vel + vRandom*8.0;
		else
			output.vel = startSpeed * abs(vRandom);

        output.timer = shellLife + vRandom.y*0.5;
        output.type = stages;
        //output.type = 1;
		
        ParticleOutputStream.Append( output );
        
    }
    
    //emit ourselves to keep us alive
    ParticleOutputStream.Append( input );
    
}

//
// Shell type particle handler
//
void GSShellHandler( VSParticleIn input, inout PointStream<VSParticleIn> ParticleOutputStream )
{
    if(input.timer <= 0)
    {
		if(input.type > 1)
		{
			VSParticleIn output;
			float3 vRandom = float3(0,0,0);

			//time to emit a series of new Ember1s  
			for(int i=0; i<particlesPerStage; i++)
			{
				vRandom = normalize( RandomDir( input.type + i ) );
				/*output.pos = input.pos + input.vel*g_fElapsedTime;
				output.vel = input.vel + vRandom*15.0;
				output.Timer = P_SHELLLIFE + vRandom.y*0.5;
				output.Type = input.Type - 1;*/


				output.pos = input.pos + radie*RandomLaunchDir(input.type)*radieDirections;

				if(randDir)
					output.vel = startSpeed * vRandom;//input.vel + vRandom*8.0;
				else
					output.vel = startSpeed * abs(vRandom);

				output.timer = shellLife + vRandom.y*0.5;
				output.type = input.type - 1;
			
				ParticleOutputStream.Append( output );
			}

		}
    }
    else
    {
        GSGenericHandler( input, ParticleOutputStream );
    }
}



[maxvertexcount(128)]
void GSAdvanceParticlesMain(point VSParticleIn input[1], inout PointStream<VSParticleIn> ParticleOutputStream)
{
	/*if(Spawned == false)
	{
		GSLauncherHandler( input[0], ParticleOutputStream );
		Spawned = true;
	}*/

    if( input[0].type == 0 )
        GSLauncherHandler( input[0], ParticleOutputStream );
    else// if ( input[0].Type == PT_SHELL )
        GSShellHandler( input[0], ParticleOutputStream );

}

GeometryShader gsStreamOut = ConstructGSWithSO( CompileShader( gs_5_0, GSAdvanceParticlesMain() ), "POSITION.xyz; NORMAL.xyz; TIMER.x; TYPE.x;" );
