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

#ifndef _QX_DATA_MEMBER_QOBJECT_H_
#define _QX_DATA_MEMBER_QOBJECT_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file QxDataMember_QObject.h
 * \author Lionel Marty
 * \ingroup QxDataMember
 * \brief Connect Qt introspection engine (based on QObject class, with QMetaObject type) to QxOrm library introspection engine
 */

#include <QtCore/qmetaobject.h>
#include <QtCore/qmetatype.h>

#include <qxorm/QxDataMember/IxDataMember.h>

#include <qxorm/QxSerialize/Qt/QxSerialize_QString.h>
#include <qxorm/QxSerialize/Qt/QxSerialize_QVariant.h>

#define QX_DATA_MEMBER_QOBJECT_IMPL_VIRTUAL_ARCHIVE_HPP(ArchiveInput, ArchiveOutput) \
virtual void toArchive(const void * pOwner, ArchiveOutput & ar) const; \
virtual void fromArchive(void * pOwner, ArchiveInput & ar);

namespace qx {

/*!
 * \ingroup QxDataMember
 * \brief qx::QxDataMember_QObject : connect Qt introspection engine (based on QObject class, with QMetaObject type) to QxOrm library introspection engine
 */
class QX_DLL_EXPORT QxDataMember_QObject : public IxDataMember
{

protected:

   const QMetaObject *  m_metaObject;    //!< Meta-object from introspection engine of Qt library (& MyQObject::staticMetaObject)
   QMetaProperty        m_metaProperty;  //!< Meta-property from introspection engine of Qt library

public:

   QxDataMember_QObject(const QMetaObject * pMetaObject, const QString & sKey);
   virtual ~QxDataMember_QObject() { ; }

   virtual bool isEqual(const void * pOwner1, const void * pOwner2) const;
   virtual QString toString(const void * pOwner, const QString & sFormat, int iIndexName = -1) const;
   virtual qx_bool fromString(void * pOwner, const QString & s, const QString & sFormat, int iIndexName = -1);
   virtual QVariant toVariant(const void * pOwner, const QString & sFormat, int iIndexName = -1) const;
   virtual qx_bool fromVariant(void * pOwner, const QVariant & v, const QString & sFormat, int iIndexName = -1);

public:

#if _QX_SERIALIZE_POLYMORPHIC
   QX_DATA_MEMBER_QOBJECT_IMPL_VIRTUAL_ARCHIVE_HPP(boost::archive::polymorphic_iarchive, boost::archive::polymorphic_oarchive)
#endif // _QX_SERIALIZE_POLYMORPHIC

#if _QX_SERIALIZE_BINARY
   QX_DATA_MEMBER_QOBJECT_IMPL_VIRTUAL_ARCHIVE_HPP(boost::archive::binary_iarchive, boost::archive::binary_oarchive)
#endif // _QX_SERIALIZE_BINARY

#if _QX_SERIALIZE_TEXT
   QX_DATA_MEMBER_QOBJECT_IMPL_VIRTUAL_ARCHIVE_HPP(boost::archive::text_iarchive, boost::archive::text_oarchive)
#endif // _QX_SERIALIZE_TEXT

#if _QX_SERIALIZE_XML
   QX_DATA_MEMBER_QOBJECT_IMPL_VIRTUAL_ARCHIVE_HPP(boost::archive::xml_iarchive, boost::archive::xml_oarchive)
#endif // _QX_SERIALIZE_XML

#if _QX_SERIALIZE_PORTABLE_BINARY
   QX_DATA_MEMBER_QOBJECT_IMPL_VIRTUAL_ARCHIVE_HPP(eos::portable_iarchive, eos::portable_oarchive)
#endif // _QX_SERIALIZE_PORTABLE_BINARY

#if _QX_SERIALIZE_WIDE_BINARY
   QX_DATA_MEMBER_QOBJECT_IMPL_VIRTUAL_ARCHIVE_HPP(boost::archive::binary_wiarchive, boost::archive::binary_woarchive)
#endif // _QX_SERIALIZE_WIDE_BINARY

#if _QX_SERIALIZE_WIDE_TEXT
   QX_DATA_MEMBER_QOBJECT_IMPL_VIRTUAL_ARCHIVE_HPP(boost::archive::text_wiarchive, boost::archive::text_woarchive)
#endif // _QX_SERIALIZE_WIDE_TEXT

#if _QX_SERIALIZE_WIDE_XML
   QX_DATA_MEMBER_QOBJECT_IMPL_VIRTUAL_ARCHIVE_HPP(boost::archive::xml_wiarchive, boost::archive::xml_woarchive)
#endif // _QX_SERIALIZE_WIDE_XML

protected:

   virtual boost::any getDataPtr(const void * pOwner) const;
   virtual boost::any getDataPtr(void * pOwner);
   virtual void * getDataVoidPtr(const void * pOwner) const;
   virtual void * getDataVoidPtr(void * pOwner);

};

} // namespace qx

#endif // _QX_DATA_MEMBER_QOBJECT_H_
