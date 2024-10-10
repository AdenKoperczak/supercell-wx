#pragma once
#include <scwx/qt/types/marker_types.hpp>

#include <QDialog>

namespace Ui
{
class AddMarkerDialog;
}

namespace scwx
{
namespace qt
{
namespace ui
{
class AddMarkerDialog : public QDialog
{
   Q_OBJECT
   Q_DISABLE_COPY_MOVE(AddMarkerDialog)

public:
   explicit AddMarkerDialog(QWidget* parent = nullptr);
   ~AddMarkerDialog();

   void setup(double latitude, double longitude);

   types::MarkerInfo get_marker_info() const;

private:
   class Impl;
   std::unique_ptr<Impl> p;
   Ui::AddMarkerDialog*   ui;
};


} // namespace ui
} // namespace qt
} // namespace scwx
