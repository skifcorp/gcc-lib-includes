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

#ifndef _QX_TRAITS_H_
#define _QX_TRAITS_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <qxorm/QxTraits/get_base_class.h>
#include <qxorm/QxTraits/get_primary_key.h>
#include <qxorm/QxTraits/get_class_name.h>
#include <qxorm/QxTraits/get_class_name_primitive.h>
#include <qxorm/QxTraits/is_boost_intrusive_ptr.h>
#include <qxorm/QxTraits/is_boost_scoped_ptr.h>
#include <qxorm/QxTraits/is_boost_shared_ptr.h>
#include <qxorm/QxTraits/is_boost_unordered_map.h>
#include <qxorm/QxTraits/is_boost_unordered_set.h>
#include <qxorm/QxTraits/is_boost_weak_ptr.h>
#include <qxorm/QxTraits/is_container.h>
#include <qxorm/QxTraits/is_container_base_of.h>
#include <qxorm/QxTraits/is_container_key_value.h>
#include <qxorm/QxTraits/is_container_to_pod.h>
#include <qxorm/QxTraits/is_ptr_base_of.h>
#include <qxorm/QxTraits/is_ptr_to_pod.h>
#include <qxorm/QxTraits/is_qt_hash.h>
#include <qxorm/QxTraits/is_qt_linked_list.h>
#include <qxorm/QxTraits/is_qt_list.h>
#include <qxorm/QxTraits/is_qt_map.h>
#include <qxorm/QxTraits/is_qt_multi_hash.h>
#include <qxorm/QxTraits/is_qt_multi_map.h>
#include <qxorm/QxTraits/is_qt_set.h>
#include <qxorm/QxTraits/is_qt_shared_data_ptr.h>
#include <qxorm/QxTraits/is_qt_shared_ptr.h>
#include <qxorm/QxTraits/is_qt_scoped_ptr.h>
#include <qxorm/QxTraits/is_qt_vector.h>
#include <qxorm/QxTraits/is_qt_weak_ptr.h>
#include <qxorm/QxTraits/is_qt_variant_compatible.h>
#include <qxorm/QxTraits/is_qx_collection.h>
#include <qxorm/QxTraits/is_qx_pod.h>
#include <qxorm/QxTraits/is_qx_registered.h>
#include <qxorm/QxTraits/is_smart_ptr.h>
#include <qxorm/QxTraits/is_smart_ptr_base_of.h>
#include <qxorm/QxTraits/is_smart_ptr_to_pod.h>
#include <qxorm/QxTraits/is_std_list.h>
#include <qxorm/QxTraits/is_std_map.h>
#include <qxorm/QxTraits/is_std_set.h>
#include <qxorm/QxTraits/is_std_vector.h>
#include <qxorm/QxTraits/archive_wide_traits.h>
#include <qxorm/QxTraits/archive_printable.h>
#include <qxorm/QxTraits/remove_attr.h>
#include <qxorm/QxTraits/remove_smart_ptr.h>
#include <qxorm/QxTraits/construct_ptr.h>
#include <qxorm/QxTraits/get_sql_type.h>
#include <qxorm/QxTraits/generic_container.h>
#include <qxorm/QxTraits/is_valid_primary_key.h>
#include <qxorm/QxTraits/is_qx_dao_ptr.h>
#include <qxorm/QxTraits/is_equal.h>
#include <qxorm/QxTraits/qt_meta_object.h>

#endif // _QX_TRAITS_H_
