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

#ifndef _QX_IS_SMART_PTR_TO_POD_H_
#define _QX_IS_SMART_PTR_TO_POD_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file is_smart_ptr_to_pod.h
 * \author Lionel Marty
 * \ingroup QxTraits
 * \brief qx::trait::is_smart_ptr_to_pod<T>::value : return true if T is a smart-pointer of boost, Qt or QxOrm libraries and (*T) is a POD type (char, int, long, etc.), otherwise return false
 */

#include <boost/mpl/if.hpp>
#include <boost/mpl/logical.hpp>

#include <qxorm/QxTraits/is_smart_ptr.h>
#include <qxorm/QxTraits/is_qx_pod.h>

namespace qx {
namespace trait {

/*!
 * \ingroup QxTraits
 * \brief qx::trait::is_smart_ptr_to_pod<T>::value : return true if T is a smart-pointer of boost, Qt or QxOrm libraries and (*T) is a POD type (char, int, long, etc.), otherwise return false
 */
template <typename T>
class is_smart_ptr_to_pod
{

private:

   template <typename U>
   static typename boost::mpl::if_c<qx::trait::is_qx_pod<U>::value, char, int>::type removeSmartPtr(const boost::scoped_ptr<U> &);

   template <typename U>
   static typename boost::mpl::if_c<qx::trait::is_qx_pod<U>::value, char, int>::type removeSmartPtr(const boost::shared_ptr<U> &);

   template <typename U>
   static typename boost::mpl::if_c<qx::trait::is_qx_pod<U>::value, char, int>::type removeSmartPtr(const boost::weak_ptr<U> &);

   template <typename U>
   static typename boost::mpl::if_c<qx::trait::is_qx_pod<U>::value, char, int>::type removeSmartPtr(const boost::intrusive_ptr<U> &);

   template <typename U>
   static typename boost::mpl::if_c<qx::trait::is_qx_pod<U>::value, char, int>::type removeSmartPtr(const QSharedDataPointer<U> &);

   template <typename U>
   static typename boost::mpl::if_c<qx::trait::is_qx_pod<U>::value, char, int>::type removeSmartPtr(const QSharedPointer<U> &);

   template <typename U>
   static typename boost::mpl::if_c<qx::trait::is_qx_pod<U>::value, char, int>::type removeSmartPtr(const QWeakPointer<U> &);

   template <typename U>
   static typename boost::mpl::if_c<qx::trait::is_qx_pod<U>::value, char, int>::type removeSmartPtr(const qx::dao::ptr<U> &);

   static int removeSmartPtr(...);
   static T t;

public:

   enum { value = (qx::trait::is_smart_ptr<T>::value && (sizeof(qx::trait::is_smart_ptr_to_pod<T>::removeSmartPtr(t)) == sizeof(char))) };

   typedef typename boost::mpl::if_c<qx::trait::is_smart_ptr_to_pod<T>::value, boost::mpl::true_, boost::mpl::false_>::type type;

};

} // namespace trait
} // namespace qx

#endif // _QX_IS_SMART_PTR_TO_POD_H_
