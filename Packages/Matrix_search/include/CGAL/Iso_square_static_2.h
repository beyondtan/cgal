#line 206 "square.awi"
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
// file          : Iso_square_static_2.h
// chapter       : $CGAL_Chapter: Geometric Optimisation $
// package       : $CGAL_Package: Matrix_search $
// source        : pcenter.aw
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Michael Hoffmann <hoffmann@inf.ethz.ch>
//
// coordinator   : ETH Zurich (Bernd Gaertner <gaertner@inf.ethz.ch>)
//
// 2D Iso-squares with static radius
// ============================================================================

#line 210 "square.awi"
#line 54 "code_formatting.awi"
#if ! (ISO_SQUARE_STATIC_2_H)
#define ISO_SQUARE_STATIC_2_H 1

#line 26 "square.awi"
#ifndef CGAL_OPTIMISATION_ASSERTIONS_H
#include <CGAL/optimisation_assertions.h>
#endif // CGAL_OPTIMISATION_ASSERTIONS_H

#line 46 "code_formatting.awi"
CGAL_BEGIN_NAMESPACE
#line 29 "square.awi"

// class Iso_square_static_2 represents a square with center and radius.
// all objects have the same radius ==> radius is a static member
// no handle-rep stuff

template < class _R >
class Iso_square_static_2 {
public:
  typedef _R                       R;
  typedef typename _R::Point_2     Point_2;
  typedef typename _R::FT          FT;

  Iso_square_static_2() {}

  Iso_square_static_2( const Point_2& p)
  : _center( p)
  {}

  const Point_2&
  center() const
  { return _center; }

  FT
  radius() const
  { return _radius; }

  static void
  set_radius( FT r)
  {
    CGAL_optimisation_precondition( r >= 0);
    _radius = r;
  }

  FT
  xmin() const
  { return center().x() - radius(); }

  FT
  xmax() const
  { return center().x() + radius(); }

  FT
  ymin() const
  { return center().y() - radius(); }

  FT
  ymax() const
  { return center().y() + radius(); }

  bool
  has_on_bounded_side( const Point_2& p) const
  {
    return p.x() > xmin() && p.x() < xmax() &&
      p.y() > ymin() && p.y() < ymax();
  }

  bool
  has_on_unbounded_side( const Point_2& p) const
  {
    return p.x() < xmin() || p.x() > xmax() ||
      p.y() < ymin() || p.y() > ymax();
  }

private:
  Point_2      _center;
  static FT    _radius;
};

#line 103 "square.awi"
// for output to ostream:
template< class R >
ostream& operator<<( ostream& os,
                     const Iso_square_static_2< R >& s)
{
  switch(os.iword(IO::mode)) {
  case IO::ASCII :
    return os << s.center() << ' ' << s.radius();
  case IO::BINARY :
    os << s.center() << s.radius();
    // write(os, s.radius());
    return os;
  default:
    return os << "Iso_square_static_2(" << s.center() << ", "
              << s.radius() << ')';
  }
}

// for input from istream:
template < class R >
istream&
operator>>( istream& is,
            Iso_square_static_2< R >& s)
{
  typename R::Point_2  c;
  typename R::FT       r;

  is >> c >> r;

  s = Iso_square_static_2< R >( c);
  s.set_radius( r);
  return is;
}

#ifdef CGAL_WINDOW_STREAM_H

// for output to Window_stream:
template< class R >
Window_stream&
operator<<( Window_stream& w,
            const Iso_square_static_2< R >& s)
{
  w.draw_rectangle( to_double( s.xmin()),
                    to_double( s.ymin()),
                    to_double( s.xmax()),
                    to_double( s.ymax()));
  return w;
}

// for input from Window_stream:
template< class R >
Window_stream&
operator>>( Window_stream& w,
            Iso_square_static_2< R >& r)
{
  Window_stream W;
  drawing_mode old_mode( W.set_mode( leda_xor_mode));
  leda_color old_color( W.set_color( leda_red));

  // get center:
  double c_x, c_y;
  do {}
  while ( W.get_mouse( c_x, c_y) == NO_BUTTON);

  // get radius:
  double x, y, rad( 0), r_old;
  int i;
  do {
    i = W.get_mouse( x, y);
    r_old = rad;
    rad = max( fabs( x - c_x), fabs( y - c_y));
    if ( r_old != rad) {
      W.draw_rectangle( c_x - r_old, c_y - r_old,
                        c_x + r_old, c_y + r_old);
      W.draw_rectangle( c_x - rad, c_y - rad,
                        c_x + rad, c_y + rad);
    }
  } while ( i != MOUSE_BUTTON( 1));

  W.draw_rectangle( c_x - rad, c_y - rad,
                    c_x + rad, c_y + rad);
  W.set_mode( old_mode);
  W.set_color( old_color);

  r = Iso_square_static_2< R >( c_x, c_y);
  r.set_radius( rad);
  return w << r;
}

#endif // CGAL_WINDOW_STREAM_H
#line 196 "square.awi"

// the static radius of all Iso_square_static_2< R >:
template < class R >
Iso_square_static_2< R >::FT
Iso_square_static_2< R >::_radius;

#line 50 "code_formatting.awi"
CGAL_END_NAMESPACE
#line 203 "square.awi"


#endif // ! (ISO_SQUARE_STATIC_2_H)

#line 12 "code_formatting.awi"
// ----------------------------------------------------------------------------
// ** EOF
// ----------------------------------------------------------------------------

