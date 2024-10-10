#include "add_marker_dialog.hpp"
#include "ui_add_marker_dialog.h"

#include <scwx/qt/types/marker_types.hpp>
#include <scwx/util/logger.hpp>


#include <string>
#include <QObject>


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

};

AddMarkerDialog::AddMarkerDialog(QWidget* parent) :
   QDialog(parent),
   p {std::make_unique<Impl>()},
   ui(new Ui::AddMarkerDialog)
{
   ui->setupUi(this);
}

AddMarkerDialog::~AddMarkerDialog()
{
   delete ui;
}

void AddMarkerDialog::setup(double latitude, double longitude)
{
   ui->nameLineEdit->setText("");
   ui->latitudeDoubleSpinBox->setValue(latitude);
   ui->longitudeDoubleSpinBox->setValue(longitude);
}

types::MarkerInfo AddMarkerDialog::get_marker_info() const
{
   return types::MarkerInfo(ui->nameLineEdit->text().toStdString(),
                            ui->latitudeDoubleSpinBox->value(),
                            ui->longitudeDoubleSpinBox->value());
}


} // namespace ui
} // namespace qt
} // namespace scwx
