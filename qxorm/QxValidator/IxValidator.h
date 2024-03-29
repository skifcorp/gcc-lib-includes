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

#ifndef _IX_VALIDATOR_H_
#define _IX_VALIDATOR_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file IxValidator.h
 * \author Lionel Marty
 * \ingroup QxValidator
 * \brief Common interface for validator engine
 */

#include <qxorm/QxCommon/QxPropertyBag.h>

namespace qx {

class IxDataMember;
class QxInvalidValueX;

/*!
 * \ingroup QxValidator
 * \brief qx::IxValidator : common interface for validator engine
 *
 * For more informations about <b>QxValidator module</b>, <a href="http://www.qxorm.com/qxorm_en/faq.html#faq_250" target="_blank">goto the FAQ of QxOrm website</a> :
 * <a href="http://www.qxorm.com/qxorm_en/faq.html#faq_250" target="_blank">http://www.qxorm.com/qxorm_en/faq.html#faq_250</a>
 */
class QX_DLL_EXPORT IxValidator : public QxPropertyBag
{

public:

   enum validator_type
   {
      not_null, not_empty, min_value, max_value, 
      min_length, max_length, date_past, date_future, 
      min_decimal, max_decimal, regular_expression, e_mail, 
      recursive_validator, custom_validator
   };

protected:

   validator_type    m_type;           //!< Validator type
   QString           m_message;        //!< Validator message when invalid value is detected
   QString           m_group;          //!< Validator group
   QVariantList      m_Constraints;    //!< List of constraints to verify
   IxDataMember *    m_pDataMember;    //!< Registered property associated to validator

public:

   IxValidator(validator_type type);
   virtual ~IxValidator();

   validator_type getType() const;
   QString getMessage() const;
   QString getGroup() const;
   QVariant getConstraint() const;
   QVariantList getConstraints() const;
   IxDataMember * getDataMember() const;

   void setMessage(const QString & s);
   void setGroup(const QString & s);
   void setConstraint(const QVariant & v);
   void setConstraints(const QVariantList & lst);
   void setDataMember(IxDataMember * p);

   virtual void validate(void * pOwner, QxInvalidValueX & lstInvalidValues) const;

protected:

   void initDefaultMessage();

   void validateNotNull(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;
   void validateNotEmpty(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;
   void validateMinValue(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;
   void validateMaxValue(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;
   void validateMinDecimal(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;
   void validateMaxDecimal(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;
   void validateMinLength(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;
   void validateMaxLength(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;
   void validateDatePast(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;
   void validateDateFuture(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;
   void validateRegularExpression(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;
   void validateEMail(const QVariant & v, QxInvalidValueX & lstInvalidValues) const;

};

typedef boost::shared_ptr<IxValidator> IxValidator_ptr;

} // namespace qx

#endif // _IX_VALIDATOR_H_
