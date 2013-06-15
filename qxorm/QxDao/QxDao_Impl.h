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

#ifndef _QX_DAO_IMPL_H_
#define _QX_DAO_IMPL_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/static_assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <qxorm/QxDao/QxDao.h>
#include <qxorm/QxDao/QxDaoPointer.h>
#include <qxorm/QxDao/QxDao_IsDirty.h>
#include <qxorm/QxDao/QxSqlDatabase.h>
#include <qxorm/QxDao/QxSqlQueryBuilder.h>
#include <qxorm/QxDao/QxSqlQueryHelper.h>

#include <qxorm/QxCollection/QxCollection.h>

#include <qxorm/QxRegister/QxClass.h>

#include <qxorm/QxTraits/is_qx_registered.h>
#include <qxorm/QxTraits/is_container.h>
#include <qxorm/QxTraits/is_smart_ptr.h>
#include <qxorm/QxTraits/get_base_class.h>
#include <qxorm/QxTraits/get_class_name_primitive.h>
#include <qxorm/QxTraits/construct_ptr.h>
#include <qxorm/QxTraits/generic_container.h>
#include <qxorm/QxTraits/is_valid_primary_key.h>

#include "qxorm/inl/QxDao/QxDao_Helper.inl"
#include "qxorm/inl/QxDao/QxDao_Count.inl"
#include "qxorm/inl/QxDao/QxDao_FetchById.inl"
#include "qxorm/inl/QxDao/QxDao_FetchById_WithRelation.inl"
#include "qxorm/inl/QxDao/QxDao_FetchAll.inl"
#include "qxorm/inl/QxDao/QxDao_FetchAll_WithRelation.inl"
#include "qxorm/inl/QxDao/QxDao_Insert.inl"
#include "qxorm/inl/QxDao/QxDao_Insert_WithRelation.inl"
#include "qxorm/inl/QxDao/QxDao_Update.inl"
#include "qxorm/inl/QxDao/QxDao_Update_Optimized.inl"
#include "qxorm/inl/QxDao/QxDao_Update_WithRelation.inl"
#include "qxorm/inl/QxDao/QxDao_Save.inl"
#include "qxorm/inl/QxDao/QxDao_Save_WithRelation.inl"
#include "qxorm/inl/QxDao/QxDao_DeleteById.inl"
#include "qxorm/inl/QxDao/QxDao_DeleteAll.inl"
#include "qxorm/inl/QxDao/QxDao_Exist.inl"
#include "qxorm/inl/QxDao/QxDao_CreateTable.inl"
#include "qxorm/inl/QxDao/QxDao_Trigger.inl"
#include "qxorm/inl/QxDao/QxDao_ExecuteQuery.inl"

#endif // _QX_DAO_IMPL_H_
