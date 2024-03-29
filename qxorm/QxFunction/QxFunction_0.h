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

#ifndef _QX_FUNCTION_0_H_
#define _QX_FUNCTION_0_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file QxFunction_0.h
 * \author Lionel Marty
 * \ingroup QxFunction
 * \brief Concrete function class registered into QxOrm context without parameter
 */

#include <qxorm/QxFunction/IxFunction.h>
#include <qxorm/QxFunction/QxParameters.h>

namespace qx {

/*!
 * \ingroup QxFunction
 * \brief qx::QxFunction_0<Owner, R> : concrete function registered into QxOrm context defined into class Owner, returning an object of type R and without parameter
 */
template <class Owner, typename R>
class QxFunction_0 : public IxFunction
{

public:

   typedef boost::function<R (Owner *)> type_fct;
   QX_FUNCTION_CLASS_MEMBER_FCT(QxFunction_0);

   virtual qx_bool isValidParams(const QString & params) const          { Q_UNUSED(params); return true; }
   virtual qx_bool isValidParams(const type_any_params & params) const  { Q_UNUSED(params); return true; }

private:

   template <class T, bool bReturnValue /* = false */>
   struct QxInvokerFct
   {
      static inline qx_bool invoke(void * pOwner, const T & params, boost::any * ret, const QxFunction_0 * pThis)
      {
         QX_FUNCTION_INVOKE_START_WITH_OWNER();
         try { pThis->m_fct(static_cast<Owner *>(pOwner)); }
         QX_FUNCTION_CATCH_AND_RETURN_INVOKE();
      }
   };

   template <class T>
   struct QxInvokerFct<T, true>
   {
      static inline qx_bool invoke(void * pOwner, const T & params, boost::any * ret, const QxFunction_0 * pThis)
      {
         QX_FUNCTION_INVOKE_START_WITH_OWNER();
         try { R retTmp = pThis->m_fct(static_cast<Owner *>(pOwner)); if (ret) { (* ret) = boost::any(retTmp); } }
         QX_FUNCTION_CATCH_AND_RETURN_INVOKE();
      }
   };

};

template <typename R>
class QxFunction_0<void, R> : public IxFunction
{

public:

   typedef boost::function<R ()> type_fct;
   QX_FUNCTION_CLASS_FCT(QxFunction_0);

   virtual qx_bool isValidParams(const QString & params) const          { Q_UNUSED(params); return true; }
   virtual qx_bool isValidParams(const type_any_params & params) const  { Q_UNUSED(params); return true; }

private:

   template <class T, bool bReturnValue /* = false */>
   struct QxInvokerFct
   {
      static inline qx_bool invoke(const T & params, boost::any * ret, const QxFunction_0 * pThis)
      {
         QX_FUNCTION_INVOKE_START_WITHOUT_OWNER();
         try { pThis->m_fct(); }
         QX_FUNCTION_CATCH_AND_RETURN_INVOKE();
      }
   };

   template <class T>
   struct QxInvokerFct<T, true>
   {
      static inline qx_bool invoke(const T & params, boost::any * ret, const QxFunction_0 * pThis)
      {
         QX_FUNCTION_INVOKE_START_WITHOUT_OWNER();
         try { R retTmp = pThis->m_fct(); if (ret) { (* ret) = boost::any(retTmp); } }
         QX_FUNCTION_CATCH_AND_RETURN_INVOKE();
      }
   };

};

namespace function {

template <class Owner, typename R>
IxFunction_ptr bind_fct_0(const typename QxFunction_0<Owner, R>::type_fct & fct)
{
   typedef boost::is_same<Owner, void> qx_verify_owner_tmp;
   BOOST_STATIC_ASSERT(qx_verify_owner_tmp::value);
   IxFunction_ptr ptr; ptr.reset(new QxFunction_0<void, R>(fct));
   return ptr;
}

template <class Owner, typename R>
IxFunction_ptr bind_member_fct_0(const typename QxFunction_0<Owner, R>::type_fct & fct)
{
   typedef boost::is_same<Owner, void> qx_verify_owner_tmp;
   BOOST_STATIC_ASSERT(! qx_verify_owner_tmp::value);
   IxFunction_ptr ptr; ptr.reset(new QxFunction_0<Owner, R>(fct));
   return ptr;
}

} // namespace function
} // namespace qx

#endif // _QX_FUNCTION_0_H_
