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

#ifndef _QX_TIME_NEUTRAL_H_
#define _QX_TIME_NEUTRAL_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file QxTimeNeutral.h
 * \author Lionel Marty
 * \ingroup QxDao
 * \brief Helper class to store a time value into database under neutral format (HHMMSS) => cross database compatibility
 */

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

#include <QtCore/qdatetime.h>

#include <qxorm/QxSerialize/Qt/QxSerialize_QString.h>

#include <qxorm/QxTraits/get_class_name.h>

namespace qx {

/*!
 * \ingroup QxDao
 * \brief qx::QxTimeNeutral : helper class to store a time value into database under neutral format (HHMMSS) => cross database compatibility
 */
class QxTimeNeutral
{

   friend class boost::serialization::access;

private:

   QTime m_time;        //!< Data value under QTime format from Qt library
   QString m_neutral;   //!< Data value under neutral format 'hhmmss'

public:

   QxTimeNeutral() { ; }
   explicit QxTimeNeutral(const QTime & time) : m_time(time) { update(); }
   explicit QxTimeNeutral(const QString & neutral) : m_neutral(neutral) { update(); }
   virtual ~QxTimeNeutral() { ; }

   inline QTime toTime() const         { return m_time; }
   inline QString toNeutral() const    { return m_neutral; }
   inline bool isValid() const         { return m_time.isValid(); }

   inline void setTime(const QTime & time)            { m_neutral = ""; m_time = time; update(); }
   inline void setNeutral(const QString & neutral)    { m_time = QTime(); m_neutral = neutral; update(); }

   static QxTimeNeutral fromTime(const QTime & time)           { return QxTimeNeutral(time); }
   static QxTimeNeutral fromNeutral(const QString & neutral)   { return QxTimeNeutral(neutral); }

private:

   static inline const char * format() { return "hhmmss"; }

   void update()
   {
      if (m_neutral.isEmpty() && ! m_time.isValid()) { return; }
      else if (m_time.isValid()) { m_neutral = m_time.toString(format()); }
      else { qAssert(m_neutral.size() == QString(format()).size()); m_time = QTime::fromString(m_neutral, format()); qAssert(m_time.isValid()); }
   }

   template <class Archive>
   void serialize(Archive & ar, const unsigned int file_version)
   {
      Q_UNUSED(file_version);
      ar & boost::serialization::make_nvp("time_neutral", m_neutral);
      if (Archive::is_loading::value) { m_time = QTime(); update(); }
   }

};

} // namespace qx

QX_REGISTER_CLASS_NAME(qx::QxTimeNeutral)

#endif // _QX_TIME_NEUTRAL_H_
