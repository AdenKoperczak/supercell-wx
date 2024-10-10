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

   void ConnectSignals(AddMarkerDialog* self);

   QLineEdit* nameLineEdit_ {nullptr};
   std::string name_;
   double latitude_;
   double longitude_;

};

void AddMarkerDialog::Impl::ConnectSignals(AddMarkerDialog* self)
{
   QObject::connect(nameLineEdit_,
                    &QLineEdit::textEdited,
                    self,
                    [=, this](const QString& text)
                    { name_ = text.toStdString(); });
}

AddMarkerDialog::AddMarkerDialog(QWidget* parent) :
   QDialog(parent),
   p {std::make_unique<Impl>()},
   ui(new Ui::AddMarkerDialog)
{
   ui->setupUi(this);

   p->nameLineEdit_ = ui->nameLineEdit;
   p->ConnectSignals(this);
}

AddMarkerDialog::~AddMarkerDialog()
{
   delete ui;
}

void AddMarkerDialog::set_coordinate(double latitude, double longitude)
{
   p->latitude_ = latitude;
   p->longitude_ = longitude;
}

types::MarkerInfo AddMarkerDialog::get_marker_info() const
{
   return types::MarkerInfo(p->name_, p->latitude_, p->longitude_);
}


} // namespace ui
} // namespace qt
} // namespace scwx
