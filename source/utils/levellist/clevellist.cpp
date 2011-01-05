#include "clevellist.h"

namespace ceng {

std::string CLevelList::GetLevelFile( int i ) const
{
	if( i >= 0 && i < (int)myLevels.size() )
		return myLevels[ i ].filename;
	
	return "";
}

int CLevelList::GetCurrentLevel() const
{
	return myCurrentLevel;
}

std::string CLevelList::GetNextLevel() 
{
	if( myCurrentLevel >= 0 && myCurrentLevel < (int)myLevels.size() )
	{
		myLevels[ myCurrentLevel ].completed = true;
		myCurrentLevel++;
	}
	
	return GetLevelFile( myCurrentLevel );
}

void CLevelList::Serialize( ceng::CXmlFileSys* filesys )
{
	XML_BindAlias( filesys, myJumpTo, "JumpTo" );
	if( filesys->IsWriting() )
	{
		int i;
		for( i = 0; i < (int)myLevels.size(); ++i )
		{
			filesys->AddElement( "Level" );
			filesys->AddAttribute( "position", i );
			filesys->AddAttribute( "filename", myLevels[ i ].filename );
			filesys->AddAttribute( "completed", myLevels[ i ].completed );
			filesys->EndElement( "Level" );
		}
	}
	else if ( filesys->IsReading() )
	{
		int i = 0;
		for( i = 0; i < filesys->GetNode()->GetChildCount(); i++ )
		{
			if( filesys->GetNode()->GetChild( i )->GetName() == "Level" ) 
			{
				if( filesys->GetNode()->GetChild( i )->HasAttribute( "position" ) && filesys->GetNode()->GetChild( i )->HasAttribute( "filename" ) ) 
				{
					int pos;
					pos = ::ceng::XML_CAnyContainerCast( filesys->GetNode()->GetChild( i )->GetAttributeValue( "position" ), pos );	
					
					std::string filename;
					filename = ::ceng::XML_CAnyContainerCast( filesys->GetNode()->GetChild( i )->GetAttributeValue( "filename" ), filename );	
					
					bool completed = false;
					if( filesys->GetNode()->GetChild( i )->HasAttribute( "completed" ) )
						completed = ::ceng::XML_CAnyContainerCast( filesys->GetNode()->GetChild( i )->GetAttributeValue( "completed" ), completed );	

					if( pos > 0 && pos < 10000 )
					{
						myLevels.resize( pos + 1 );
						myLevels[ pos ] = CLevelFile( filename, completed );
						// myLevels[ pos ].filename = filename;
						// myLevels[ pos ].completed = completed;
					}
				}
			}
		}

		if( myJumpTo )
			myCurrentLevel = myJumpTo;
	}
}

} // end of namespace ceng