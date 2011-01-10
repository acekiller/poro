#include "cmodifiedbuttonwidget.h"
#include "cwidgetfactory.h"
#include "cgfxhandler.h"
#include "ievent.h"
#include "imouse.h"

namespace ceng {
namespace ui {

namespace {

	
	static const types::ticks double_click_time = 300;
}
///////////////////////////////////////////////////////////////////////////////

CModifiedButtonWidget::CModifiedButtonWidget() :
	CButtonWidget(),
	myParam( CModifiedButtonWidget::parameter_type() ),
	OnMouseOver(),
	OnMouseOut(),
	mySelectionOn( NULL )
{

	// OnDrag += CSlot( this, CButtonWidget::MoveBy( types::point() ) );
}

//=============================================================================

CModifiedButtonWidget::CModifiedButtonWidget( CWidget* parent, const types::rect& rect, bool relative, const types::id& id , const std::string& sprite_img, const parameter_type& param  ) :
	CButtonWidget( parent, rect, relative, id, sprite_img ),
	myParam( param ),
	mySelectionOn( NULL )
{
	// myResizeType = types::resize_position;
}

//=============================================================================

CModifiedButtonWidget::~CModifiedButtonWidget()
{
}

///////////////////////////////////////////////////////////////////////////////

void CModifiedButtonWidget::OnMouseEvent( CMouseEvent* event )
{
	// logger << event->GetButtons() << std::endl;
	switch( event->GetType() )
	{
	case types::mouse_button_down:
		
		if( event->GetButtons() & myClickableButtons ) 
		{
			mySpriteHandler->PlayAnimation( GetSprite(), "mouse_button_down" );
			myClicked = true;
			OnMouseDown( myParam );
		}

		if( myDragable && event->GetButtons() & myDragableButtons )
		{
			// mySpriteHandler->PlayAnimation( GetSprite(), "mouse_on_drag" );
			myDragging = true;
			myDragOffset = types::point( this->GetRect().x, this->GetRect().y ) - event->GetPosition();
			IMouse::AddConstantEventListeners( this );
		}

		if( myDoubleClickable && event->GetButtons() & myDoubleClickableButtons && myDoubleClicking == true )
		{
			if( myDoubleClickTimer.GetTime() < double_click_time )
				OnDoubleClick( myParam );
			myDoubleClicking = false;
		}
		else if( myDoubleClickable && event->GetButtons() & myDoubleClickableButtons && myDoubleClicking == false )
		{
			myDoubleClickTimer.Reset();
			myDoubleClickTimer.Resume();
			myDoubleClicking = true;
		}
		else if ( myDoubleClickable && event->GetButtons() & !myDoubleClickableButtons )
		{
			myDoubleClicking = false;
		}



		break;

	case types::mouse_button_up:
		if( myClicked && event->GetButtons() & myClickableButtons ) 
		{
			mySpriteHandler->PlayAnimation( GetSprite(), "mouse_button_up" );
			OnClick( myParam );

			myClicked = false;
			OnMouseUp( myParam );
		}

		if( myDragable && event->GetButtons() & myDragableButtons )
		{
			// mySpriteHandler->PlayAnimation( GetSprite(), "mouse_on_drag" );
			myDragging = false;
			IMouse::RemoveConstantEventListeners( this );
			OnDragEnd();
		}

		myClicked = false;
		break;

	case types::mouse_move:

		if( myDragging )
		{
			types::point where_to = event->GetPosition() + myDragOffset;

			// Fixed 12-09-2007 by Pete
			//--------------------------------------------------------------------------
			// BUGBUG: This should be the commented line, but the signal-slot 
			// doesn't support three parameters at the current time
			// OnDrag( where_to.x - GetRect().x, where_to.y - GetRect().y, myParam );

			if( config::automatic_dragging )
				MoveBy( types::point( where_to.x - GetRect().x, where_to.y - GetRect().y ) );	

			OnDrag( where_to.x - GetRect().x, where_to.y - GetRect().y, myParam );
		}

		// myDoubleClicking = false;
		break;

	case types::mouse_over:
		{
			const std::string animation = mySelectionOn?"select_mouse_over":"mouse_over";
			mySpriteHandler->PlayAnimation( GetSprite(), animation );
			OnMouseOver( myParam );
		}
		break;

	case types::mouse_out:
		{
			const std::string animation = mySelectionOn?"select_mouse_out":"mouse_out";
			mySpriteHandler->PlayAnimation( GetSprite(), animation );
			myClicked = false;
			myDoubleClicking = false;
			OnMouseOut( myParam );
		}
		break;

	default:
		break;
	}

}

void CModifiedButtonWidget::SetSelectionOn( bool value )
{
	if( mySelectionOn != value )
	{
		mySelectionOn = value;

		const std::string animation = mySelectionOn?"select_mouse_out":"mouse_out";
		mySpriteHandler->PlayAnimation( GetSprite(), animation );
	}
}

bool CModifiedButtonWidget::GetSelectionOn() const
{
	return mySelectionOn;
}

	
	void CModifiedButtonWidget::OnKeyboardEvent( CKeyboardEvent* event )
	{
		if( event->GetType() == types::key_down )
		{
			switch( event->GetKey() )
			{
				case types::keys::_return:
					OnClick( myParam );
					break;
					
				case types::keys::_tab:
					OnKeyTab();
					break;
					
				case types::keys::_up:
					OnKeyUp();
					break;
					
				case types::keys::_down:
					OnKeyDown();
					break;
					
				case types::keys::_left:
					OnKeyLeft();
					break;
					
				case types::keys::_right:
					OnKeyRight();
					break;
					
				case types::keys::_escape:
					OnKeyEsc();
					break;
					
				default:
					break;
			}
		}
	}
	

///////////////////////////////////////////////////////////////////////////////
} // end of namespace ui
} // end of namespace ceng

