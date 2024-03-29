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

#ifndef _QX_SQL_RELATION_LINKED_H_
#define _QX_SQL_RELATION_LINKED_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file QxSqlRelationLinked.h
 * \author Lionel Marty
 * \ingroup QxDao
 * \brief Hierarchy of relationships to build SQL query
 */

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>

#include <QtSql/qsqlerror.h>

#include <qxorm/QxCommon/QxBool.h>

#include <qxorm/QxDao/IxSqlRelation.h>

#include <qxorm/QxCollection/QxCollection.h>

namespace qx {

/*!
 * \ingroup QxDao
 * \brief qx::QxSqlRelationLinked : hierarchy of relationships to build SQL query
 *
 * Here is the structure, each real relation has a relation linked associated to build the hierarchy, like this :
 * \code
(<root>, <relation_linked>)
   ("blog", blog_relation)
   ("blog", <relation_linked>)
      ("author", author_relation)
      ("author", <relation_linked>)
         ("list_blog", list_blog_relation)
         ("list_blog", <relation_linked>)
            (etc...)
      ("comment", comment_relation)
      ("comment", <relation_linked>)
         ("blog_id", blog_id_relation)
         ("blog_id", <relation_linked>)
            (etc...)
      ("category", category_relation)
      ("category", <relation_linked>)
         ("list_blog", list_blog_relation)
         ("list_blog", <relation_linked>)
            (etc...)
 * \endcode
 */
class QX_DLL_EXPORT QxSqlRelationLinked
{

protected:

   typedef boost::shared_ptr<QxSqlRelationLinked> type_ptr;
   typedef boost::tuple<qx::dao::sql_join::join_type, IxSqlRelation *> type_relation;
   typedef qx::QxCollection<QString, type_relation> type_lst_relation;
   typedef QHash<QString, type_ptr> type_lst_relation_linked;

   type_lst_relation m_relationX;               //!< List of relationships for current level
   type_lst_relation_linked m_relationLinkedX;  //!< List of child to build the hierarchy
   IxSqlRelationX * m_allRelationX;             //!< List of all relationships per level
   bool m_bRoot;                                //!< Root of the hierarchy

public:

   QxSqlRelationLinked();
   QxSqlRelationLinked(bool bRoot);
   virtual ~QxSqlRelationLinked();

   qx_bool buildHierarchy(IxSqlRelationX * pRelationX, const QStringList & sRelationX);

   void hierarchySelect(QxSqlRelationParams & params);
   void hierarchyFrom(QxSqlRelationParams & params);
   void hierarchyJoin(QxSqlRelationParams & params);
   void hierarchyWhereSoftDelete(QxSqlRelationParams & params);
   void hierarchyResolveOutput(QxSqlRelationParams & params);
   QSqlError hierarchyOnBeforeSave(QxSqlRelationParams & params);
   QSqlError hierarchyOnAfterSave(QxSqlRelationParams & params);

   bool getCartesianProduct() const;
   long getAllRelationCount() const;
   long getRelationCount() const;

protected:

   qx_bool insertRelationToHierarchy(const QStringList & sRelationX, const QString & sKey, qx::dao::sql_join::join_type eJoinType);

};

typedef boost::shared_ptr<QxSqlRelationLinked> QxSqlRelationLinked_ptr;

} // namespace qx

#endif // _QX_SQL_RELATION_LINKED_H_
