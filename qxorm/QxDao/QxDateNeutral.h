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

#ifndef _QX_DATE_NEUTRAL_H_
#define _QX_DATE_NEUTRAL_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file QxDateNeutral.h
 * \author Lionel Marty
 * \ingroup QxDao
 * \brief Helper class to store a date value into database under neutral format (YYYYMMDD) => cross database compatibility
 */

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

#include <QtCore/qdatetime.h>

#include <qxorm/QxSerialize/Qt/QxSerialize_QString.h>

#include <qxorm/QxTraits/get_class_name.h>

namespace qx {

/*!
 * \ingroup QxDao
 * \brief qx::QxDateNeutral : helper class to store a date value into database under neutral format (YYYYMMDD) => cross database compatibility
 */
class QxDateNeutral
{

   friend class boost::serialization::access;

private:

   QDate m_date;        //!< Data value under QDate format from Qt library
   QString m_neutral;   //!< Data value under neutral format 'yyyyMMdd'

public:

   QxDateNeutral() { ; }
   explicit QxDateNeutral(const QDate & date) : m_date(date) { update(); }
   explicit QxDateNeutral(const QString & neutral) : m_neutral(neutral) { update(); }
   virtual ~QxDateNeutral() { ; }

   inline QDate toDate() const         { return m_date; }
   inline QString toNeutral() const    { return m_neutral; }
   inline bool isValid() const         { return m_date.isValid(); }

   inline void setDate(const QDate & date)            { m_neutral = ""; m_date = date; update(); }
   inline void setNeutral(const QString & neutral)    { m_date = QDate(); m_neutral = neutral; update(); }

   static QxDateNeutral fromDate(const QDate & date)           { return QxDateNeutral(date); }
   static QxDateNeutral fromNeutral(const QString & neutral)   { return QxDateNeutral(neutral); }

private:

   static inline const char * format() { return "yyyyMMdd"; }

   void update()
   {
      if (m_neutral.isEmpty() && ! m_date.isValid()) { return; }
      else if (m_date.isValid()) { m_neutral = m_date.toString(format()); }
      else { qAssert(m_neutral.size() == QString(format()).size()); m_date = QDate::fromString(m_neutral, format()); qAssert(m_date.isValid()); }
   }

   template <class Archive>
   void serialize(Archive & ar, const unsigned int file_version)
   {
      Q_UNUSED(file_version);
      ar & boost::serialization::make_nvp("date_neutral", m_neutral);
      if (Archive::is_loading::value) { m_date = QDate(); update(); }
   }

};

} // namespace qx

QX_REGISTER_CLASS_NAME(qx::QxDateNeutral)

#endif // _QX_DATE_NEUTRAL_H_
