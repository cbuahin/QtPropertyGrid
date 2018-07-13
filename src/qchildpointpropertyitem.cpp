/*!
 * \author Caleb Amoa Buahin <caleb.buahin@gmail.com>
 * \version 1.0.0
 * \description
 * \license
 * This file and its associated files, and libraries are free software.
 * You can redistribute it and/or modify it under the terms of the
 * Lesser GNU Lesser General Public License as published by the Free Software Foundation;
 * either version 3 of the License, or (at your option) any later version.
 * This file and its associated files is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.(see <http://www.gnu.org/licenses/> for details)
 * \copyright Copyright 2014-2018, Caleb Buahin, All rights reserved.
 * \date 2014-2018
 * \pre
 * \bug
 * \warning
 * \todo
 */

#include "stdafx.h"
#include "qchildpropertyitems.h"
#include "qpropertymodel.h"

QChildPointPropertyItem::QChildPointPropertyItem(const QVariant& value, const QString& name, QPropertyItem *parent)
   : QPropertyItem(value, name, parent), m_isSettingChildren(false)
{

}

QChildPointPropertyItem::~QChildPointPropertyItem()
{

}

QVariant QChildPointPropertyItem::data(int column, Qt::ItemDataRole  role) const
{

   switch (column)
   {
      case QPropertyItem::Property:
         switch (role)
         {
            case Qt::DisplayRole:
               return m_name;
               break;
         }

         break;
      case QPropertyItem::Value:
         switch (role)
         {
            case Qt::EditRole:
               {
                  return  m_value;

               }break;
            case Qt::DisplayRole:
            case Qt::ToolTipRole:
            case Qt::StatusTipRole:
            case Qt::WhatsThisRole:
               {
                  QPoint v = qvariant_cast<QPoint>(m_value);
                  QString label = QString("[X=%1, Y=%2]").arg(v.x()).arg(v.y());
                  return label;
               }
               break;
         }

         break;
   }
   return QVariant();
}

void QChildPointPropertyItem::setChildValues()
{
   if (!m_isSettingChildren)
   {
      m_isSettingChildren = true;

      QPoint v = qvariant_cast<QPoint>(m_value);

      for (int i = 0; i < m_children.count(); i++)
      {
         QPropertyItem* child = m_children[i];
         QString propertyName = child->name();
         QVariant tval;

         if (propertyName == "X")
         {
            tval = v.x();
         }
         else if (propertyName == "Y")
         {
            tval = v.y();
         }
         m_model->setData(child->index(), tval);
      }


      m_isSettingChildren = false;
   }
}

bool QChildPointPropertyItem::hasChildren()
{
   if (!m_childrenSet)
   {
      m_childrenSet = true;

      QPoint v = qvariant_cast<QPoint>(m_value);

      QPropertyItem* X = new QPropertyItem(v.x(), "X", this);
      m_children.append(X);
      connect(X, SIGNAL(valueChanged(const QString&, const QVariant&)), this,
              SLOT(onChildItemValueChanged(const QString&, const QVariant&)));

      QPropertyItem* Y = new QPropertyItem(v.y(), "Y", this);
      m_children.append(Y);
      connect(Y, SIGNAL(valueChanged(const QString&, const QVariant&)), this,
              SLOT(onChildItemValueChanged(const QString&, const QVariant&)));


      return true;
   }
   else
   {
      if (m_children.count() > 0)
      {
         return true;
      }
   }

   return false;
}

Qt::ItemFlags QChildPointPropertyItem::flags() const
{
   Qt::ItemFlags  flags;

   if (m_isCheckable)
      flags = flags | Qt::ItemFlag::ItemIsUserCheckable;
   if (m_isEnabled)
      flags = flags | Qt::ItemFlag::ItemIsEnabled;
   //if (m_isEditable)
   //	flags = flags | Qt::ItemFlag::ItemIsEditable;
   if (m_isEnabled)
      flags = flags | Qt::ItemFlag::ItemIsEnabled;
   if (m_isSelectable)
      flags = flags | Qt::ItemFlag::ItemIsSelectable;
   if (m_isTristate)
      flags = flags | Qt::ItemFlag::ItemIsTristate;

   return flags;
}

void QChildPointPropertyItem::onChildItemValueChanged(const QString& name, const QVariant& value)
{
   if (!m_isSettingChildren)
   {
      m_isSettingChildren = true;

      QPoint v = qvariant_cast<QPoint>(m_value);

      if (name == "X")
      {
         v.setX(value.toDouble());
      }
      else if (name == "Y")
      {
         v.setY(value.toDouble());
      }

      m_model->setData(m_index, v);

      m_isSettingChildren = false;

   }
}
