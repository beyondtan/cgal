// ============================================================================
//
// Copyright (c) 1997-2004 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       : $CGAL_Revision: CGAL-I $
// release_date  : $CGAL_Date$
//
// file          : include/CGAL/QP_solver/QP__partial_base.h
// package       : $CGAL_Package: QP_solver $
// chapter       : Quadratic Programming Engine
//
// revision      : 3.0alpha
// revision_date : 2004/06
//
// author(s)     : Sven Sch�nherr <sven@inf.ethz.ch>
// coordinator   : ETH Z�rich (Bernd G�rtner <gaertner@inf.ethz.ch>)
//
// implementation: Base Class for Partial Pricing of the QP Solver
// ============================================================================

#ifndef CGAL_QP__PARTIAL_BASE_H
#define CGAL_QP__PARTIAL_BASE_H

// includes
#include <CGAL/QP_pricing_strategy.h>
#include <CGAL/Random.h>
#include <algorithm>
#include <vector>
#include <cmath>

CGAL_BEGIN_NAMESPACE

// ==================
// class declarations
// ==================
template < class Rep_ >
class QP__partial_base;

// ===============
// class interface
// ===============
template < class Rep_ >
class QP__partial_base : virtual public QP_pricing_strategy<Rep_> {

    // self
    typedef  Rep_                       Rep;
    typedef  QP__partial_base<Rep>     Self;
    typedef  QP_pricing_strategy<Rep>  Base;

    typedef  typename Base::QP_solver   QP_solver;
  protected:

    // types
    typedef  typename QP_solver::Indices               Indices;
    typedef  typename QP_solver::Index_iterator        Index_iterator;
    typedef  typename QP_solver::Index_const_iterator  Index_const_iterator;

    // construction
    QP__partial_base( bool  randomize, Random&  random);

    // initialization
    virtual  void  init( );

    // access
    Index_const_iterator    active_set_begin( ) const { return N.begin();   }
    Index_const_iterator    active_set_end  ( ) const { return N.begin()+s; }

    Index_const_iterator  inactive_set_begin( ) const { return N.begin()+s; }
    Index_const_iterator  inactive_set_end  ( ) const { return N.end  ();   }

    // operations
    void  entering_basis( Index_const_iterator  it);
    void  activating    ( Index_const_iterator& it);

    virtual  void  leaving_basis( int i);
    virtual  void  transition( );

  private:

    // data members
    Indices                  N;         // non-basis;
    int                      s;         // size of active set

    bool                     permute;   // flag: permute initial non-basis
    Random&                  rand_src;  // random source
};

// ----------------------------------------------------------------------------

// ===============================
// class implementation (template)
// ===============================

// construction
template < class Rep_ >  inline
QP__partial_base<Rep_>::
QP__partial_base( bool  randomize, Random&  random)
    : permute( randomize), rand_src( random)
{ }

// initialization
template < class Rep_ >
void
QP__partial_base<Rep_>::
init( )
{
    // initialize indices of non-basic variables
    int  w = this->solver().number_of_working_variables();
    int  b = this->solver().number_of_basic_variables();

    if ( ! N.empty()) N.clear();
    N.reserve( w-b);                                        // use 'w' ???
    for ( int i = 0; i < w; ++i) {
	if ( ! this->solver().is_basic( i)) N.push_back( i);
    }
    if ( permute) std::random_shuffle( N.begin(), N.end(), rand_src);

    // initialize size of active set
    int  n = this->solver().number_of_variables();
    int  m = this->solver().number_of_constraints();

    s = std::min( static_cast< unsigned int>( m*std::sqrt( n/2.0)),
		  static_cast< unsigned int>(N.size()));
}

// operations
template < class Rep_ >  inline
void
QP__partial_base<Rep_>::
entering_basis( Index_const_iterator it)
{
    CGAL_qpe_precondition( it >= active_set_begin() && it < active_set_end());

    // remove from active set
    --s;
    const_cast< typename Indices::value_type&>( *it) = N[ s];
    N[ s] = N.back();
    N.pop_back();
}

template < class Rep_ >  inline
void
QP__partial_base<Rep_>::
activating( Index_const_iterator& it)
{
    CGAL_qpe_precondition(
	it >= inactive_set_begin() && it < inactive_set_end());

    // 'append' to active set
    std::swap( const_cast< typename Indices::value_type&>( *it), N[ s]);
    it = N.begin()+s;
    ++s;
}

template < class Rep_ >
void
QP__partial_base<Rep_>::
leaving_basis( int i)
{
    // all non-basic variables active?
    if ( s == static_cast< int>( N.size())) {

	// append at the end of all non-basic variables
	N.push_back( i);

    } else {

	// insert at the end of the current active subset
	N.push_back( N[ s]);
	N[ s] = i;
    }
    ++s;
}


template < class Rep_ >
void
QP__partial_base<Rep_>::
transition( )
{
    // remove artificial variables from non-basis
    int  w = this->solver().number_of_working_variables();
    N.erase( std::partition( N.begin(), N.end(),
			     std::bind2nd( std::less<int>(), w)),
	     N.end());

    // initialize size of active set
    int  n = this->solver().number_of_variables();
    int  m = this->solver().number_of_constraints();

    s = std::min( static_cast< unsigned int>( m*std::sqrt( n/2.0)),
		  static_cast< unsigned int>(N.size()));
}

CGAL_END_NAMESPACE

#endif // CGAL_QP__PARTIAL_BASE_H

// ===== EOF ==================================================================
