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

#ifndef _QX_TRAIT_IS_EQUAL_H_
#define _QX_TRAIT_IS_EQUAL_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file is_equal.h
 * \author Lionel Marty
 * \ingroup QxTraits
 * \brief qx::trait::has_operator_equal_equal<T>::value : return true if T provides operator==() function, T must be registered with QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(T) macro
 */

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qvariant.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtCore/qregexp.h>
#include <QtCore/qsize.h>
#include <QtCore/qurl.h>
#include <QtCore/quuid.h>
#include <QtCore/qsharedpointer.h>

#if _QX_ENABLE_QT_GUI_DEPENDENCY
#include <QtGui/qcolor.h>
#include <QtGui/qfont.h>
#include <QtGui/qimage.h>
#include <QtGui/qbrush.h>
#include <QtGui/qmatrix.h>
#include <QtGui/qregion.h>
#endif // _QX_ENABLE_QT_GUI_DEPENDENCY

#include <qxorm/QxDao/QxDaoPointer.h>

namespace qx {
namespace trait {

/*!
 * \ingroup QxTraits
 * \brief qx::trait::has_operator_equal_equal<T>::value : return true if T provides operator==() function, T must be registered with QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(T) macro
 */
template <typename T>
struct has_operator_equal_equal
{ enum { value = boost::is_pointer<T>::value }; };

} // namespace trait
} // namespace qx

#define QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(className) \
namespace qx { namespace trait { \
template <> \
struct has_operator_equal_equal< className > { enum { value = true }; }; \
} } // namespace qx::trait

#define QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL_TEMPLATE_1(className) \
namespace qx { namespace trait { \
template <typename T> \
struct has_operator_equal_equal< className<T> > { enum { value = true }; }; \
} } // namespace qx::trait

QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(bool)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(int)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(short)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(long)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(float)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(double)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(long double)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(char)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(unsigned int)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(unsigned short)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(unsigned long)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(unsigned char)

QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(std::string)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(std::wstring)

QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QString)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QByteArray)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QDate)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QDateTime)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QPoint)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QRect)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QRegExp)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QSize)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QTime)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QUrl)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QVariant)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QUuid)

#if _QX_ENABLE_QT_GUI_DEPENDENCY
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QColor)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QFont)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QImage)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QBrush)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QMatrix)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL(QRegion)
#endif // _QX_ENABLE_QT_GUI_DEPENDENCY

QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL_TEMPLATE_1(boost::shared_ptr)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL_TEMPLATE_1(QSharedPointer)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL_TEMPLATE_1(QWeakPointer)
QX_TYPE_HAS_OPERATOR_EQUAL_EQUAL_TEMPLATE_1(qx::dao::ptr)

#endif // _QX_TRAIT_IS_EQUAL_H_
