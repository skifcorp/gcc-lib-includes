/****************************************************************************
**
** http://www.qxorm.com/
** http://sourceforge.net/projects/qxorm/
** Original file by Lionel Marty
**
** This file is part of the QxOrm library
**
** This software is provided 'as-is', without any express or implied
** warranty. In no event will the authors be held liable for any
** damages arising from the use of this software.
**
** GNU Lesser General Public License Usage
** This file must be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file 'license.lgpl.txt' included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact :
** contact@qxorm.com
**
****************************************************************************/

#ifndef _QX_GET_BASE_CLASS_H_
#define _QX_GET_BASE_CLASS_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file get_base_class.h
 * \author Lionel Marty
 * \ingroup QxTraits
 * \brief qx::trait::get_base_class<T>::type : retrieve base class of type T registered into QxOrm context and return qx::trait::no_base_class_defined if no base class defined
 */

#include <boost/mpl/if.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/type_traits/is_same.hpp>

#include <qxorm/QxTraits/get_class_name.h>

namespace qx {
namespace trait {

class no_base_class_defined
{ public: no_base_class_defined() { ; }; virtual ~no_base_class_defined() { ; }; virtual void dummy() = 0; };

/*!
 * \ingroup QxTraits
 * \brief qx::trait::get_base_class<T>::type : retrieve base class of type T registered into QxOrm context and return qx::trait::no_base_class_defined if no base class defined
 */
template <class T>
class get_base_class
{ public: typedef qx::trait::no_base_class_defined type; };

template <class T>
class is_base_class_defined
{ public: enum { value = (boost::is_same<typename qx::trait::get_base_class<T>::type, qx::trait::no_base_class_defined>::value ? 0 : 1) }; };

template <class T>
class get_base_class_2
{
private: typedef typename qx::trait::get_base_class<T>::type type_base;
private: enum { is_base_ok = (boost::is_same<type_base, qx::trait::no_base_class_defined>::value ? 0 : 1) };
public: typedef typename boost::mpl::if_c<is_base_ok, type_base, T>::type type;
};

} // namespace trait
} // namespace qx

QX_REGISTER_CLASS_NAME(qx::trait::no_base_class_defined)

#define QX_REGISTER_BASE_CLASS(derivedClass, baseClass) \
namespace qx { namespace trait { \
template <> \
class get_base_class< derivedClass > \
{ public: typedef baseClass type; }; \
} } // namespace qx::trait

#define QX_GET_BASE_CLASS(T)                       qx::trait::get_base_class< T >::type
#define QX_GET_BASE_CLASS_WITH_TYPENAME(T)         qx::trait::get_base_class< typename T >::type
#define QX_IS_BASE_CLASS_DEFINED(T)                qx::trait::is_base_class_defined< T >::value
#define QX_IS_BASE_CLASS_DEFINED_WITH_TYPENAME(T)  qx::trait::is_base_class_defined< typename T >::value
#define QX_GET_BASE_CLASS_2(T)                     qx::trait::get_base_class_2< T >::type
#define QX_GET_BASE_CLASS_2_WITH_TYPENAME(T)       qx::trait::get_base_class_2< typename T >::type

#endif // _QX_GET_BASE_CLASS_H_
