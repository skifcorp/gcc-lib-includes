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

#ifndef _QX_DATE_TIME_NEUTRAL_H_
#define _QX_DATE_TIME_NEUTRAL_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file QxDateTimeNeutral.h
 * \author Lionel Marty
 * \ingroup QxDao
 * \brief Helper class to store a date-time value into database under neutral format (YYYYMMDDHHMMSS) => cross database compatibility
 */

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

#include <QtCore/qdatetime.h>

#include <qxorm/QxSerialize/Qt/QxSerialize_QString.h>

#include <qxorm/QxTraits/get_class_name.h>

namespace qx {

/*!
 * \ingroup QxDao
 * \brief qx::QxDateTimeNeutral : helper class to store a date-time value into database under neutral format (YYYYMMDDHHMMSS) => cross database compatibility
 */
class QxDateTimeNeutral
{

   friend class boost::serialization::access;

private:

   QDateTime m_dt;      //!< Data value under QDateTime format from Qt library
   QString m_neutral;   //!< Data value under neutral format 'yyyyMMddhhmmss'

public:

   QxDateTimeNeutral() { ; }
   explicit QxDateTimeNeutral(const QDateTime & dt) : m_dt(dt) { update(); }
   explicit QxDateTimeNeutral(const QString & neutral) : m_neutral(neutral) { update(); }
   virtual ~QxDateTimeNeutral() { ; }

   inline QDateTime toDateTime() const    { return m_dt; }
   inline QString toNeutral() const       { return m_neutral; }
   inline bool isValid() const            { return m_dt.isValid(); }

   inline void setDateTime(const QDateTime & dt)      { m_neutral = ""; m_dt = dt; update(); }
   inline void setNeutral(const QString & neutral)    { m_dt = QDateTime(); m_neutral = neutral; update(); }

   static QxDateTimeNeutral fromDateTime(const QDateTime & dt)    { return QxDateTimeNeutral(dt); }
   static QxDateTimeNeutral fromNeutral(const QString & neutral)  { return QxDateTimeNeutral(neutral); }

private:

   static inline const char * format() { return "yyyyMMddhhmmss"; }

   void update()
   {
      if (m_neutral.isEmpty() && ! m_dt.isValid()) { return; }
      else if (m_dt.isValid()) { m_neutral = m_dt.toString(format()); }
      else { qAssert(m_neutral.size() == QString(format()).size()); m_dt = QDateTime::fromString(m_neutral, format()); qAssert(m_dt.isValid()); }
   }

   template <class Archive>
   void serialize(Archive & ar, const unsigned int file_version)
   {
      Q_UNUSED(file_version);
      ar & boost::serialization::make_nvp("dt_neutral", m_neutral);
      if (Archive::is_loading::value) { m_dt = QDateTime(); update(); }
   }

};

} // namespace qx

QX_REGISTER_CLASS_NAME(qx::QxDateTimeNeutral)

#endif // _QX_DATE_TIME_NEUTRAL_H_
