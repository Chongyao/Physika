/*
 * @file quaternion.h 
 * @brief quaternion class, it is used for rotation operations.
 * @author Sheng Yang
 * 
 * This file is part of Physika, a versatile physics simulation library.
 * Copyright (C) 2013 Physika Group.
 *
 * This Source Code Form is subject to the terms of the GNU General Public License v2.0. 
 * If a copy of the GPL was not distributed with this file, you can obtain one at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 *
 */

#ifndef PHSYIKA_CORE_QUATERNION_QUATERNION_H_
#define PHSYIKA_CORE_QUATERNION_QUATERNION_H_

#include "Physika_Core/Utilities/global_config.h"
#include "Physika_Core/Utilities/physika_assert.h"
#include "Physika_Core/Utilities/type_utilities.h"
#include "Physika_Core/Vectors/vector_3d.h"
#include "Physika_Core/Matrices/matrix_3x3.h"
#include "Physika_Core/Matrices/matrix_MxN.h"


namespace Physika{

template <typename Scalar>
class Quaternion
{
public:    
    /* Constructors */
    Quaternion();
    Quaternion(Scalar , Scalar , Scalar , Scalar );
    Quaternion(const Vector<Scalar,3> &, Scalar );
    Quaternion(Scalar , const Vector<Scalar,3> &);
    Quaternion(const Scalar *); 
    Quaternion(const Quaternion<Scalar> &);
    Quaternion(const SquareMatrix<Scalar, 3> &);   //init from a 3x3matrix
    Quaternion(const MatrixMxN<Scalar> &);         //init from a 4x4matrix
    
    /* Assignment operators */
    Quaternion<Scalar> &operator = (const Quaternion<Scalar> &);
    Quaternion<Scalar> &operator += (const Quaternion<Scalar> &);
    Quaternion<Scalar> &operator -= (const Quaternion<Scalar> &);
    
    /* Get and Set functions */
    inline Scalar x() const { return x_;}
    inline Scalar y() const { return y_;}
    inline Scalar z() const { return z_;}
    inline Scalar w() const { return w_;}

    inline void setX(Scalar x) { x_ = x;}
    inline void setY(Scalar y) { y_ = y;}
    inline void setZ(Scalar z) { z_ = z;}
    inline void setW(Scalar w) { w_ = w;}

    /* Special functions */
    Scalar norm();
    Quaternion<Scalar>& normalize();
    void set(const Vector<Scalar,3>&, Scalar );
    void set(Scalar , const Vector<Scalar,3>& );
    Scalar getAngle() const;                                         // return the angle between this quat and the identity quaternion.
    Scalar getAngle(const Quaternion<Scalar>&) const;                // return the angle between this and the argument
    Scalar dot(const Quaternion<Scalar> &) const;
    Quaternion<Scalar> getConjugate() const;                         // return the conjugate
    const Vector<Scalar,3> rotate(const Vector<Scalar,3> ) const;    // rotates passed vec by this;
    SquareMatrix<Scalar, 3> get3x3Matrix() const;  //return 3x3matrix format
    MatrixMxN<Scalar> get4x4Matrix() const;        //return 4x4matrix with a identity transform.



    /* Operator overloading */
    Quaternion<Scalar> operator - (const Quaternion<Scalar> &);
    Quaternion<Scalar> operator - (void);
    Quaternion<Scalar> operator + (const Quaternion<Scalar> &);
    Quaternion<Scalar> operator * (Scalar );
    Quaternion<Scalar> operator / (Scalar );
    bool operator == (const Quaternion<Scalar> &);
    bool operator != (const Quaternion<Scalar> &);
    Scalar& operator[] (int);
    const Scalar& operator[] (int) const;

    static inline Quaternion<Scalar> identityQuaternion() { return Quaternion<Scalar>(0,0,0,1); }

protected:
    Scalar x_,y_,z_,w_;
protected:
    PHYSIKA_STATIC_ASSERT((is_same<Scalar,float>::value||is_same<Scalar,double>::value),
                           "Quaternion<Scalar> are only defined for Scalar type of float and double");
};


template <typename Scalar>
std::ostream& operator<< (std::ostream &s, const Quaternion<Scalar> &quat)
{
    s <<quat.x()<<", "<<quat.y()<<", "<<quat.z()<<", "<<quat.w()<<std::endl;
    return s; 
}

//make * operator commutative
template <typename S, typename T>
Quaternion<T> operator *(S scale, const Quaternion<T> &quad)
{
    return quad * scale;
}

//convenient typedefs
typedef Quaternion<float> Quaternionf;
typedef Quaternion<double> Quaterniond;

}//end of namespace Physika

#endif //PHSYIKA_CORE_QUATERNION_QUATERNION_H_
