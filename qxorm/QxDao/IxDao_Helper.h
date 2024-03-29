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

#ifndef _IX_DAO_HELPER_H_
#define _IX_DAO_HELPER_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file IxDao_Helper.h
 * \author Lionel Marty
 * \ingroup QxDao
 * \brief Helper class to communicate with database
 */

#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlquery.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqldriver.h>

#include <qxorm/QxTraits/get_primary_key.h>
#include <qxorm/QxTraits/is_valid_primary_key.h>

#include <qxorm/QxDao/QxSqlDatabase.h>
#include <qxorm/QxDao/QxSqlQueryBuilder.h>
#include <qxorm/QxDao/QxSqlQueryHelper.h>
#include <qxorm/QxDao/QxSqlQuery.h>
#include <qxorm/QxDao/IxSqlRelation.h>
#include <qxorm/QxDao/QxSqlRelationLinked.h>

#include <qxorm/QxDao/QxSqlGenerator/IxSqlGenerator.h>

#include <qxorm/QxCollection/QxCollection.h>

#include <qxorm/QxDataMember/IxDataMemberX.h>

#include <qxorm/QxValidator/QxInvalidValueX.h>
#include <qxorm/QxValidator/QxValidatorError.h>

namespace qx {
template <class T>
QxInvalidValueX validate(T & t, const QString & group);
} // namespace qx

namespace qx {
namespace dao {
namespace detail {

/*!
 * \ingroup QxDao
 * \brief qx::dao::detail::IxDao_Helper : helper class to communicate with database
 */
class QX_DLL_EXPORT IxDao_Helper
{

protected:

   QTime          m_time;                 //!< Time (in ms) to execute query
   QSqlDatabase   m_database;             //!< Connection to database to execute query
   QSqlQuery      m_query;                //!< Query to execute
   QSqlError      m_error;                //!< Error executing query
   QString        m_context;              //!< Description of context : fetch, insert, update, delete, etc...
   long           m_lDataCount;           //!< Data member collection count
   bool           m_bTransaction;         //!< Transaction in progress : commit if valid, rollback if error
   bool           m_bQuiet;               //!< Display message and assert in debug mode
   bool           m_bTraceQuery;          //!< Trace sql query
   bool           m_bTraceRecord;         //!< Trace sql record
   bool           m_bCartesianProduct;    //!< Recordset can return cartesian product => same id in multiple records
   bool           m_bValidatorThrowable;  //!< An exception of type qx::validator_error is thrown when invalid values are detected inserting or updating an element into database
   QStringList    m_lstColumns;           //!< List of columns to execute sql query (if empty => all columns)

   qx::IxSqlQueryBuilder_ptr     m_pQueryBuilder;        //!< Sql query builder
   qx::IxDataMemberX *           m_pDataMemberX;         //!< Collection of data member
   qx::IxDataMember *            m_pDataId;              //!< Data member id
   qx::QxSqlQuery                m_qxQuery;              //!< Query sql with place-holder
   IxSqlGenerator *              m_pSqlGenerator;        //!< SQL generator to build SQL query specific for each database
   qx::QxInvalidValueX           m_lstInvalidValues;     //!< List of invalid values using validator engine
   qx::QxSqlRelationLinked_ptr   m_pSqlRelationLinked;   //!< List of relation linked to build a hierarchy of relationships

protected:

   IxDao_Helper();
   virtual ~IxDao_Helper();

public:

   bool isValid() const;
   bool hasFeature(QSqlDriver::DriverFeature ft) const;

   QSqlDatabase & database();
   const QSqlDatabase & database() const;
   QSqlQuery & query();
   const QSqlQuery & query() const;
   QSqlError & error();
   const QSqlError & error() const;
   qx::QxSqlQuery & qxQuery();
   const qx::QxSqlQuery & qxQuery() const;
   qx::IxSqlQueryBuilder & builder();
   const qx::IxSqlQueryBuilder & builder() const;
   qx::IxDataMemberX * getDataMemberX() const;
   long getDataCount() const;
   qx::IxDataMember * getDataId() const;
   qx::IxDataMember * nextData(long & l) const;
   QString sql() const;
   qx::QxSqlRelationLinked * getSqlRelationLinked() const;
   bool getCartesianProduct() const;
   QStringList getSqlColumns() const;
   void setSqlColumns(const QStringList & lst);
   IxSqlGenerator * getSqlGenerator() const;
   void addInvalidValues(const qx::QxInvalidValueX & lst);

   QSqlError errFailed();
   QSqlError errEmpty();
   QSqlError errNoData();
   QSqlError errInvalidId();
   QSqlError errInvalidRelation();

   bool transaction();
   bool nextRecord();
   void updateError(const QSqlError & error);
   void quiet();
   bool exec();

   bool updateSqlRelationX(const QStringList & relation);
   void dumpRecord() const;
   void addQuery(const qx::QxSqlQuery & query, bool bResolve);

   template <class U>
   inline bool isValidPrimaryKey(const U & u)
   {
       //qDebug() << "isValidPrimaryKey: data_id: " << m_pDataId << " trait: " << qx::trait::is_valid_primary_key(m_pDataId->toVariant(& u)) ;

       return (m_pDataId && qx::trait::is_valid_primary_key(m_pDataId->toVariant(& u)));
   }

   template <class U>
   inline void updateLastInsertId(U & u)
   {
      if (m_pDataId && m_pDataId->getAutoIncrement() && this->hasFeature(QSqlDriver::LastInsertId))
      { m_pDataId->fromVariant((& u), m_query.lastInsertId()); }
   }

   template <class U>
   inline bool validateInstance(U & u)
   {
      qx::QxInvalidValueX invalidValues = qx::validate(u, "");
      this->addInvalidValues(invalidValues);
      return (invalidValues.count() <= 0);
   }

protected:

   virtual void updateQueryBuilder();

   QSqlError updateError(const QString & sError);
   void init(QSqlDatabase * pDatabase, const QString & sContext);
   void terminate();

};

} // namespace detail
} // namespace dao
} // namespace qx

#endif // _IX_DAO_HELPER_H_
