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

#ifndef _IX_BOOST_SERIALIZE_REGISTER_HELPER_H_
#define _IX_BOOST_SERIALIZE_REGISTER_HELPER_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/noncopyable.hpp>

#include <qxorm/QxCommon/QxConfig.h>

#include <qxorm/QxSerialize/boost/QxSerializeInclude.h>

#define QX_IX_BOOST_SERIALIZE_REGISTER_HELPER_PURE_VIRTUAL_ARCHIVE(ArchiveIn, ArchiveOut) \
virtual void helper(ArchiveIn & ar) const = 0; \
virtual void helper(ArchiveOut & ar) const = 0;

namespace qx {

class QX_DLL_EXPORT IxBoostSerializeRegisterHelper : private boost::noncopyable
{

private:

   QString m_sKey;   // Register key used by 'QxBoostSerializeRegisterHelperX'

protected:

   IxBoostSerializeRegisterHelper(const QString & sKey);
   virtual ~IxBoostSerializeRegisterHelper() = 0;

public:

   virtual void helper() const = 0;

#if _QX_SERIALIZE_POLYMORPHIC
   QX_IX_BOOST_SERIALIZE_REGISTER_HELPER_PURE_VIRTUAL_ARCHIVE(boost::archive::polymorphic_iarchive, boost::archive::polymorphic_oarchive)
#endif // _QX_SERIALIZE_POLYMORPHIC

#if _QX_SERIALIZE_BINARY
   QX_IX_BOOST_SERIALIZE_REGISTER_HELPER_PURE_VIRTUAL_ARCHIVE(boost::archive::binary_iarchive, boost::archive::binary_oarchive)
#endif // _QX_SERIALIZE_BINARY

#if _QX_SERIALIZE_TEXT
   QX_IX_BOOST_SERIALIZE_REGISTER_HELPER_PURE_VIRTUAL_ARCHIVE(boost::archive::text_iarchive, boost::archive::text_oarchive)
#endif // _QX_SERIALIZE_TEXT

#if _QX_SERIALIZE_XML
   QX_IX_BOOST_SERIALIZE_REGISTER_HELPER_PURE_VIRTUAL_ARCHIVE(boost::archive::xml_iarchive, boost::archive::xml_oarchive)
#endif // _QX_SERIALIZE_XML

#if _QX_SERIALIZE_PORTABLE_BINARY
   QX_IX_BOOST_SERIALIZE_REGISTER_HELPER_PURE_VIRTUAL_ARCHIVE(eos::portable_iarchive, eos::portable_oarchive)
#endif // _QX_SERIALIZE_PORTABLE_BINARY

#if _QX_SERIALIZE_WIDE_BINARY
   QX_IX_BOOST_SERIALIZE_REGISTER_HELPER_PURE_VIRTUAL_ARCHIVE(boost::archive::binary_wiarchive, boost::archive::binary_woarchive)
#endif // _QX_SERIALIZE_WIDE_BINARY

#if _QX_SERIALIZE_WIDE_TEXT
   QX_IX_BOOST_SERIALIZE_REGISTER_HELPER_PURE_VIRTUAL_ARCHIVE(boost::archive::text_wiarchive, boost::archive::text_woarchive)
#endif // _QX_SERIALIZE_WIDE_TEXT

#if _QX_SERIALIZE_WIDE_XML
   QX_IX_BOOST_SERIALIZE_REGISTER_HELPER_PURE_VIRTUAL_ARCHIVE(boost::archive::xml_wiarchive, boost::archive::xml_woarchive)
#endif // _QX_SERIALIZE_WIDE_XML

};

} // namespace qx

#endif // _IX_BOOST_SERIALIZE_REGISTER_HELPER_H_
