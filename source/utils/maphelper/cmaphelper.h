///////////////////////////////////////////////////////////////////////////////
//
// CMapHelper
// ==========
//
// This is a map that can contain multiple elements under one key
//
// Handy in various places.
//
// Created xx.xx.xxxx by Pete
//.............................................................................
//
// Example program:
/*

class Turha
{
public:
	Turha( int i ) : myI( i ) { }

	int myI;
};


int main()
{

	CMapHelper< int, Turha* > myMap;

	myMap.Insert( 1, new Turha( 1 ) );
	myMap.Insert( 2, new Turha( 2 ) );
	myMap.Insert( 1, new Turha( 2 ) );


	CMapHelperForEach( myMap, 1, std::cout << (*myMap.i)->myI << std::endl );

	return 0;
}

*/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//
// 06.10.2005 Pete
//		Fixed a bug that caused the Find sometimes to return false results.
//		Fixed another bug that had crept in the macro defination. Added a unit 
//		test to bust them. Actually the unit test helped catching them in the 
//		first place.
//
//=============================================================================

#pragma warning ( disable : 4786 )

#ifndef INC_CMAPHELPER_H
#define INC_CMAPHELPER_H

#include <map>
#include <list>

#include "../debug.h"

namespace ceng {

//! This is a map that can contain multiple elements under one key
template < class T1, class T2, class PR = std::less< T1 > >
class CMapHelper
{
public:
	
	typedef typename std::map<  T1, std::list< T2 >, PR >::iterator	Iterator;
	typedef std::list< T2 >											List;
	typedef typename std::list< T2 >::iterator						ListIterator;
	
	List*			HelperList;
	ListIterator	i;
	
	Iterator Begin() { return myMap.begin(); }
	Iterator End() { return myMap.end(); }

	void Clear() { myMap.clear(); }

	void HelperGet( const T1& me )
	{

		HelperList = &Get( me );
	}

	CMapHelper() { }
	~CMapHelper() { }

	bool Find( const T1& me )
	{
		// if ( myIterator != myMap.end() && myIterator->first == me ) return true;

		
		Iterator myIterator = myMap.find( me );
		
		return ( myIterator != myMap.end() );
	}

	
	std::list< T2 >& Get( const T1& me )
	{
		// if ( myIterator != myMap.end() && myIterator->first == me ) return myIterator->second;

		return myMap[ me ];
	}

	void Insert( const T1& first, const T2& second )
	{

		if ( myMap.find( first ) != myMap.end() )
		{
			myMap[ first ].push_back( second );
			return;
		}

		List tmp_list;
		tmp_list.push_back( second );

		myMap.insert( std::pair< T1, std::list< T2 > >( first, tmp_list ) );

	}

	void RemoveFirst( const T1& me )
	{
		myMap.erase( me );
	}

	void RemoveSecond( const T2& me )
	{
		typename std::map< T1, std::list< T2 >, PR >::iterator i = myMap.begin();
		
		while( i != myMap.end() )
		{
			ListIterator j = i->second.begin();
			while( j != i->second.end() )
			{
				if( (*j) == me )
				{
					ListIterator remove = j;
					++j;
					i->second.erase( remove );
				}
				else
				{
					++j;
				}
			}

			if( i->second.empty() )
			{
				typename std::map< T1, std::list< T2 >, PR >::iterator remove = i;
				++i;
				myMap.erase( remove );
			}
			else
			{
				++i;
			}
		}
	}
	void Remove( const T1& me )
	{
		RemoveFirst( me );
	}

	void Remove( const T1& first, const T2& second )
	{
		typename std::map< T1, std::list< T2 >, PR >::iterator i;
		
		i = myMap.find( first );
		if ( i == myMap.end() ) return;

		ListIterator j = i->second.begin();
		int safty_count = 0;
		while( j != i->second.end() && safty_count < 10000 )
		{
			if( (*j) == second )
			{
				ListIterator remove = j;
				++j;
				i->second.erase( remove );
			}
			else
			{
				++j;
			}

			safty_count++;
		}

		cassert( safty_count < 10000 );

		/*
		for ( j = i->second.begin(); j != i->second.end(); ++j )
		{
			if ( (*j) == second ) 
			{
				j = i->second.erase( j );
			}
		}
		*/
		if( i->second.empty() )
		{
			myMap.erase( i );
		}
	}

	bool Empty() const
	{
		return myMap.empty();
	}


private:
	std::map< T1, std::list< T2 >, PR >						myMap;
	//typename std::map< T1, std::list< T2 >, PR >::iterator	myIterator;
	typename std::list< T2 >::iterator						myListIterator;

};



#define CMapHelperForEach( map, varname, dacode ) \
	{	if( map.Find( varname ) ) { map.HelperGet( varname ); \
		for ( map.i = map.HelperList->begin(); map.i != map.HelperList->end(); ++map.i ) \
		{ dacode; } } }
	
}

#endif
