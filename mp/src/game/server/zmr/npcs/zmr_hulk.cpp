#include "cbase.h"
#include "npcevent.h"

#include "zmr_gamerules.h"
#include "zmr_hulk.h"


#define ZOMBIE_MODEL        "models/zombie/hulk.mdl"


extern ConVar zm_sk_hulk_health;
extern ConVar zm_sk_hulk_dmg;


LINK_ENTITY_TO_CLASS( npc_poisonzombie, CZMHulk );
PRECACHE_REGISTER( npc_poisonzombie );

IMPLEMENT_SERVERCLASS_ST( CZMHulk, DT_ZM_Hulk )
END_SEND_TABLE()

CZMHulk::CZMHulk()
{
    SetZombieClass( ZMCLASS_HULK );
    CZMRules::IncPopCount( GetZombieClass() );
}

CZMHulk::~CZMHulk()
{
}

void CZMHulk::Precache()
{
    if ( !IsPrecacheAllowed() )
        return;


    PrecacheModel( ZOMBIE_MODEL );


    PrecacheScriptSound( "NPC_PoisonZombie.Die" );
    PrecacheScriptSound( "NPC_PoisonZombie.Idle" );
    PrecacheScriptSound( "NPC_PoisonZombie.Pain" );
    PrecacheScriptSound( "NPC_PoisonZombie.Alert" );
    PrecacheScriptSound( "NPC_PoisonZombie.FootstepRight" );
    PrecacheScriptSound( "NPC_PoisonZombie.FootstepLeft" );
    PrecacheScriptSound( "NPC_PoisonZombie.Attack" );

    PrecacheScriptSound( "NPC_PoisonZombie.FastBreath" );
    PrecacheScriptSound( "NPC_PoisonZombie.Moan1" );

    BaseClass::Precache();
}

void CZMHulk::Spawn()
{
    SetModel( ZOMBIE_MODEL );

    SetMaxHealth( zm_sk_hulk_health.GetInt() );


    BaseClass::Spawn();
}

NPCR::CPathCostGroundOnly* CZMHulk::GetPathCost() const
{
    static NPCR::CPathCostGroundOnly* cost = nullptr;
    if ( cost == nullptr )
    {
        cost = new NPCR::CPathCostGroundOnly;
        //cost->SetMaxHeightChange( 512.0f );
    }

    return cost;
}

void CZMHulk::HandleAnimEvent( animevent_t* pEvent )
{
	if ( pEvent->event == AE_ZOMBIE_ATTACK_RIGHT )
	{
		Vector forward, right;

		AngleVectors( GetLocalAngles(), &forward, &right, nullptr );

		right = right * 200;
		forward = forward * 200;

        QAngle viewpunch( -15, -20, -10 );
        Vector punchvel = right + forward;

		ClawAttack( GetClawAttackRange(), zm_sk_hulk_dmg.GetInt(), viewpunch, punchvel );
		return;
	}

    BaseClass::HandleAnimEvent( pEvent );
}

void CZMHulk::AlertSound()
{
    EmitSound( "NPC_PoisonZombie.Alert" );
}

void CZMHulk::AttackSound()
{
    EmitSound( "NPC_PoisonZombie.Attack" );
}

void CZMHulk::DeathSound()
{
    EmitSound( "NPC_PoisonZombie.Die" );
}

void CZMHulk::FootstepSound( bool bRightFoot )
{
    EmitSound( bRightFoot ? "NPC_PoisonZombie.FootstepRight" : "NPC_PoisonZombie.FootstepLeft" );
}

void CZMHulk::FootscuffSound( bool bRightFoot )
{
}
