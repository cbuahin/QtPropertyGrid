#include "qcustomeditors.h"
#include "qpropertymodel.h"

QVariantListPropertyItemEditor::QVariantListPropertyItemEditor(QWidget *parent)
   :QPopUpPropertyItemEditor(parent)
{
   m_editorDialog = new QDialog(this);
   ui.setupUi(m_editorDialog);
   m_editorDialog->setWindowTitle("QVariantList Editor");

   m_variantModel = new QStandardItemModel(ui.listViewItems);

   ui.listViewItems->setModel(m_variantModel);
   ui.listViewItems->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui.listViewItems->setSelectionMode(QAbstractItemView::ExtendedSelection);
   ui.listViewItems->setEditTriggers(QAbstractItemView::NoEditTriggers);

   m_propertyModel = new QPropertyModel(this);
   QPropertyItemDelegate* propertyModelDelegate = new QPropertyItemDelegate(m_propertyModel);


   ui.treeViewProperties->setModel(m_propertyModel);
   ui.treeViewProperties->setEditTriggers(QAbstractItemView::AllEditTriggers);
   ui.treeViewProperties->setItemDelegate(propertyModelDelegate);
   ui.treeViewProperties->setAlternatingRowColors(true);
   ui.treeViewProperties->expandToDepth(1);
   ui.treeViewProperties->resizeColumnToContents(0);
   ui.treeViewProperties->resizeColumnToContents(1);

   m_variantHolder = new QVariantHolderHelper(QVariant() , this);

   connect(ui.pushButtonOK, SIGNAL(clicked()), m_editorDialog, SLOT(accept()));

   connect(ui.listViewItems, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onItemClicked(const QModelIndex&)));

   connect(m_propertyModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int>)),
           this, SLOT(onDataChanged(const QModelIndex &, const QModelIndex &, const QVector<int>)));

   connect(ui.pushButtonAdd, SIGNAL(clicked()), this, SLOT(onAddQVariant()));
   connect(ui.pushButtonRemove, SIGNAL(clicked()), this, SLOT(onRemoveQVariant()));

}


QVariantListPropertyItemEditor::~QVariantListPropertyItemEditor()
{

}

void QVariantListPropertyItemEditor::setValue(const QVariant &value)
{
   m_values = qvariant_cast<QVariantList>(value);
   setUpChildWidget();
}

QVariant QVariantListPropertyItemEditor::getValue() const
{
   return m_values;
}

void QVariantListPropertyItemEditor::setUpChildWidget()
{

   m_variantModel->clear();

   if(m_values.length())
   {
      for(int i = 0 ; i < m_values.length() ; i++ )
      {
         QVariant v = m_values[i];
         QStandardItem* item = new QStandardItem();
         item->setData(v ,Qt::UserRole);
         item->setData(QString(v.typeName()) , Qt::DisplayRole);
         m_variantModel->appendRow(item);
      }
   }


   QVariantPropertyItem* test = (QVariantPropertyItem*) m_propertyItem ;

   if ( (m_propertyItem->isEditable() && !test ) || (test && test->metaProperty().isValid()
                                                    && test->metaProperty().isWritable()))
   {
      ui.pushButtonAdd->setEnabled(true);
      ui.pushButtonRemove->setEnabled(true);
      ui.treeViewProperties->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked |
                                             QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed);
   }
   else
   {
      ui.pushButtonAdd->setEnabled(false);
      ui.pushButtonRemove->setEnabled(false);
      ui.treeViewProperties->setEditTriggers(QAbstractItemView::NoEditTriggers);
   }
}


void QVariantListPropertyItemEditor::onItemClicked(const QModelIndex& index)
{
   m_currentlySelected = index;
   m_variantHolder->setValue(m_values[index.row()]);
   m_propertyModel->setData(QVariant::fromValue(m_variantHolder));
   ui.treeViewProperties->expandToDepth(1);
   ui.treeViewProperties->resizeColumnToContents(0);
   ui.treeViewProperties->resizeColumnToContents(1);
}

void QVariantListPropertyItemEditor::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
   if(m_currentlySelected.isValid())
   {
      m_values[m_currentlySelected.row()] = m_variantHolder->value();
      m_variantModel->setData(m_currentlySelected , m_values[m_currentlySelected.row()] , Qt::UserRole);
      m_variantModel->setData(m_currentlySelected , QString(m_values[m_currentlySelected.row()].typeName()) , Qt::DisplayRole);
   }
}


void QVariantListPropertyItemEditor::onAddQVariant()
{
   if(m_values.length())
   {
      int type = m_values[0].userType();
      m_values.append(QVariant((QVariant::Type) type ));
   }
   else
   {
      m_values.append(QVariant());
   }

   setUpChildWidget();
}

void QVariantListPropertyItemEditor::onRemoveQVariant()
{
   QModelIndexList selectedItems = ui.listViewItems->selectionModel()->selectedRows();

   while (selectedItems.count() > 0)
   {
      m_variantModel->removeRow(selectedItems[0].row());
      m_values.removeAt(selectedItems[0].row());
      selectedItems = ui.listViewItems->selectionModel()->selectedRows();
   }
}
