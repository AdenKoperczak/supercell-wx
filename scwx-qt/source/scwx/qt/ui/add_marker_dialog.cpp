#include "add_marker_dialog.hpp"
#include "ui_add_marker_dialog.h"

#include <scwx/qt/types/marker_types.hpp>
#include <scwx/util/logger.hpp>

#include <string>
#include <vector>

#include <QObject>
#include <QString>
#include <QIcon>


namespace scwx
{
namespace qt
{
namespace ui
{

static const std::string logPrefix_ = "scwx::qt::ui::add_marker_dialog";
static const auto        logger_    = scwx::util::Logger::Create(logPrefix_);

class AddMarkerDialog::Impl
{
public:
   explicit Impl()
   {
   }

   std::vector<QIcon> icons_ = {};
};

AddMarkerDialog::AddMarkerDialog(QWidget* parent) :
   QDialog(parent),
   p {std::make_unique<Impl>()},
   ui(new Ui::AddMarkerDialog)
{
   ui->setupUi(this);

   auto& icons = types::getMarkerIcons();
   p->icons_.reserve(icons.size());
   for (auto& markerIcon : icons)
   {
      size_t index = p->icons_.size();
      p->icons_.emplace_back(QString::fromStdString(markerIcon.path));
      ui->iconComboBox->addItem(p->icons_[index],
                                QString(""),
                                QString::fromStdString(markerIcon.name));
   }
}

AddMarkerDialog::~AddMarkerDialog()
{
   delete ui;
}

void AddMarkerDialog::setup(double latitude, double longitude)
{
   ui->nameLineEdit->setText("");
   ui->iconComboBox->setCurrentIndex(0);
   ui->latitudeDoubleSpinBox->setValue(latitude);
   ui->longitudeDoubleSpinBox->setValue(longitude);
}

types::MarkerInfo AddMarkerDialog::get_marker_info() const
{
   return types::MarkerInfo(
      ui->nameLineEdit->text().toStdString(),
      ui->iconComboBox->currentData().toString().toStdString(),
      ui->latitudeDoubleSpinBox->value(),
      ui->longitudeDoubleSpinBox->value());
}

} // namespace ui
} // namespace qt
} // namespace scwx
