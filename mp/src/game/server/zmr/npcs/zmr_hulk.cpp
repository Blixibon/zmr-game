#include "cbase.h"
#include "npcevent.h"

#include "zmr_gamerules.h"
#include "zmr_hulk.h"


#define ZOMBIE_MODEL        "models/zombie/hulk.mdl"


extern ConVar zm_sk_hulk_health;
extern ConVar zm_sk_hulk_dmg;

extern ConVar zm_sk_hulk_hitmult_legs;
extern ConVar zm_sk_hulk_hitmult_head;
extern ConVar zm_sk_hulk_hitmult_stomach;
extern ConVar zm_sk_hulk_hitmult_chest;
extern ConVar zm_sk_hulk_hitmult_arms;


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
    SetMaxHealth( zm_sk_hulk_health.GetInt() );


    BaseClass::Spawn();

    // Hulk's hitboxes go beyond its collision bounds, by default this means they cannot be hit by rays.
    // We have to specify the surrounding bounds in order to get hit in these hitboxes!
    Vector mins( -32.0f, -32.0f, 0.0f );
    Vector maxs( 32.0f, 32.0f, 90.0f );
    CollisionProp()->SetSurroundingBoundsType( USE_SPECIFIED_BOUNDS, &mins, &maxs );
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

void CZMHulk::ScaleDamageByHitgroup( int iHitGroup, CTakeDamageInfo& info ) const
{
    switch ( iHitGroup )
    {
    case HITGROUP_LEFTARM :
    case HITGROUP_RIGHTARM :
        info.ScaleDamage( zm_sk_hulk_hitmult_arms.GetFloat() );
        break;
    case HITGROUP_CHEST :
        info.ScaleDamage( zm_sk_hulk_hitmult_chest.GetFloat() );
        break;
    case HITGROUP_STOMACH :
        info.ScaleDamage( zm_sk_hulk_hitmult_stomach.GetFloat() );
        break;
    case HITGROUP_HEAD :
        info.ScaleDamage( zm_sk_hulk_hitmult_head.GetFloat() );
        break;
    case HITGROUP_LEFTLEG :
    case HITGROUP_RIGHTLEG :
    default :
        info.ScaleDamage( zm_sk_hulk_hitmult_legs.GetFloat() );
        break;
    }
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

