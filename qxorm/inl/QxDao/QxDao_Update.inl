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
struct QxDao_Update_Generic
{

   static QSqlError update(const qx::QxSqlQuery & query, T & t,
        QSqlDatabase * pDatabase, const QStringList & columns, const QString& table_name)
   {
      qx::dao::detail::QxDao_Helper<T> dao(t, pDatabase, "update", table_name);
      if (! dao.isValid()) { return dao.error(); }
      if (! dao.isValidPrimaryKey(t)) { return dao.errInvalidId(); }
      if (! dao.validateInstance(t)) { return dao.error(); }

      QString sql = dao.builder().update(columns).getSqlQuery();
      if (! dao.getDataId() || sql.isEmpty()) { return dao.errEmpty(); }
      if (! query.isEmpty()) { dao.addQuery(query, false); sql = dao.builder().getSqlQuery(); }
      if (! pDatabase) { dao.transaction(); }
      dao.query().prepare(sql);

      IxSqlGenerator * pSqlGenerator = dao.getSqlGenerator();
      if (pSqlGenerator) { pSqlGenerator->onBeforeUpdate((& dao), (& t)); }
      qx::dao::on_before_update<T>((& t), (& dao));
      qx::dao::detail::QxSqlQueryHelper_Update<T>::resolveInput(t, dao.query(), dao.builder(), columns);

      //qDebug () <<

      if (! query.isEmpty()) { query.resolve(dao.query()); }

     //

      if (! dao.query().exec()) {
        //qDebug () <<" ############################ " << dao.query().executedQuery();
        return dao.errFailed();
      }
      qx::dao::on_after_update<T>((& t), (& dao));
      if (pSqlGenerator) { pSqlGenerator->onAfterUpdate((& dao), (& t)); }

      return dao.error();
   }

};

template <class T>
struct QxDao_Update_Container
{

   static QSqlError update(const qx::QxSqlQuery & query, T & t,
               QSqlDatabase * pDatabase, const QStringList & columns, const QString& table_name)
   {
      if (qx::trait::generic_container<T>::size(t) <= 0) { return QSqlError(); }
      qx::dao::detail::QxDao_Helper_Container<T> dao(t, pDatabase, "update", table_name);
      if (! dao.isValid()) { return dao.error(); }
      if (! dao.validateInstance(t)) { return dao.error(); }

      QString sql = dao.builder().update(columns).getSqlQuery();
      if (! dao.getDataId() || sql.isEmpty()) { return dao.errEmpty(); }
      if (! query.isEmpty()) { dao.addQuery(query, false); sql = dao.builder().getSqlQuery(); }
      if (! pDatabase) { dao.transaction(); }
      dao.setSqlColumns(columns);
      dao.query().prepare(sql);

      for (typename T::iterator it = t.begin(); it != t.end(); ++it)
      { if (! updateItem((* it), dao)) { return dao.error(); } }

      return dao.error();
   }

private:

   template <typename U>
   static inline bool updateItem(U & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
   {
      bool bUpdateOk = updateItem_Helper<U, boost::is_pointer<U>::value || qx::trait::is_smart_ptr<U>::value>::update(item, dao);
      if (bUpdateOk) { qx::dao::detail::QxDao_Keep_Original<U>::backup(item); }
      return bUpdateOk;
   }

   template <typename U, bool bIsPointer /* = true */>
   struct updateItem_Helper
   {
      static inline bool update(U & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      { return (item ? qx::dao::detail::QxDao_Update_Container<T>::updateItem((* item), dao) : true); }
   };

   template <typename U1, typename U2>
   struct updateItem_Helper<std::pair<U1, U2>, false>
   {
      static inline bool update(std::pair<U1, U2> & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      { return qx::dao::detail::QxDao_Update_Container<T>::updateItem(item.second, dao); }
   };

   template <typename U1, typename U2>
   struct updateItem_Helper<const std::pair<U1, U2>, false>
   {
      static inline bool update(const std::pair<U1, U2> & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      { return qx::dao::detail::QxDao_Update_Container<T>::updateItem(item.second, dao); }
   };

   template <typename U1, typename U2>
   struct updateItem_Helper<QPair<U1, U2>, false>
   {
      static inline bool update(QPair<U1, U2> & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      { return qx::dao::detail::QxDao_Update_Container<T>::updateItem(item.second, dao); }
   };

   template <typename U1, typename U2>
   struct updateItem_Helper<const QPair<U1, U2>, false>
   {
      static inline bool update(const QPair<U1, U2> & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      { return qx::dao::detail::QxDao_Update_Container<T>::updateItem(item.second, dao); }
   };

   template <typename U>
   struct updateItem_Helper<U, false>
   {
      static bool update(U & item, qx::dao::detail::QxDao_Helper_Container<T> & dao)
      {
         QStringList columns = dao.getSqlColumns();
         if (! dao.isValidPrimaryKey(item)) { dao.errInvalidId(); return false; }
         IxSqlGenerator * pSqlGenerator = dao.getSqlGenerator();
         if (pSqlGenerator) { pSqlGenerator->onBeforeUpdate((& dao), (& item)); }
         qx::dao::on_before_update<U>((& item), (& dao));
         qx::dao::detail::QxSqlQueryHelper_Update<U>::resolveInput(item, dao.query(), dao.builder(), columns);
         if (! dao.qxQuery().isEmpty()) { dao.qxQuery().resolve(dao.query()); }
         if (! dao.query().exec()) { dao.errFailed(); return false; }
         qx::dao::on_after_update<U>((& item), (& dao));
         if (pSqlGenerator) { pSqlGenerator->onAfterUpdate((& dao), (& item)); }

         return dao.isValid();
      }
   };

};

template <class T>
struct QxDao_Update_Ptr
{

   static inline QSqlError update(const qx::QxSqlQuery & query, T & t,
                  QSqlDatabase * pDatabase, const QStringList & columns, const QString& table_name)
   { return (t ? qx::dao::update_by_query(query, (* t), pDatabase, columns, table_name) : QSqlError()); }

};

template <class T>
struct QxDao_Update
{

   static inline QSqlError update(const qx::QxSqlQuery & query, T & t,
                      QSqlDatabase * pDatabase, const QStringList & columns,
                                  const QString& table_name)
   {
      typedef typename boost::mpl::if_c< boost::is_pointer<T>::value, qx::dao::detail::QxDao_Update_Ptr<T>, qx::dao::detail::QxDao_Update_Generic<T> >::type type_dao_1;
      typedef typename boost::mpl::if_c< qx::trait::is_smart_ptr<T>::value, qx::dao::detail::QxDao_Update_Ptr<T>, type_dao_1 >::type type_dao_2;
      typedef typename boost::mpl::if_c< qx::trait::is_container<T>::value, qx::dao::detail::QxDao_Update_Container<T>, type_dao_2 >::type type_dao_3;

      QSqlError error = type_dao_3::update(query, t, pDatabase, columns, table_name);
      if (! error.isValid()) { qx::dao::detail::QxDao_Keep_Original<T>::backup(t); }
      return error;
   }

};

} // namespace detail
} // namespace dao
} // namespace qx
