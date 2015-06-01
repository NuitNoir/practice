#ifndef PPOINT_HPP_
#define PPOINT_HPP_
/*! \file ppoint.hpp
   Copyright 2012 Yusuke Kameda

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */
#include <opencv2/core/core.hpp>
#include <iostream>
namespace tutor {
/* how to use matrix
  cv::Matx33d m(1,2,3,
                4,5,6,
                7,8,9);
  cv::Matx33d a = m * 2;
  assert(m(0, 0) == 1  && m(0, 1) == 2  && m(2, 0) == 7);
*/

//! 2-dimensional projective point of 3D homogeneous coordinate.
/*! This class is almost same to cv::Point3_, see OpenCV reference.
  \code
  PPoint2d p(1,2); // or PPoint2d p(1,2,1);
  \endcode */
template <typename T> struct PPoint2 : public cv::Point3_<T> {
  typedef cv::Point3_<T> Base;
  PPoint2(const Base &p) : Base(p) {}
  PPoint2(const cv::Vec<T, 3> &v) : Base(v) {
#ifdef UNITTEST
    std::cout << "pp2(vec) " << std::flush;
#endif
  }
  PPoint2(T x0, T x1, T x2=1) : Base(x0, x1, x2) {}
  PPoint2& operator=(const Base &p) {
    Base::operator=(p);
    return *this;
  }
  PPoint2& operator=(const cv::Vec<T, 3> &v) {
    Base::operator=(v);
    return *this;
  }
  T const& operator[](int i) const {
    return (&static_cast<const Base&>(*this).x)[i]; }
  T&       operator[](int i)       {
    return (&static_cast<      Base&>(*this).x)[i]; }
  //! x coordinate of image
  T x() const { return (*this)[0] / (*this)[2]; }
  //! y coordinate of image
  T y() const { return (*this)[1] / (*this)[2]; }
};

typedef PPoint2<uchar>  PPoint2b;
typedef PPoint2<short>  PPoint2s;
typedef PPoint2<ushort> PPoint2w;
typedef PPoint2<int>    PPoint2i;
typedef PPoint2<float>  PPoint2f;
typedef PPoint2<double> PPoint2d;

//! 3-dimensional projective point of 4D homogeneous coordinate.
/*! This class is almost same to cv::Scalar_, see OpenCV reference.
  \code
  PPoint3d p(1,2,3); // or PPoint3d p(1,2,3,1);
  \endcode */
template <typename T> struct PPoint3 : public cv::Scalar_<T> {
  typedef cv::Scalar_<T> Base;
  PPoint3(const Base &v) : Base(v) {}
  PPoint3(const cv::Vec<T, 4> &v) : Base(v) {
#ifdef UNITTEST
    std::cout << "pp3(vec) " << std::flush;
#endif
  }
  PPoint3(T x0, T x1, T x2, T x3=1) : Base(x0, x1, x2, x3) {}
  PPoint3& operator=(const Base &v) {
    Base::operator=(v);
    return *this;
  }
  PPoint3& operator=(const cv::Vec<T, 4> &v) {
    Base::operator=(v);
    return *this;
  }
  //! x coordinate of image
  T x() const { return (*this)[0] / (*this)[3]; }
  //! y coordinate of image
  T y() const { return (*this)[1] / (*this)[3]; }
  //! z coordinate of image
  T z() const { return (*this)[2] / (*this)[3]; }
};

typedef PPoint3<uchar>  PPoint3b;
typedef PPoint3<short>  PPoint3s;
typedef PPoint3<ushort> PPoint3w;
typedef PPoint3<int>    PPoint3i;
typedef PPoint3<float>  PPoint3f;
typedef PPoint3<double> PPoint3d;

template <typename T, int N> inline std::ostream&
operator<<(std::ostream& ost, const cv::Vec<T, N> &a) {
  return ost << cv::Mat(a, false); }
template <typename T, int N, int M> inline std::ostream&
operator<<(std::ostream& ost, const cv::Matx<T,N,M> &a) {
  return ost << cv::Mat(a, false); }
}      // namespace tutor

namespace cv {
template <typename _Tp, int m, int n> static inline
const Vec<_Tp, m> operator*(const Matx<_Tp, m, n> &a, const Matx<_Tp, n, 1> &b) {
  return Vec<_Tp, m>(Matx<_Tp, m, 1>(a, b, Matx_MatMulOp()), 1, Matx_ScaleOp());
}
}

//-----------------------------------------------------------------------
#ifdef UNITTEST
#undef NDEBUG
#include <cassert>
#include <iostream>
namespace {
const struct UNITTEST_MATRIX {
  UNITTEST_MATRIX() {
    using namespace std;
    using namespace tutor;
    try {
      {
      const cv::Matx33d m33(1,2,3,
                            4,5,6,
                            0,0,1);
      assert(m33(0,0) == 1 && m33(1,0) == 4);
      cout << "m33=" << m33 << '\n';
      cout << m33.t() << endl;
      PPoint2d b(1, 2);
      assert(b[0] == 1 && b[1] == 2 && b[2] == 1);
      assert(b.x() == 1 && b.y() == 2);
      cout << "b=" << b << endl;
      const PPoint2d c = m33 * b;
      cout << "c=" << c << '\n';
      b *= 2;
      assert(b == PPoint2d(2, 4, 2));
      cout << "b*2=" << b << endl;
      const PPoint2d p(2, 2, 2);
      assert(p.x() == 1);
      assert(p.y() == 1);
      const PPoint2d d = cv::Vec3d(1, 2, 3);
      cout << d.x() << " " << d.y() << endl;
      b = cv::Vec3f(3,4,5);
      cout << b << endl;
      cv::Vec3d v = m33 * cv::Vec3d(1,2,3);
    }
    cv::Matx44d m(1,2,3,4,
                  5,6,7,8,
                  9,10,11,12,
                  0,0,0,1);
    m(0,0) = -1;
    cout << m << endl;
    const PPoint3d q(2, 2, 2, 2);
	assert(q.x() == 1);
	assert(q.y() == 1);
	assert(q.z() == 1);
    PPoint3d a = m * q;
    PPoint3d r(cv::Vec4d(1,2,3,4));
    PPoint3d aa(cv::Scalar(1,2,3,4));
    r = cv::Scalar(1,2,3,4);
    aa += r;
    cout << aa << endl;
    } catch (const std::exception &e) {
      cerr << e.what() << endl;
      throw;
    } catch (...) {
      cerr << "fatal error!" << endl;
    }
  }
} Unittest_Matrix;
}
#endif // UNITTEST
#endif // PPOINT_HPP_
