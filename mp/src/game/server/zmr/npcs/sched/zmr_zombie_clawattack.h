#pragma once


#include "zmr/npcs/zmr_zombiebase.h"

class ClawAttackSched : public NPCR::CSchedule<CZMBaseZombie>
{
private:
    CountdownTimer m_FinishTimer;
    Vector m_vecLastFacing;
public:
    ClawAttackSched()
    {

    }
    ~ClawAttackSched()
    {
            
    }

    virtual const char* GetName() const OVERRIDE { return "ZombieClawAttack"; }


    virtual void OnStart() OVERRIDE
    {
        CZMBaseZombie* pOuter = GetOuter();

        CBaseEntity* pEnemy = pOuter->GetEnemy();
        if ( !pEnemy )
        {
            End( "No enemy to attack!" );
            return;
        }


        if ( !pOuter->SetActivity( ACT_MELEE_ATTACK1 ) )
        {
            End( "Couldn't start claw attack activity!" );
            return;
        }


        float wait = pOuter->SequenceDuration();
        pOuter->SetNextAttack( gpGlobals->curtime + wait );
        //pOuter->SetNextMove( gpGlobals->curtime + wait );

        m_FinishTimer.Start( wait );
        m_vecLastFacing = pEnemy->WorldSpaceCenter();
    }

    virtual void OnUpdate() OVERRIDE
    {
        if ( m_FinishTimer.IsElapsed() )
        {
            End( "Successfully finished attacking!" );
            return;
        }


        CZMBaseZombie* pOuter = GetOuter();


        // If we for some reason lose the enemy, keep facing to the old direction.
        CBaseEntity* pEnemy = pOuter->GetEnemy();
        Vector face = pEnemy ? pEnemy->WorldSpaceCenter() : m_vecLastFacing;

        pOuter->GetMotor()->FaceTowards( pEnemy->WorldSpaceCenter() );

        m_vecLastFacing = face;
    }

    virtual void OnAnimActivityInterrupted( Activity newActivity ) OVERRIDE
    {
        if ( IsDone() )
            return;

        if ( GetOuter()->GetActivity() != ACT_MELEE_ATTACK1 )
            return;


        End( "Claw attack was interrupted!" );
    }

    // Wait for us to finish before doing something else.
    virtual NPCR::QueryResult_t ShouldChase( CBaseEntity* pEnemy ) const OVERRIDE
    {
        return (!IsDone()) ? NPCR::RES_NO : NPCR::RES_NONE;
    }
};