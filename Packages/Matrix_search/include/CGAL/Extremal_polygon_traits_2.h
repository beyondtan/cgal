#line 1480 "mon_search.aw"
#line 18 "code_formatting.awi"
// ============================================================================
//
// Copyright (c) 1998 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       : $CGAL_Revision $
// release_date  : $CGAL_Date $
//
// file          : Extremal_polygon_traits_2.h
// chapter       : $CGAL_Chapter: Geometric Optimisation $
// package       : $CGAL_Package: Matrix_search $
// source        : mon_search.aw
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Michael Hoffmann <hoffmann@inf.ethz.ch>
//
// coordinator   : ETH Zurich (Bernd Gaertner <gaertner@inf.ethz.ch>)
//
// Predefined Traits classes for Extremal Polygon Computation
// ============================================================================

#line 1484 "mon_search.aw"
#line 54 "code_formatting.awi"
#if ! (EXTREMAL_POLYGON_TRAITS_2_H)
#define EXTREMAL_POLYGON_TRAITS_2_H 1

#line 1459 "mon_search.aw"
#ifndef CGAL_OPTIMISATION_ASSERTIONS_H
#include <CGAL/optimisation_assertions.h>
#endif // CGAL_OPTIMISATION_ASSERTIONS_H
#ifndef CGAL_SQUARED_DISTANCE_2_H
#include <CGAL/squared_distance_2.h>
#endif // CGAL_SQUARED_DISTANCE_2_H
#ifndef CGAL_POLYGON_2_H
#include <CGAL/Polygon_2.h>
#endif // CGAL_POLYGON_2_H
#ifndef CGAL_FUNCTION_OBJECTS_H
#include <CGAL/function_objects.h>
#endif // CGAL_FUNCTION_OBJECTS_H

#line 46 "code_formatting.awi"
CGAL_BEGIN_NAMESPACE
#line 1471 "mon_search.aw"
#line 20 "traits.awi"
template < class R > inline
#ifndef CGAL_CFG_RETURN_TYPE_BUG_1
typename R::FT
#else
R_FT_return(R)
#endif
Kgon_triangle_area( const Point_2< R >& p,
                         const Point_2< R >& q,
                         const Point_2< R >& r)
{
  return abs( p.x() * ( q.y() - r.y()) +
                   q.x() * ( r.y() - p.y()) +
                   r.x() * ( p.y() - q.y()));
}

#line 72 "traits.awi"
#line 42 "traits.awi"
template < class _R >
class _Kgon_area_operator
: public CGAL_STD::binary_function< Point_2< _R >,
                                    Point_2< _R >,
                                    typename _R::FT >
{
public:
  typedef _R                 R;
  typedef Point_2< R >  Point_2;
  typedef typename R::FT     FT;

  _Kgon_area_operator( const Point_2& p)
  : root( p)
  {}

  FT
  operator()( const Point_2& p, const Point_2& q) const
  { return Kgon_triangle_area( p, q, root); }

private:
  const Point_2& root;
};


#line 73 "traits.awi"

template < class _R >
class Kgon_area_traits
{
public:
  typedef          _R                   R;
  typedef          Point_2< R >    Point_2;
  typedef typename _R::FT               FT;
  typedef _Kgon_area_operator< R > Operation;

  int
  min_k() const
  { return 3; }

  FT
  init( const Point_2&, const Point_2&) const
  { return FT( 0); }

  Operation
  operation( const Point_2& p) const
  { return Operation( p); }

#ifndef CGAL_CFG_NO_MEMBER_TEMPLATES
  template < class RandomAccessIC, class OutputIterator >
#else
  typedef typename vector< Point_2 >::iterator
    RandomAccessIC;
  typedef typename vector< int >::reverse_iterator
    OutputIterator;
#endif
  OutputIterator
  compute_min_k_gon( RandomAccessIC points_begin,
                     RandomAccessIC points_end,
                     FT& max_area,
                     OutputIterator o) const
  // RandomAccessIC is a random access iterator or
  // circulator with value_type Point_2.
  // OutputIterator accepts int as value_type.
  //
  // PRE: n := | [points_begin, points_end) | >= min_k() and
  //  the points described by the range [points_begin, points_end)
  //  form the boundary of a convex polygon oriented counterclockwise.
  //
  // POST: write the points of [points_begin, points_end)
  //  forming a min_k()-gon rooted at points_begin[0]
  //  of maximum area to o in counterclockwise order and return
  //  the past-the-end iterator for that range (== o + min_k()).
  {
    #line 205 "traits.awi"
    int number_of_points(
      iterator_distance( points_begin, points_end));
    CGAL_optimisation_precondition( number_of_points > min_k());
    
    // this gives the area of the triangle of two points with
    // the root:
    Operation op( operation( points_begin[0]));
    
    int p1( 1);
    int p2( 2);
    
    // maximal triangle so far (and the corresponding points):
    max_area = op( points_begin[p1], points_begin[p2]);
    int opt_p1( p1);
    int opt_p2( p2);
    
    // maximal triangle containing p1 so far:
    FT tmp_area( max_area);
    
    for (;;) {
      while ( p2 + 1 < number_of_points &&
              tmp_area < op( points_begin[p1], points_begin[p2+1])) {
        tmp_area = op( points_begin[p1], points_begin[++p2]);
      }
      if ( tmp_area > max_area) {
        max_area = tmp_area;
        opt_p1 = p1;
        opt_p2 = p2;
      }
      if ( ++p1 == number_of_points - 1)
        break;
      if ( p2 == p1)
        ++p2;
      tmp_area = op( points_begin[p1], points_begin[p2]);
    } // for (;;)
    
    // give result:
    *o++ = opt_p2;
    *o++ = opt_p1;
    *o++ = 0;
    return o;
#line 122 "traits.awi"
  } // compute_min_k_gon( ... )

  #line 130 "traits.awi"
  #ifndef CGAL_CFG_NO_MEMBER_TEMPLATES
  template < class RandomAccessIC >
  #endif
  bool
  is_convex( RandomAccessIC points_begin,
             RandomAccessIC points_end) const
  // PRE: value_type of RandomAccessIC is Point_2
  // POST: return true, iff the points [ points_begin, points_end)
  //   form a convex chain.
  {
    typedef Polygon_traits_2< R >        P_traits;
    typedef vector< Point_2 >            Cont;
    typedef Polygon_2< P_traits, Cont >  Polygon_2;
  
    Polygon_2 p( points_begin, points_end);
    return p.is_convex();
  } // is_convex( points_begin, points_end)
#line 125 "traits.awi"

};

#line 1472 "mon_search.aw"
#line 251 "traits.awi"
#line 50 "code_formatting.awi"
CGAL_END_NAMESPACE
#line 252 "traits.awi"
#ifndef CGAL_OPTIMISATION_ASSERTIONS_H
#include <CGAL/optimisation_assertions.h>
#endif // CGAL_OPTIMISATION_ASSERTIONS_H
#ifndef CGAL_PROTECT_CMATH
#include <cmath>
#define CGAL_PROTECT_CMATH
#endif
#ifdef CGAL_USE_LEDA
#ifndef CGAL_LEDA_REAL_H
#include <CGAL/leda_real.h>
#endif // CGAL_LEDA_REAL_H
#endif
#line 46 "code_formatting.awi"
CGAL_BEGIN_NAMESPACE
#line 263 "traits.awi"

#ifndef CGAL_CFG_NO_NAMESPACE
inline double
sqrt( double x)
{ return ::sqrt( x); }

#ifdef CGAL_USE_LEDA
inline leda_real
sqrt( const leda_real& x)
{ return ::sqrt( x); }
#endif
#endif

template < class _FT >
struct Sqrt
: public CGAL_STD::binary_function< _FT, _FT, _FT >
{
  typedef _FT  FT;

  FT
  operator()( const FT& x) const
  { return CGAL::sqrt( x); }

};
#line 332 "traits.awi"
#line 298 "traits.awi"
template < class _R >
class _Kgon_perimeter_operator
: public CGAL_STD::binary_function< Point_2< _R >,
                                    Point_2< _R >,
                                    typename _R::FT >
{
public:
  typedef _R              R;
  typedef Point_2< R >    Point_2;
  typedef typename R::FT  FT;

  _Kgon_perimeter_operator( const Point_2& p)
  : root( p)
  {}

  FT
  operator()( const Point_2& p, const Point_2& q) const
  { return dist( p, root) + dist( p, q) - dist( q, root); }

private:
  static
  FT
  dist( const Point_2& p, const Point_2& q)
  { return CGAL::sqrt( squared_distance( p, q)); }

  const Point_2& root;
};

#line 333 "traits.awi"

template < class _R >
class Kgon_perimeter_traits
{
public:
  typedef          _R                    R;
  typedef          Point_2< R >          Point_2;
  typedef typename _R::FT                FT;
  typedef _Kgon_perimeter_operator< R >  Operation;

  int
  min_k() const
  { return 2; }

  FT
  init( const Point_2& p, const Point_2& r) const
  { return operation( r)( p, r); }

  Operation
  operation( const Point_2& p) const
  { return Operation( p); }

#ifndef CGAL_CFG_NO_MEMBER_TEMPLATES
  template < class RandomAccessIC, class OutputIterator >
#else
  typedef typename vector< Point_2 >::iterator
    RandomAccessIC;
  typedef typename vector< int >::reverse_iterator
    OutputIterator;
#endif
  OutputIterator
  compute_min_k_gon( RandomAccessIC points_begin,
                     RandomAccessIC points_end,
                     FT& max_perimeter,
                     OutputIterator o) const
  // RandomAccessIC is a random access iterator or
  // circulator with value_type Point_2.
  // OutputIterator has value_type Point_2.
  //
  // PRE: n := | [points_begin, points_end) | >= min_k() and
  //  the points described by the range [points_begin, points_end)
  //  form the boundary of a convex polygon oriented counterclockwise.
  //
  // POST: write the points of [points_begin, points_end)
  //  forming a min_k()-gon rooted at points_begin[0] of maximum
  //  perimeter to o in counterclockwise order and return the
  //  past-the-end iterator for that range (== o + min_k()).
  {
#ifndef CGAL_CFG_NO_NAMESPACE
    using std::bind2nd;
    using std::less;
    using std::max_element;
#endif

    #line 401 "traits.awi"
    CGAL_optimisation_precondition_code(
      int number_of_points(
        iterator_distance( points_begin, points_end));)
    CGAL_optimisation_precondition( number_of_points > min_k());
    
    // kind of messy, but first we have to have something
    // like Distance (function object) ...
    RandomAccessIC maxi(
      max_element(
        points_begin + 1,
        points_end,
        compose2_2(
          less< FT >(),
          bind2nd( operation( points_begin[0]), points_begin[0]),
          bind2nd( operation( points_begin[0]), points_begin[0]))));
    
    // give result:
    *o++ = iterator_distance( points_begin, maxi);
    *o++ = 0;
    
    return o;
#line 388 "traits.awi"
  } // compute_min_k_gon( ... )

  #line 130 "traits.awi"
  #ifndef CGAL_CFG_NO_MEMBER_TEMPLATES
  template < class RandomAccessIC >
  #endif
  bool
  is_convex( RandomAccessIC points_begin,
             RandomAccessIC points_end) const
  // PRE: value_type of RandomAccessIC is Point_2
  // POST: return true, iff the points [ points_begin, points_end)
  //   form a convex chain.
  {
    typedef Polygon_traits_2< R >        P_traits;
    typedef vector< Point_2 >            Cont;
    typedef Polygon_2< P_traits, Cont >  Polygon_2;
  
    Polygon_2 p( points_begin, points_end);
    return p.is_convex();
  } // is_convex( points_begin, points_end)
#line 391 "traits.awi"

};

#line 1473 "mon_search.aw"

#line 199 "mon_search.aw"
template < class RandomAccessIC,
           class OutputIterator >
inline
OutputIterator
maximum_area_inscribed_k_gon(
  RandomAccessIC points_begin,
  RandomAccessIC points_end,
  int k,
  OutputIterator o)
#line 168 "mon_search.aw"
//
// preconditions:
// --------------
//  * Traits fulfills the requirements for an extremal polygon
//    traits class
//  * the range [points_begin, points_end) of size n > 0
//    describes the vertices of a convex polygon $P$
//    enumerated clock- or counterclockwise
#line 209 "mon_search.aw"
//  * value_type of RandomAccessIC (=: Point_2)
//    is Point_2<R> for some representation class R
//  * OutputIterator accepts Point_2 as value_type
//  * k >= 3
#line 185 "mon_search.aw"
//
// functionality:
// --------------
// computes maximum area inscribed k-gon $P_k$
// of the polygon $P$,
#line 193 "mon_search.aw"
// writes the indices (relative to points_begin)
// of $P_k$'s vertices to o and
// returns the past-the-end iterator of that sequence.
#line 215 "mon_search.aw"
{
  return _CGAL_maximum_area_inscribed_k_gon(
    points_begin,
    points_end,
    k,
    o,
    std::value_type( points_begin));
} // maximum_area_inscribed_k_gon( ... )

template < class RandomAccessIC,
           class OutputIterator,
           class R >
inline
OutputIterator
_CGAL_maximum_area_inscribed_k_gon(
  RandomAccessIC points_begin,
  RandomAccessIC points_end,
  int k,
  OutputIterator o,
  Point_2< R >*)
#line 168 "mon_search.aw"
//
// preconditions:
// --------------
//  * Traits fulfills the requirements for an extremal polygon
//    traits class
//  * the range [points_begin, points_end) of size n > 0
//    describes the vertices of a convex polygon $P$
//    enumerated clock- or counterclockwise
#line 236 "mon_search.aw"
//  * R is a CGAL representation class
//  * value_type of RandomAccessIC is Point_2<R>
//  * OutputIterator accepts Point_2<R> as value_type
//  * k >= 3
#line 185 "mon_search.aw"
//
// functionality:
// --------------
// computes maximum area inscribed k-gon $P_k$
// of the polygon $P$,
#line 193 "mon_search.aw"
// writes the indices (relative to points_begin)
// of $P_k$'s vertices to o and
// returns the past-the-end iterator of that sequence.
#line 242 "mon_search.aw"
{
  return extremal_polygon(
    points_begin,
    points_end,
    k,
    o,
    Kgon_area_traits< R >());
} // _CGAL_maximum_area_inscribed_k_gon( ... )

#line 199 "mon_search.aw"
template < class RandomAccessIC,
           class OutputIterator >
inline
OutputIterator
maximum_perimeter_inscribed_k_gon(
  RandomAccessIC points_begin,
  RandomAccessIC points_end,
  int k,
  OutputIterator o)
#line 168 "mon_search.aw"
//
// preconditions:
// --------------
//  * Traits fulfills the requirements for an extremal polygon
//    traits class
//  * the range [points_begin, points_end) of size n > 0
//    describes the vertices of a convex polygon $P$
//    enumerated clock- or counterclockwise
#line 209 "mon_search.aw"
//  * value_type of RandomAccessIC (=: Point_2)
//    is Point_2<R> for some representation class R
//  * OutputIterator accepts Point_2 as value_type
//  * k >= 2
#line 185 "mon_search.aw"
//
// functionality:
// --------------
// computes maximum perimeter inscribed k-gon $P_k$
// of the polygon $P$,
#line 193 "mon_search.aw"
// writes the indices (relative to points_begin)
// of $P_k$'s vertices to o and
// returns the past-the-end iterator of that sequence.
#line 215 "mon_search.aw"
{
  return _CGAL_maximum_perimeter_inscribed_k_gon(
    points_begin,
    points_end,
    k,
    o,
    std::value_type( points_begin));
} // maximum_perimeter_inscribed_k_gon( ... )

template < class RandomAccessIC,
           class OutputIterator,
           class R >
inline
OutputIterator
_CGAL_maximum_perimeter_inscribed_k_gon(
  RandomAccessIC points_begin,
  RandomAccessIC points_end,
  int k,
  OutputIterator o,
  Point_2< R >*)
#line 168 "mon_search.aw"
//
// preconditions:
// --------------
//  * Traits fulfills the requirements for an extremal polygon
//    traits class
//  * the range [points_begin, points_end) of size n > 0
//    describes the vertices of a convex polygon $P$
//    enumerated clock- or counterclockwise
#line 236 "mon_search.aw"
//  * R is a CGAL representation class
//  * value_type of RandomAccessIC is Point_2<R>
//  * OutputIterator accepts Point_2<R> as value_type
//  * k >= 2
#line 185 "mon_search.aw"
//
// functionality:
// --------------
// computes maximum perimeter inscribed k-gon $P_k$
// of the polygon $P$,
#line 193 "mon_search.aw"
// writes the indices (relative to points_begin)
// of $P_k$'s vertices to o and
// returns the past-the-end iterator of that sequence.
#line 242 "mon_search.aw"
{
  return extremal_polygon(
    points_begin,
    points_end,
    k,
    o,
    Kgon_perimeter_traits< R >());
} // _CGAL_maximum_perimeter_inscribed_k_gon( ... )

#line 50 "code_formatting.awi"
CGAL_END_NAMESPACE
#line 1477 "mon_search.aw"

#endif // ! (EXTREMAL_POLYGON_TRAITS_2_H)

#line 12 "code_formatting.awi"
// ----------------------------------------------------------------------------
// ** EOF
// ----------------------------------------------------------------------------

