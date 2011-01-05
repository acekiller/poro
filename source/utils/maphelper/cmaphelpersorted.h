///////////////////////////////////////////////////////////////////////////////
//
// CMapHelperSorted
// ================
//
//
// Created 20.07.2006 by Pete
//=============================================================================
//.............................................................................
#ifndef INC_CMAPHELPERSORTED_H
#define INC_CMAPHELPERSORTED_H

#pragma warning ( disable : 4503 )

#include <map>
#include <list>
#include "../debug.h"

#pragma warning ( disable : 4503 )

namespace ceng {

template < class T >
struct CMapHelperSorter
{
	bool operator()( const T& left, const T& right ) const
	{
		return ( left < right );
	}
};

template < class T1, class T2, class SORTER = CMapHelperSorter< T2 >, class PR = std::less< T1 > >
class CMapHelperSorted
{
public:
	
	typedef typename std::map<  T1, std::list< T2 >, PR >::iterator	Iterator;
	typedef std::list< T2 >											List;
	typedef typename std::list< T2 >::iterator						ListIterator;
	typedef SORTER													ListSorter;

	List*			HelperList;
	ListIterator	i;
	
	Iterator Begin() { return myMap.begin(); }
	Iterator End() { return myMap.end(); }

	void HelperGet( const T1& me )
	{

		HelperList = &Get( me );
	}

	CMapHelperSorted() { }
	~CMapHelperSorted() { }

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
			ListIterator i = myMap[ first ].end();
			--i;

			ListSorter sort;
			//bool pushed = false;
			for( ; i != myMap[ first ].end(); --i )
			{
				if( i == myMap[ first ].begin() )
				{
					if( sort( *i, second ) )
					{
						// for the fucking bug in insert
						if( myMap[ first ].size() == 1 )
						{
							myMap[ first ].push_back( second );
							return;
						}
						else
						{
							myMap[ first ].insert( i, second );
							return;
						}
					}
					else
					{
						myMap[ first ].push_front( second );
						return;
					}
				}

				if( sort( *i, second ) )
				{
					myMap[ first ].insert( i, second );
					return;
				}

			}
			/*
			myMap[ first ].push_back( second );
			myMap[ first ].sort( ListSorter() );
			*/
			cassert( false && "this should happen" );
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
		
		while( j != i->second.end() )
		{
			if ( (*j) == second ) 
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

} // end of namespace ceng

#endif