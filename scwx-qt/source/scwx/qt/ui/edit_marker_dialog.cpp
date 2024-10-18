#include "edit_marker_dialog.hpp"
#include "ui_edit_marker_dialog.h"

#include <scwx/qt/manager/marker_manager.hpp>
#include <scwx/qt/types/marker_types.hpp>
#include <scwx/qt/util/color.hpp>
#include <scwx/util/logger.hpp>

#include <string>
#include <vector>

#include <QObject>
#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QColorDialog>


namespace scwx
{
namespace qt
{
namespace ui
{

static const std::string logPrefix_ = "scwx::qt::ui::edit_marker_dialog";
static const auto        logger_    = scwx::util::Logger::Create(logPrefix_);

class EditMarkerDialog::Impl
{
public:
   explicit Impl(EditMarkerDialog* self) :
      self_{self}
   {
   }

   void show_color_dialog();
   void set_icon_color(const std::string& color);

   void connect_signals();

   void handle_accepted();
   void handle_rejected();

   EditMarkerDialog* self_;

   std::shared_ptr<manager::MarkerManager> markerManager_ =
      manager::MarkerManager::Instance();
   std::vector<QIcon> icons_ = {};
   size_t editIndex_;
   bool adding_;
};

EditMarkerDialog::EditMarkerDialog(QWidget* parent) :
   QDialog(parent),
   p {std::make_unique<Impl>(this)},
   ui(new Ui::EditMarkerDialog)
{
   ui->setupUi(this);

   auto& icons = types::getMarkerIcons();
   p->icons_.reserve(icons.size());
   for (auto& markerIcon : icons)
   {
      size_t index = p->icons_.size();
      if (markerIcon.multicolored)
      {
         p->icons_.emplace_back(QString::fromStdString(markerIcon.path));
      }
      else
      {
         QPixmap pixmap = QPixmap(QString::fromStdString(markerIcon.path));
         QBitmap mask =
            pixmap.createMaskFromColor(QColor("white"), Qt::MaskOutColor);
         pixmap.fill(QWidget::palette().color(QWidget::foregroundRole()));
         pixmap.setMask(mask);

         p->icons_.emplace_back(pixmap);
      }

      ui->iconComboBox->addItem(p->icons_[index],
                                QString(""),
                                QString::fromStdString(markerIcon.name));
   }
   p->connect_signals();
}

EditMarkerDialog::~EditMarkerDialog()
{
   delete ui;
}

void EditMarkerDialog::setup()
{
   p->editIndex_ = p->markerManager_->marker_count();
   ui->iconComboBox->setCurrentIndex(0);
   p->markerManager_->add_marker(types::MarkerInfo(
      "",
      ui->iconComboBox->currentData().toString().toStdString(),
      0,
      0,
      boost::gil::rgba8_pixel_t(255, 255, 255, 255)));
   setup(p->editIndex_);
   p->adding_ = true;
}

void EditMarkerDialog::setup(double latitude, double longitude)
{
   p->editIndex_ = p->markerManager_->marker_count();
   ui->iconComboBox->setCurrentIndex(0);
   p->markerManager_->add_marker(types::MarkerInfo(
      "",
      ui->iconComboBox->currentData().toString().toStdString(),
      latitude,
      longitude,
      boost::gil::rgba8_pixel_t(255, 255, 255, 255)));

   setup(p->editIndex_);
   p->adding_ = true;
}

void EditMarkerDialog::setup(size_t index)
{
   std::optional<types::MarkerInfo> marker =
      p->markerManager_->get_marker(index);
   if (!marker)
   {
      return;
   }

   p->editIndex_ = index;
   p->adding_ = false;

   int iconIndex =
      ui->iconComboBox->findData(QString::fromStdString(marker->iconName));
   if (iconIndex < 0 || marker->iconName == "")
   {
      iconIndex = 0;
   }

   std::string iconColorStr = util::color::ToArgbString(marker->iconColor);

   ui->nameLineEdit->setText(QString::fromStdString(marker->name));
   ui->iconComboBox->setCurrentIndex(iconIndex);
   ui->latitudeDoubleSpinBox->setValue(marker->latitude);
   ui->longitudeDoubleSpinBox->setValue(marker->longitude);
   ui->iconColorLineEdit->setText(QString::fromStdString(iconColorStr));

   p->set_icon_color(iconColorStr);
}

types::MarkerInfo EditMarkerDialog::get_marker_info() const
{
   QString colorName = ui->iconColorLineEdit->text();
   boost::gil::rgba8_pixel_t color =
      util::color::ToRgba8PixelT(colorName.toStdString());

   return types::MarkerInfo(
      ui->nameLineEdit->text().toStdString(),
      ui->iconComboBox->currentData().toString().toStdString(),
      ui->latitudeDoubleSpinBox->value(),
      ui->longitudeDoubleSpinBox->value(),
      color);
}

void EditMarkerDialog::Impl::show_color_dialog()
{

   QColorDialog* dialog = new QColorDialog(self_);

   dialog->setAttribute(Qt::WA_DeleteOnClose);
   dialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);

   QColor initialColor(self_->ui->iconColorLineEdit->text());
   if (initialColor.isValid())
   {
      dialog->setCurrentColor(initialColor);
   }

   QObject::connect(dialog,
                    &QColorDialog::colorSelected,
                    self_,
                    [this](const QColor& qColor)
                    {
                       std::optional<types::MarkerInfo> marker =
                          markerManager_->get_marker(editIndex_);
                       if (!marker)
                       {
                          return;
                       }

                       QString colorName =
                          qColor.name(QColor::NameFormat::HexArgb);
                       self_->ui->iconColorLineEdit->setText(colorName);
                    });
   dialog->open();
}

void EditMarkerDialog::Impl::connect_signals()
{
   connect(self_,
           &EditMarkerDialog::accepted,
           self_,
           [this]() { handle_accepted(); });

   connect(self_,
           &EditMarkerDialog::rejected,
           self_,
           [this]() { handle_rejected(); });

   connect(self_->ui->iconColorLineEdit,
           &QLineEdit::textEdited,
           self_,
           [=, this](const QString& text) { set_icon_color(text.toStdString()); });

   connect(self_->ui->iconColorButton,
           &QAbstractButton::clicked,
           self_,
           [=, this]() { self_->p->show_color_dialog(); });
}

void EditMarkerDialog::Impl::set_icon_color(const std::string& color)
{
   self_->ui->iconColorFrame->setStyleSheet(
      QString::fromStdString(fmt::format("background-color: {}", color)));
}

void EditMarkerDialog::Impl::handle_accepted()
{
   markerManager_->set_marker(editIndex_, self_->get_marker_info());
}

void EditMarkerDialog::Impl::handle_rejected()
{
   if (adding_)
   {
      markerManager_->remove_marker(editIndex_);
   }
}



} // namespace ui
} // namespace qt
} // namespace scwx
