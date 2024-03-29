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

#ifndef _QX_CACHE_H_
#define _QX_CACHE_H_

#ifdef _MSC_VER
#pragma once
#endif

/*!
 * \file QxCache.h
 * \author Lionel Marty
 * \ingroup QxCache
 * \brief qx::cache : based on singleton pattern, provide basic thread-safe cache feature to backup and restore any kind of objects (for example, object fetched from database)
 */

#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>

#include <qxorm/QxCommon/QxBool.h>

#include <qxorm/QxCollection/QxCollection.h>

#include <qxorm/QxSingleton/QxSingleton.h>

namespace qx {
namespace cache {
namespace detail {

class QX_DLL_EXPORT QxCache : public qx::QxSingleton<QxCache>
{

   friend class qx::QxSingleton<QxCache>;

protected:

   typedef boost::tuple<long, QDateTime, boost::any> type_qx_cache;
   typedef qx::QxCollection<QString, type_qx_cache> type_qx_lst_cache;

   type_qx_lst_cache m_cache;    //!< List of objects in cache under boost::any format
   QMutex m_oMutexCache;         //!< Mutex => 'QxCache' is thread-safe
   long m_lMaxCost;              //!< Max cost before deleting object in cache
   long m_lCurrCost;             //!< Current cost in cache

public:

   QxCache();
   virtual ~QxCache();

   long getCurrCost() const;
   long getMaxCost() const;
   void setMaxCost(long l);

   long count() const;
   long size() const;
   bool isEmpty() const;
   bool exist(const QString & sKey) const;
   bool contains(const QString & sKey) const;
   boost::any at(const QString & sKey);
   long insertionCost(const QString & sKey);
   QDateTime insertionDateTime(const QString & sKey);
   void clear();

   bool insert(const QString & sKey, const boost::any & anyObj, long lCost = 1, const QDateTime & dt = QDateTime());
   bool remove(const QString & sKey);

private:

   void updateCost();

};

} // namespace detail
} // namespace cache
} // namespace qx

QX_DLL_EXPORT_QX_SINGLETON_HPP(qx::cache::detail::QxCache)

namespace qx {
namespace cache {

/*!
 * \ingroup QxCache
 * \brief Set the maximum allowed total cost of the cache to l. If the current total cost is greater than l, some objects are deleted immediately
 */
inline void max_cost(long l)
{ qx::cache::detail::QxCache::getSingleton()->setMaxCost(l); }

/*!
 * \ingroup QxCache
 * \brief Return the maximum allowed total cost of the cache
 */
inline long max_cost()
{ return qx::cache::detail::QxCache::getSingleton()->getMaxCost(); }

/*!
 * \ingroup QxCache
 * \brief Return the current cost used by the cache
 */
inline long current_cost()
{ return qx::cache::detail::QxCache::getSingleton()->getCurrCost(); }

/*!
 * \ingroup QxCache
 * \brief Return the number of objects in the cache
 */
inline long count()
{ return qx::cache::detail::QxCache::getSingleton()->count(); }

/*!
 * \ingroup QxCache
 * \brief Return true if the cache contains no object; otherwise return false
 */
inline bool is_empty()
{ return qx::cache::detail::QxCache::getSingleton()->isEmpty(); }

/*!
 * \ingroup QxCache
 * \brief Delete all the objects in the cache
 */
inline void clear()
{ qx::cache::detail::QxCache::getSingleton()->clear(); }

/*!
 * \ingroup QxCache
 * \brief Return true if the cache contains an object associated with key sKey; otherwise return false
 */
inline bool exist(const QString & sKey)
{ return qx::cache::detail::QxCache::getSingleton()->exist(sKey); }

/*!
 * \ingroup QxCache
 * \brief Delete the object associated with key sKey. Return true if the object was found in the cache; otherwise return false
 */
inline bool remove(const QString & sKey)
{ return qx::cache::detail::QxCache::getSingleton()->remove(sKey); }

/*!
 * \ingroup QxCache
 * \brief Insert object t into the cache with key sKey, associated cost lCost and insertion date-time dt. Any object with the same key already in the cache will be removed
 */
template <typename T>
inline bool set(const QString & sKey, T & t, long lCost = 1, const QDateTime & dt = QDateTime())
{
   boost::any obj(t);
   return qx::cache::detail::QxCache::getSingleton()->insert(sKey, obj, lCost, dt);
}

/*!
 * \ingroup QxCache
 * \brief Return the object of type T associated with key sKey, or return default instance of T() if the key does not exist in the cache
 */
template <typename T>
inline T get(const QString & sKey)
{
   boost::any obj = qx::cache::detail::QxCache::getSingleton()->at(sKey);
   if (obj.empty()) { return T(); }
   try { return boost::any_cast<T>(obj); }
   catch (const boost::bad_any_cast & err) { Q_UNUSED(err); return T(); }
}

/*!
 * \ingroup QxCache
 * \brief Return true if object t can be fetched with associated key sKey and insertion date-time dt; otherwise return false with an error description
 */
template <typename T>
inline qx_bool get(const QString & sKey, T & t, QDateTime & dt)
{
   dt = QDateTime();
   if (! qx::cache::exist(sKey)) { return qx_bool(false, 0, "[QxOrm] qx::cache : key doesn't exist in cache"); }
   boost::any obj = qx::cache::detail::QxCache::getSingleton()->at(sKey);
   dt = qx::cache::detail::QxCache::getSingleton()->insertionDateTime(sKey);
   try { t = boost::any_cast<T>(obj); return qx_bool(true); }
   catch (const boost::bad_any_cast & err) { Q_UNUSED(err); return qx_bool(false, 0, "[QxOrm] qx::cache : bad any cast exception"); }
}

/*!
 * \ingroup QxCache
 * \brief Return true if object t can be fetched with associated key sKey; otherwise return false with an error description
 */
template <typename T>
inline qx_bool get(const QString & sKey, T & t)
{
   QDateTime dt;
   return qx::cache::get<T>(sKey, t, dt);
}

} // namespace cache
} // namespace qx

#endif // _QX_CACHE_H_
