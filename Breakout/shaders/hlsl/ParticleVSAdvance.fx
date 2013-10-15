#include "resource.fx"
struct VSParticleIn
{
    float3 pos              : POSITION;         //position of the particle
    float3 vel              : NORMAL;           //velocity of the particle
    float  timer            : TIMER;            //timer for the particle
    uint   type             : TYPE;             //particle type
};

VSParticleIn VSPassThroughmain(VSParticleIn input)
{
    return input;
}
