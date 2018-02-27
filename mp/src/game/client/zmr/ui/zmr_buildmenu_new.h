#pragma once

#include "baseviewport.h"
#include "hudelement.h"


#include <vgui_controls/EditablePanel.h>
#include <game/client/iviewport.h>


#include "zmr_menu_world.h"
#include "zmr_buildmenu_base.h"
#include "zmr/zmr_shareddefs.h"



class CZMRadialButton;
class CZMRadialPanel;
class CZMImageRow;

class CZMBuildMenuNew : public CZMBuildMenuBase, public CZMWorldMenu
{
public:
    DECLARE_CLASS_SIMPLE( CZMBuildMenuNew, CZMBuildMenuBase );


    CZMBuildMenuNew( vgui::Panel* pParent );
    ~CZMBuildMenuNew();

    virtual void ShowPanel( bool state ) OVERRIDE;
	virtual void OnThink() OVERRIDE;

    virtual void OnMousePressed( vgui::MouseCode code ) OVERRIDE;


    
    MESSAGE_FUNC_PARAMS( OnImageRowPressed, "OnImageRowPressed", kv );
    MESSAGE_FUNC_PARAMS( OnRadialOver, "OnRadialOver", kv );
    MESSAGE_FUNC_PARAMS( OnRadialLeave, "OnRadialLeave", kv );

    virtual const char* GetName() OVERRIDE { return "ZMBuildMenuNew"; };

    virtual void ShowMenu( C_ZMEntZombieSpawn* pSpawn ) OVERRIDE;
    virtual void UpdateQueue( const int queue[], int size ) OVERRIDE;

private:
    void UpdateButtons();

    CZMRadialPanel* m_pRadial;
    CZMImageRow* m_pImageList;
    int m_iCurTooltip;

    vgui::IImage* m_pQueueImages[ZMCLASS_MAX];
};