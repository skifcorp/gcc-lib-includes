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

#ifndef _QX_CLASS_X_H_
#define _QX_CLASS_X_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file QxClassX.h
 * \author Lionel Marty
 * \ingroup QxRegister
 * \brief List of all classes registered into QxOrm context
 */

#include <boost/function.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <qxorm/QxCommon/QxBool.h>

#include <qxorm/QxRegister/IxClass.h>

#include <qxorm/QxSingleton/QxSingleton.h>

#include <qxorm/QxCollection/QxCollection.h>

#include <qxorm/QxTraits/is_smart_ptr.h>
#include <qxorm/QxTraits/get_sql_type.h>

namespace qx {

/*!
 * \ingroup QxRegister
 * \brief qx::QxClassX : list of all classes registered into QxOrm context (this container is a singleton)
 */
class QX_DLL_EXPORT QxClassX : public QxSingleton<QxClassX>
{

   friend class QxSingleton<QxClassX>;
   friend class IxClass;

public:

   typedef IxFunction::type_any_params type_any_params;
   typedef boost::function<QString (const QVariant &, int, const unsigned int)> type_fct_save_qvariant_usertype;
   typedef boost::function<QVariant (const QString &, int, const unsigned int)> type_fct_load_qvariant_usertype;

protected:

   QxCollection<QString, IxClass *> m_lstClass;                   //!< Collection of classes registered into QxOrm context
   QHash<QString, QString> m_lstSqlTypeByClassName;               //!< List of SQL types by class name
   QHash<QString, QString> m_lstValidatorMessage;                 //!< List of validator message when an invalid value is detected
   type_fct_save_qvariant_usertype m_fctSaveQVariantUserType;     //!< Serialization of QVariant UserType => function to save a QVariant UserType (cf. QxSerialize_QVariant.cpp file)
   type_fct_load_qvariant_usertype m_fctLoadQVariantUserType;     //!< Serialization of QVariant UserType => function to load a QVariant UserType (cf. QxSerialize_QVariant.cpp file)

private:

   QxClassX();
   virtual ~QxClassX();

   QxCollection<QString, IxClass *> * getAll();
   IxClass * get(const QString & sKey) const;
   bool exist(const QString & sKey) const;
   bool insert(const QString & sKey, IxClass * pClass);
   bool remove(const QString & sKey);
   void clear();
   void initSqlTypeByClassName();
   void initValidatorMessage();
   const std::type_info & typeInfo(const QString & sKey) const;

public:

   static boost::any create(const QString & sKey);
   static IxClass * getClass(const QString & sKey);
   static IxDataMemberX * getDataMemberX(const QString & sKey);
   static IxFunctionX * getFctMemberX(const QString & sKey);
   static IxDataMember * getDataMember(const QString & sClassKey, const QString & sDataKey, bool bRecursive = true);
   static IxFunction * getFctMember(const QString & sClassKey, const QString & sFctKey, bool bRecursive = true);

   static QxCollection<QString, IxClass *> * getAllClasses();
   static void registerAllClasses();
   static QString dumpAllClasses();
   static QString dumpSqlSchema();

   static QHash<QString, QString> * getAllValidatorMessage()               { return (& QxClassX::getSingleton()->m_lstValidatorMessage); }
   static QHash<QString, QString> * getAllSqlTypeByClassName()             { return (& QxClassX::getSingleton()->m_lstSqlTypeByClassName); }
   static QString getSqlTypeByClassName(const QString & sClassName)        { return QxClassX::getAllSqlTypeByClassName()->value(sClassName); }
   static const std::type_info & getTypeInfo(const QString & sClassName)   { return QxClassX::getSingleton()->typeInfo(sClassName); }

   static type_fct_save_qvariant_usertype getFctSaveQVariantUserType()           { return QxClassX::getSingleton()->m_fctSaveQVariantUserType; }
   static type_fct_load_qvariant_usertype getFctLoadQVariantUserType()           { return QxClassX::getSingleton()->m_fctLoadQVariantUserType; }
   static void setFctSaveQVariantUserType(type_fct_save_qvariant_usertype fct)   { QxClassX::getSingleton()->m_fctSaveQVariantUserType = fct; }
   static void setFctLoadQVariantUserType(type_fct_load_qvariant_usertype fct)   { QxClassX::getSingleton()->m_fctLoadQVariantUserType = fct; }

   template <class U>
   static inline qx_bool invoke(const QString & sClassKey, const QString & sFctKey, U & pOwner, const QString & params = QString(), boost::any * ret = NULL)
   {
      typedef typename boost::mpl::if_c< boost::is_pointer<U>::value, QxClassX::invoke_ptr<U>, QxClassX::invoke_default<U> >::type type_invoke_1;
      typedef typename boost::mpl::if_c< qx::trait::is_smart_ptr<U>::value, QxClassX::invoke_ptr<U>, type_invoke_1 >::type type_invoke_2;
      return type_invoke_2::invoke(sClassKey, sFctKey, pOwner, params, ret);
   }

   template <class U>
   static inline qx_bool invoke(const QString & sClassKey, const QString & sFctKey, U & pOwner, const type_any_params & params, boost::any * ret = NULL)
   {
      typedef typename boost::mpl::if_c< boost::is_pointer<U>::value, QxClassX::invoke_ptr<U>, QxClassX::invoke_default<U> >::type type_invoke_1;
      typedef typename boost::mpl::if_c< qx::trait::is_smart_ptr<U>::value, QxClassX::invoke_ptr<U>, type_invoke_1 >::type type_invoke_2;
      return type_invoke_2::invoke(sClassKey, sFctKey, pOwner, params, ret);
   }

private:

   template <class U>
   struct invoke_ptr
   {
      static inline qx_bool invoke(const QString & sClassKey, const QString & sFctKey, U & pOwner, const QString & params = QString(), boost::any * ret = NULL)
      { return QxClassX::invokeVoidPtr(sClassKey, sFctKey, static_cast<void *>(& (* pOwner)), params, ret); }
      static inline qx_bool invoke(const QString & sClassKey, const QString & sFctKey, U & pOwner, const type_any_params & params, boost::any * ret = NULL)
      { return QxClassX::invokeVoidPtr(sClassKey, sFctKey, static_cast<void *>(& (* pOwner)), params, ret); }
   };

   template <class U>
   struct invoke_default
   {
      static inline qx_bool invoke(const QString & sClassKey, const QString & sFctKey, U & pOwner, const QString & params = QString(), boost::any * ret = NULL)
      { return QxClassX::invokeVoidPtr(sClassKey, sFctKey, static_cast<void *>(& pOwner), params, ret); }
      static inline qx_bool invoke(const QString & sClassKey, const QString & sFctKey, U & pOwner, const type_any_params & params, boost::any * ret = NULL)
      { return QxClassX::invokeVoidPtr(sClassKey, sFctKey, static_cast<void *>(& pOwner), params, ret); }
   };

   static qx_bool invokeVoidPtr(const QString & sClassKey, const QString & sFctKey, void * pOwner, const QString & params = QString(), boost::any * ret = NULL);
   static qx_bool invokeVoidPtr(const QString & sClassKey, const QString & sFctKey, void * pOwner, const type_any_params & params, boost::any * ret = NULL);

   static bool isValid_DataMember(IxDataMember * p);
   static bool isValid_SqlRelation(IxDataMember * p);

};

} // namespace qx

QX_DLL_EXPORT_QX_SINGLETON_HPP(qx::QxClassX)

#endif // _QX_CLASS_X_H_
