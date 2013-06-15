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

#ifndef _QX_ORM_H_
#define _QX_ORM_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \defgroup QxCommon QxCommon : QxOrm library common tools and macros
 * \defgroup QxCollection QxCollection : QxOrm library container (keep insertion order + quick access by index + quick access by key)
 * \defgroup QxSingleton QxSingleton : QxOrm library thread-safe singleton pattern
 * \defgroup QxFactory QxFactory : QxOrm library factory pattern used by introspection engine
 * \defgroup QxDataMember QxDataMember : QxOrm library data member (or property) used by introspection engine
 * \defgroup QxFunction QxFunction : QxOrm library function used by introspection engine
 * \defgroup QxRegister QxRegister : QxOrm library register context used by introspection engine (class, function, property...)
 * \defgroup QxSerialize QxSerialize : QxOrm library serialization engine based on 'boost::serialization' library
 * \defgroup QxTraits QxTraits : QxOrm library traits (template metaprogramming) not available in 'boost::type_traits' library
 * \defgroup QxDao QxDao : QxOrm library database communication used by persistence engine (ORM - Object Relational Mapping)
 * \defgroup QxValidator QxValidator : QxOrm library validation engine using validator pattern
 * \defgroup QxService QxService : QxOrm library services engine to provide easy and powerful way to create C++ application server (to transfer data over network)
 * \defgroup QxCache QxCache : QxOrm library basic thread-safe cache feature to backup and restore any kind of objects (for example, object fetched from database)
 * \defgroup QxMemLeak QxMemLeak : QxOrm library memory leak detection (by Wu Yongwei)
 */

#include <qxorm/QxPrecompiled.h>

#include <qxorm/QxCommon/QxConfig.h>
#include <qxorm/QxCommon/QxMacro.h>
#include <qxorm/QxCommon/QxHashValue.h>
#include <qxorm/QxCommon/QxBool.h>
#include <qxorm/QxCommon/QxCache.h>
#include <qxorm/QxCommon/QxPropertyBag.h>
#include <qxorm/QxCommon/QxSimpleCrypt.h>

#include <qxorm/QxMemLeak/bool_array.h>
#include <qxorm/QxMemLeak/class_level_lock.h>
#include <qxorm/QxMemLeak/cont_ptr_utils.h>
#include <qxorm/QxMemLeak/fast_mutex.h>
#include <qxorm/QxMemLeak/fixed_mem_pool.h>
#include <qxorm/QxMemLeak/mem_pool_base.h>
#include <qxorm/QxMemLeak/object_level_lock.h>
#include <qxorm/QxMemLeak/pctimer.h>
#include <qxorm/QxMemLeak/set_assign.h>
#include <qxorm/QxMemLeak/static_assert.h>
#include <qxorm/QxMemLeak/static_mem_pool.h>

#include <qxorm/QxCollection/IxCollection.h>
#include <qxorm/QxCollection/QxCollection.h>
#include <qxorm/QxCollection/QxCollectionIterator.h>
#include <qxorm/QxCollection/QxForeach.h>

#include <qxorm/QxSingleton/IxSingleton.h>
#include <qxorm/QxSingleton/QxSingleton.h>
#include <qxorm/QxSingleton/QxSingletonX.h>

#include <qxorm/QxFactory/IxFactory.h>
#include <qxorm/QxFactory/QxFactory.h>
#include <qxorm/QxFactory/QxFactoryX.h>

#include <qxorm/QxTraits/qx_traits.h>

#include <qxorm/QxDataMember/IxDataMember.h>
#include <qxorm/QxDataMember/IxDataMemberX.h>
#include <qxorm/QxDataMember/QxDataMember.h>
#include <qxorm/QxDataMember/QxDataMemberX.h>
#include <qxorm/QxDataMember/QxDataMember_QObject.h>

#include <qxorm/QxFunction/QxFunctionInclude.h>

#include <qxorm/QxXml/QxXmlReader.h>
#include <qxorm/QxXml/QxXmlWriter.h>
#include <qxorm/QxXml/QxXml.h>

#include <qxorm/QxDao/IxSqlQueryBuilder.h>
#include <qxorm/QxDao/QxSqlQueryBuilder.h>
#include <qxorm/QxDao/QxSqlQueryHelper.h>
#include <qxorm/QxDao/QxSqlQuery.h>
#include <qxorm/QxDao/QxSqlDatabase.h>
#include <qxorm/QxDao/IxSqlRelation.h>
#include <qxorm/QxDao/QxSqlRelation.h>
#include <qxorm/QxDao/QxSqlRelationParams.h>
#include <qxorm/QxDao/QxSqlRelation_ManyToMany.h>
#include <qxorm/QxDao/QxSqlRelation_ManyToOne.h>
#include <qxorm/QxDao/QxSqlRelation_OneToMany.h>
#include <qxorm/QxDao/QxSqlRelation_OneToOne.h>
#include <qxorm/QxDao/QxSqlRelation_RawData.h>
#include <qxorm/QxDao/QxDao.h>
#include <qxorm/QxDao/QxDao_Impl.h>
#include <qxorm/QxDao/QxDaoStrategy.h>
#include <qxorm/QxDao/QxDaoPointer.h>
#include <qxorm/QxDao/QxDao_IsDirty.h>
#include <qxorm/QxDao/QxSoftDelete.h>
#include <qxorm/QxDao/QxSqlError.h>
#include <qxorm/QxDao/QxSession.h>
#include <qxorm/QxDao/QxDateNeutral.h>
#include <qxorm/QxDao/QxTimeNeutral.h>
#include <qxorm/QxDao/QxDateTimeNeutral.h>
#include <qxorm/QxDao/IxPersistable.h>
#include <qxorm/QxDao/QxSqlJoin.h>
#include <qxorm/QxDao/QxSqlRelationLinked.h>

#include <qxorm/QxDao/QxSqlElement/QxSqlElement.h>

#include <qxorm/QxDao/QxSqlGenerator/QxSqlGenerator.h>

#include <qxorm/QxDao/QxRepository/IxRepository.h>
#include <qxorm/QxDao/QxRepository/QxRepository.h>
#include <qxorm/QxDao/QxRepository/QxRepositoryX.h>

#include <qxorm/QxSerialize/QxSerialize.h>

#include <qxorm/QxCommon/QxStringCvt.h>
#include <qxorm/QxCommon/QxStringCvt_Impl.h>
#include <qxorm/QxCommon/QxStringCvt_Export.h>
#include <qxorm/QxCommon/QxAnyCastDynamic.h>

#include <qxorm/QxRegister/IxClass.h>
#include <qxorm/QxRegister/QxClass.h>
#include <qxorm/QxRegister/QxClassX.h>
#include <qxorm/QxRegister/QxClassName.h>
#include <qxorm/QxRegister/QxRegister.h>
#include <qxorm/QxRegister/QxRegisterInternalHelper.h>
#include <qxorm/QxRegister/IxTypeInfo.h>
#include <qxorm/QxRegister/QxRegisterQtProperty.h>

#include <qxorm/QxService/IxParameter.h>
#include <qxorm/QxService/IxService.h>
#include <qxorm/QxService/QxClientAsync.h>
#include <qxorm/QxService/QxConnect.h>
#include <qxorm/QxService/QxServer.h>
#include <qxorm/QxService/QxService.h>
#include <qxorm/QxService/QxThread.h>
#include <qxorm/QxService/QxThreadPool.h>
#include <qxorm/QxService/QxTools.h>
#include <qxorm/QxService/QxTransaction.h>

#include <qxorm/QxValidator/IxValidator.h>
#include <qxorm/QxValidator/IxValidatorX.h>
#include <qxorm/QxValidator/QxInvalidValue.h>
#include <qxorm/QxValidator/QxInvalidValueX.h>
#include <qxorm/QxValidator/QxValidator.h>
#include <qxorm/QxValidator/QxValidatorX.h>
#include <qxorm/QxValidator/QxValidatorError.h>
#include <qxorm/QxValidator/QxValidatorFct.h>

#endif // _QX_ORM_H_
