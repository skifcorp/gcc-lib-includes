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

#ifndef _QX_CLONE_H_
#define _QX_CLONE_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file QxClone.h
 * \author Lionel Marty
 * \ingroup QxSerialize
 * \brief Clone all classes registered into QxOrm context using QxOrm library serialization engine
 */

#include <string>
#include <iostream>
#include <sstream>
#include <exception>

#include <boost/archive/archive_exception.hpp>

#include <qxorm/QxSerialize/boost/QxSerializeInclude.h>
#include <qxorm/QxSerialize/QxBoostSerializeHelper/QxBoostSerializeRegisterHelperX.h>
#include <qxorm/QxSerialize/QxSerializeInvoker.h>

#define QX_STR_CLONE_SERIALIZATION_ERROR "[QxOrm] qx::clone() serialization error : '%s'"
#define QX_STR_CLONE_DESERIALIZATION_ERROR "[QxOrm] qx::clone() deserialization error : '%s'"

namespace qx {

/*!
 * \ingroup QxSerialize
 * \brief qx::clone_to_nude_ptr(const T & obj) : return a nude pointer (be careful with memory leak) of a new instance of type T cloned from obj
 */
template <class T>
T * clone_to_nude_ptr(const T & obj)
{
   QX_CLONE_STRING_STREAM ioss(std::ios_base::binary | std::ios_base::in | std::ios_base::out);
   QX_CLONE_BINARY_OUTPUT_ARCHIVE oar(ioss, boost::archive::no_header);
   QxBoostSerializeRegisterHelperX::helper(oar);
   bool bSerializeOk = false;

   try { oar << obj; bSerializeOk = ioss.good(); }
   catch (const boost::archive::archive_exception & e) { qDebug(QX_STR_CLONE_SERIALIZATION_ERROR, e.what()); }
   catch (const std::exception & e) { qDebug(QX_STR_CLONE_SERIALIZATION_ERROR, e.what()); }
   catch (...) { qDebug(QX_STR_CLONE_SERIALIZATION_ERROR, "unknown error"); }
   if (! bSerializeOk) { qAssert(false); return NULL; }

   T * pClone = new T();
   QX_CLONE_BINARY_INPUT_ARCHIVE iar(ioss, boost::archive::no_header);
   QxBoostSerializeRegisterHelperX::helper(iar);
   bool bDeserializeOk = false;

   try { iar >> (* pClone); bDeserializeOk = ioss.good(); }
   catch (const boost::archive::archive_exception & e) { qDebug(QX_STR_CLONE_DESERIALIZATION_ERROR, e.what()); }
   catch (const std::exception & e) { qDebug(QX_STR_CLONE_DESERIALIZATION_ERROR, e.what()); }
   catch (...) { qDebug(QX_STR_CLONE_DESERIALIZATION_ERROR, "unknown error"); }
   qAssert(bDeserializeOk);

   return (bDeserializeOk ? pClone : NULL);
}

/*!
 * \ingroup QxSerialize
 * \brief qx::clone(const T & obj) : return a boost smart-pointer (boost::shared_ptr<T>) of a new instance of type T cloned from obj
 */
template <class T>
boost::shared_ptr<T> clone(const T & obj)
{ T * ptr = qx::clone_to_nude_ptr<T>(obj); return boost::shared_ptr<T>(ptr); }

/*!
 * \ingroup QxSerialize
 * \brief qx::clone_to_qt_shared_ptr(const T & obj) : return a Qt smart-pointer (QSharedPointer<T>) of a new instance of type T cloned from obj
 */
template <class T>
QSharedPointer<T> clone_to_qt_shared_ptr(const T & obj)
{ T * ptr = qx::clone_to_nude_ptr<T>(obj); return QSharedPointer<T>(ptr); }

} // namespace qx

#endif // _QX_CLONE_H_
