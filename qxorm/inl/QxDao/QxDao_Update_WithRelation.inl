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

namespace qx {
namespace dao {
namespace detail {

template <class T>
struct QxDao_Update_WithRelation_Generic
{

   static QSqlError update(const QStringList & relation, const qx::QxSqlQuery & query, T & t, QSqlDatabase * pDatabase)
   {
      qx::dao::detail::QxDao_Helper<T> dao(t, pDatabase, "update with relation");
      if (! dao.isValid()) { return dao.error(); }
      if (! dao.isValidPrimaryKey(t)) { return dao.errInvalidId(); }
      if (! dao.updateSqlRelationX(relation)) { return dao.errInvalidRelation(); }
      if (! pDatabase) { dao.transaction(); }
      dao.quiet();

      qx::QxSqlRelationParams params(0, 0, NULL, (& dao.builder()), (& dao.query()), (& t));
      params.setDatabase((& dao.database()));

      qx::QxSqlRelationLinked * pRelationLinked = dao.getSqlRelationLinked();
      if (pRelationLinked) { dao.updateError(pRelationLinked->hierarchyOnBeforeSave(params)); }
      if (! dao.isValid()) { return dao.error(); }

      dao.updateError(qx::dao::update_by_query(query, t, (& dao.database())));
      if (! dao.isValid()) { return dao.error(); }

      if (pRelationLinked) { dao.updateError(pRelationLinked->hierarchyOnAfterSave(params)); }
      if (! dao.isValid()) { return dao.error(); }

      return dao.error();
   }

};

template <class T>
struct QxDao_Update_WithRelation_Container
{

   static QSqlError update(const QStringList & relation, const qx::QxSqlQuery & query, T & t, QSqlDatabase * pDatabase)
   {
      if (qx::trait::generic_container<T>::size(t) <= 0) { return QSqlError(); }
      qx::dao::detail::QxDao_Helper_Container<T> dao(t, pDatabase, "update with relation");
      if (! dao.isValid()) { return dao.error(); }
      if (! dao.updateSqlRelationX(relation)) { return dao.errInvalidRelation(); }
      if (! pDatabase) { dao.transaction(); }
      dao.quiet();

      for (typename T::iterator it = t.begin(); it != t.end(); ++it)
      { if (! updateItem(query, (* it), dao)) { return dao.error(); } }

      return dao.error();
   }

private:

   template <typename U>
   static inline bool updateItem(const qx::QxSqlQuery & query, U & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
   {
      bool bUpdateOk = updateItem_Helper<U, boost::is_pointer<U>::value || qx::trait::is_smart_ptr<U>::value>::update(query, item, dao);
      if (bUpdateOk) { qx::dao::detail::QxDao_Keep_Original<U>::backup(item); }
      return bUpdateOk;
   }

   template <typename U, bool bIsPointer /* = true */>
   struct updateItem_Helper
   {
      static inline bool update(const qx::QxSqlQuery & query, U & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      { return (item ? qx::dao::detail::QxDao_Update_WithRelation_Container<T>::updateItem(query, (* item), dao) : true); }
   };

   template <typename U1, typename U2>
   struct updateItem_Helper<std::pair<U1, U2>, false>
   {
      static inline bool update(const qx::QxSqlQuery & query, std::pair<U1, U2> & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      { return qx::dao::detail::QxDao_Update_WithRelation_Container<T>::updateItem(query, item.second, dao); }
   };

   template <typename U1, typename U2>
   struct updateItem_Helper<const std::pair<U1, U2>, false>
   {
      static inline bool update(const qx::QxSqlQuery & query, const std::pair<U1, U2> & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      { return qx::dao::detail::QxDao_Update_WithRelation_Container<T>::updateItem(query, item.second, dao); }
   };

   template <typename U1, typename U2>
   struct updateItem_Helper<QPair<U1, U2>, false>
   {
      static inline bool update(const qx::QxSqlQuery & query, QPair<U1, U2> & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      { return qx::dao::detail::QxDao_Update_WithRelation_Container<T>::updateItem(query, item.second, dao); }
   };

   template <typename U1, typename U2>
   struct updateItem_Helper<const QPair<U1, U2>, false>
   {
      static inline bool update(const qx::QxSqlQuery & query, const QPair<U1, U2> & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      { return qx::dao::detail::QxDao_Update_WithRelation_Container<T>::updateItem(query, item.second, dao); }
   };

   template <typename U>
   struct updateItem_Helper<U, false>
   {
      static bool update(const qx::QxSqlQuery & query, U & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      {
         qx::QxSqlRelationParams params(0, 0, NULL, (& dao.builder()), (& dao.query()), (& item));
         params.setDatabase((& dao.database()));

         qx::QxSqlRelationLinked * pRelationLinked = dao.getSqlRelationLinked();
         if (pRelationLinked) { dao.updateError(pRelationLinked->hierarchyOnBeforeSave(params)); }
         if (! dao.isValid()) { return false; }

         dao.updateError(qx::dao::update_by_query(query, item, (& dao.database())));
         if (! dao.isValid()) { return false; }

         if (pRelationLinked) { dao.updateError(pRelationLinked->hierarchyOnAfterSave(params)); }
         if (! dao.isValid()) { return false; }

         return dao.isValid();
      }
   };

};

template <class T>
struct QxDao_Update_WithRelation_Ptr
{

   static inline QSqlError update(const QStringList & relation, const qx::QxSqlQuery & query, T & t, QSqlDatabase * pDatabase)
   { return (t ? qx::dao::update_by_query_with_relation(relation, query, (* t), pDatabase) : QSqlError()); }

};

template <class T>
struct QxDao_Update_WithRelation
{

   static inline QSqlError update(const QString & relation, const qx::QxSqlQuery & query, T & t, QSqlDatabase * pDatabase)
   {
      QStringList lst;
      if (! relation.isEmpty()) { lst = relation.split("|"); }
      return QxDao_Update_WithRelation<T>::update(lst, query, t, pDatabase);
   }

   static inline QSqlError update(const QStringList & relation, const qx::QxSqlQuery & query, T & t, QSqlDatabase * pDatabase)
   {
      typedef typename boost::mpl::if_c< boost::is_pointer<T>::value, qx::dao::detail::QxDao_Update_WithRelation_Ptr<T>, qx::dao::detail::QxDao_Update_WithRelation_Generic<T> >::type type_dao_1;
      typedef typename boost::mpl::if_c< qx::trait::is_smart_ptr<T>::value, qx::dao::detail::QxDao_Update_WithRelation_Ptr<T>, type_dao_1 >::type type_dao_2;
      typedef typename boost::mpl::if_c< qx::trait::is_container<T>::value, qx::dao::detail::QxDao_Update_WithRelation_Container<T>, type_dao_2 >::type type_dao_3;

      QSqlError error = type_dao_3::update(relation, query, t, pDatabase);
      if (! error.isValid()) { qx::dao::detail::QxDao_Keep_Original<T>::backup(t); }
      return error;
   }

};

} // namespace detail
} // namespace dao
} // namespace qx
