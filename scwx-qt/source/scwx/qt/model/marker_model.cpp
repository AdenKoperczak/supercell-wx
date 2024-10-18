#include <scwx/common/geographic.hpp>
#include <scwx/qt/model/marker_model.hpp>
#include <scwx/qt/manager/marker_manager.hpp>
#include <scwx/qt/types/marker_types.hpp>
#include <scwx/qt/types/qt_types.hpp>
#include <scwx/util/logger.hpp>

#include <QApplication>
#include <QAbstractItemView>

namespace scwx
{
namespace qt
{
namespace model
{

static const std::string logPrefix_ = "scwx::qt::model::marker_model";
static const auto        logger_    = scwx::util::Logger::Create(logPrefix_);

static constexpr int kFirstColumn =
   static_cast<int>(MarkerModel::Column::Latitude);
static constexpr int kLastColumn =
   static_cast<int>(MarkerModel::Column::Name);
static constexpr int kNumColumns = kLastColumn - kFirstColumn + 1;

class MarkerModel::Impl
{
public:
   explicit Impl() {}
   ~Impl() = default;
   std::shared_ptr<manager::MarkerManager> markerManager_ {
      manager::MarkerManager::Instance()};
};

MarkerModel::MarkerModel(QObject* parent) :
   QAbstractTableModel(parent), p(std::make_unique<Impl>())
{

   connect(p->markerManager_.get(),
         &manager::MarkerManager::MarkersInitialized,
         this,
         &MarkerModel::HandleMarkersInitialized);

   connect(p->markerManager_.get(),
         &manager::MarkerManager::MarkerAdded,
         this,
         &MarkerModel::HandleMarkerAdded);

   connect(p->markerManager_.get(),
         &manager::MarkerManager::MarkerChanged,
         this,
         &MarkerModel::HandleMarkerChanged);

   connect(p->markerManager_.get(),
         &manager::MarkerManager::MarkerRemoved,
         this,
         &MarkerModel::HandleMarkerRemoved);
}



MarkerModel::~MarkerModel() = default;

int MarkerModel::rowCount(const QModelIndex& parent) const
{
   return parent.isValid() ?
             0 :
             static_cast<int>(p->markerManager_->marker_count());
}

int MarkerModel::columnCount(const QModelIndex& parent) const
{
   return parent.isValid() ? 0 : kNumColumns;
}

Qt::ItemFlags MarkerModel::flags(const QModelIndex& index) const
{
   Qt::ItemFlags flags = QAbstractTableModel::flags(index);

   switch (index.column())
   {
   case static_cast<int>(Column::Name):
   case static_cast<int>(Column::Latitude):
   case static_cast<int>(Column::Longitude):
      flags |= Qt::ItemFlag::ItemIsEditable;
      break;
   default:
      break;
   }

   return flags;
}

QVariant MarkerModel::data(const QModelIndex& index, int role) const
{

   if (!index.isValid() || index.row() < 0)
   {
      return QVariant();
   }

   std::optional<types::MarkerInfo> markerInfo =
      p->markerManager_->get_marker(index.row());
   if (!markerInfo)
   {
      return QVariant();
   }

   switch(index.column())
   {
   case static_cast<int>(Column::Name):
      if (role == Qt::ItemDataRole::DisplayRole ||
          role == Qt::ItemDataRole::ToolTipRole)
      {
         return QString::fromStdString(markerInfo->name);
      }
      break;

   case static_cast<int>(Column::Latitude):
      if (role == Qt::ItemDataRole::DisplayRole ||
          role == Qt::ItemDataRole::ToolTipRole)
      {
         return QString::fromStdString(
            common::GetLatitudeString(markerInfo->latitude));
      }
      break;

   case static_cast<int>(Column::Longitude):
      if (role == Qt::ItemDataRole::DisplayRole ||
          role == Qt::ItemDataRole::ToolTipRole)
      {
         return QString::fromStdString(
            common::GetLongitudeString(markerInfo->longitude));
      }
      break;

   default:
      break;
   }

   return QVariant();
}

QVariant MarkerModel::headerData(int             section,
                                 Qt::Orientation orientation,
                                 int             role) const
{
   if (role == Qt::ItemDataRole::DisplayRole)
   {
      if (orientation == Qt::Horizontal)
      {
         switch (section)
         {
            case static_cast<int>(Column::Name):
               return tr("Name");

            case static_cast<int>(Column::Latitude):
               return tr("Latitude");

            case static_cast<int>(Column::Longitude):
               return tr("Longitude");

            default:
               break;
         }
      }
   }

   return QVariant();
}

bool MarkerModel::setData(const QModelIndex& index,
                          const QVariant&    value,
                          int                role)
{
   Q_UNUSED(index);
   Q_UNUSED(value);
   Q_UNUSED(role);

   return false;
}

void MarkerModel::HandleMarkersInitialized(size_t count)
{
   const int index = static_cast<int>(count - 1);

   beginInsertRows(QModelIndex(), 0, index);
   endInsertRows();
}

void MarkerModel::HandleMarkerAdded()
{
   const int newIndex = static_cast<int>(p->markerManager_->marker_count() - 1);

   beginInsertRows(QModelIndex(), newIndex, newIndex);
   endInsertRows();
}

void MarkerModel::HandleMarkerChanged(size_t index)
{
   const int changedIndex = static_cast<int>(index);
   QModelIndex topLeft = createIndex(changedIndex, kFirstColumn);
   QModelIndex bottomRight = createIndex(changedIndex, kLastColumn);

   Q_EMIT dataChanged(topLeft, bottomRight);
}

void MarkerModel::HandleMarkerRemoved(size_t index)
{
   const int removedIndex = static_cast<int>(index);

   beginRemoveRows(QModelIndex(), removedIndex, removedIndex);
   endRemoveRows();
}


} // namespace model
} // namespace qt
} // namespace scwx
