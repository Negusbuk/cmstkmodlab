/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <AssemblyObjectFinderPatRecWidget.h>
#include <AssemblyUtilities.h>

#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QPixmap>
#include <QFileDialog>

AssemblyObjectFinderPatRecWidget::AssemblyObjectFinderPatRecWidget(QWidget* parent) :
  QWidget(parent),

  layout_(nullptr),

  templa_load_button_(nullptr),
  templa_file_linee_ (nullptr),

  thresh_thresh_radbu_(nullptr),
  thresh_thresh_linee_(nullptr),

  thresh_adathr_radbu_(nullptr),
  thresh_adathr_linee_(nullptr),

  angles_prescan_label_(nullptr),
  angles_prescan_linee_(nullptr),

  angles_finemax_label_(nullptr),
  angles_finemax_linee_(nullptr),

  angles_finestep_label_(nullptr),
  angles_finestep_linee_(nullptr)
{
  layout_ = new QVBoxLayout;
  this->setLayout(layout_);

  /// Template Image ---------------
  QGroupBox* templa_box = new QGroupBox(tr("Template Image"));
  templa_box->setStyleSheet("QGroupBox { font-weight: bold; } ");

  QHBoxLayout* templa_lay = new QHBoxLayout;

  templa_load_button_ = new QPushButton(tr("Load Image"));

  connect(templa_load_button_, SIGNAL(clicked()), this, SLOT(load_image_template()));

  templa_file_linee_ = new QLineEdit(tr(""));
  templa_file_linee_->setReadOnly(true);

  connect(this, SIGNAL(updated_image_template_path(QString)), templa_file_linee_, SLOT(setText(QString)));

  templa_lay->addWidget(templa_load_button_, 34);
  templa_lay->addWidget(templa_file_linee_ , 66);

  templa_box->setLayout(templa_lay);

  layout_->addWidget(templa_box);
  /// ------------------------------

  /// Master Image Thresholding ----
  QGroupBox* thresh_box = new QGroupBox(tr("Master Image Thresholding"));
  thresh_box->setStyleSheet("QGroupBox { font-weight: bold; } ");

  QGridLayout* thresh_lay = new QGridLayout;

  thresh_thresh_radbu_ = new QRadioButton(tr("Threshold (pos int)"));
  thresh_thresh_linee_ = new QLineEdit(tr(""));

  thresh_adathr_radbu_ = new QRadioButton(tr("Adaptive Threshold (pos odd int)"));
  thresh_adathr_linee_ = new QLineEdit(tr(""));

  connect(thresh_thresh_radbu_, SIGNAL(toggled(bool)), thresh_thresh_linee_, SLOT(setEnabled(bool)));
  connect(thresh_adathr_radbu_, SIGNAL(toggled(bool)), thresh_adathr_linee_, SLOT(setEnabled(bool)));

  thresh_thresh_radbu_->setChecked(true);
  thresh_thresh_linee_->setEnabled(true);

  thresh_adathr_radbu_->setChecked(false);
  thresh_adathr_linee_->setEnabled(false);

  thresh_lay->addWidget(thresh_thresh_radbu_, 0, 0);
  thresh_lay->addWidget(thresh_thresh_linee_, 0, 1);

  thresh_lay->addWidget(thresh_adathr_radbu_, 1, 0);
  thresh_lay->addWidget(thresh_adathr_linee_, 1, 1);

  thresh_box->setLayout(thresh_lay);

  layout_->addWidget(thresh_box);
  /// ------------------------------

  /// Template-Matching Angular Scan
  QGroupBox* angles_box = new QGroupBox(tr("Template-Matching Angular Scan"));
  angles_box->setStyleSheet("QGroupBox { font-weight: bold; } ");

  QGridLayout* angles_lay = new QGridLayout;

  angles_prescan_label_ = new QLabel(tr("Pre-Scan Angles (list) [deg]"));
  angles_prescan_linee_ = new QLineEdit(tr(""));

  angles_finemax_label_ = new QLabel(tr("Fine-Scan Maximum Angle [deg]"));
  angles_finemax_linee_ = new QLineEdit(tr(""));

  angles_finestep_label_ = new QLabel(tr("Fine-Scan Angular Step [deg]"));
  angles_finestep_linee_ = new QLineEdit(tr(""));

  angles_lay->addWidget(angles_prescan_label_ , 0, 0);
  angles_lay->addWidget(angles_prescan_linee_ , 0, 1);

  angles_lay->addWidget(angles_finemax_label_ , 1, 0);
  angles_lay->addWidget(angles_finemax_linee_ , 1, 1);

  angles_lay->addWidget(angles_finestep_label_, 2, 0);
  angles_lay->addWidget(angles_finestep_linee_, 2, 1);

  angles_box->setLayout(angles_lay);

  layout_->addWidget(angles_box);
  /// ------------------------------
}

void AssemblyObjectFinderPatRecWidget::load_image_template()
{
  const QString fpath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromStdString(Config::CMSTkModLabBasePath+"/share/assembly"), tr("PNG Files (*.png);;All Files (*)"));

  this->load_image_template_from_path(fpath);

  return;
}

void AssemblyObjectFinderPatRecWidget::load_image_template_from_path(const QString& filename)
{
  if(filename.isNull() || filename.isEmpty()){ return; }

  const cv::Mat img = assembly::cv_imread(filename, CV_LOAD_IMAGE_COLOR);

  if(img.empty())
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Warning) << "load_image_template"
       << ": input image is empty, no action taken";

    return;
  }

  NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Spam) << "load_image_template"
     << ": emitting signal \"updated_image_template_path(" << filename << ")\"";

  emit updated_image_template_path(filename);

  NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Spam) << "load_image_template"
     << ": emitting signal \"updated_image_template\"";

  emit updated_image_template(img);
}

void AssemblyObjectFinderPatRecWidget::transmit_configuration()
{
  bool valid_conf(false);

  const auto& conf = this->get_configuration(valid_conf);

  if(valid_conf == false)
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "transmit_configuration"
       << ": invalid AssemblyObjectFinderPatRec::Configuration object, no action taken";

    return;
  }

  NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Spam) << "transmit_configuration"
     << ": emitting signal \"configuration(AssemblyObjectFinderPatRec::Configuration)\"";

  emit configuration(conf);
}

AssemblyObjectFinderPatRec::Configuration AssemblyObjectFinderPatRecWidget::get_configuration(bool& valid_conf) const
{
  AssemblyObjectFinderPatRec::Configuration conf;

  /// Template Image ---------------
  conf.template_filepath_ = templa_file_linee_->text();
  /// ------------------------------

  /// Master Image Thresholding ----
  if(thresh_thresh_radbu_->isChecked() == thresh_adathr_radbu_->isChecked())
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "get_configuration"
       << ": invalid configuration for master-image thresholding (selected zero or multiple options), no action taken";

    valid_conf = false;

    conf.reset();

    return conf;
  }
  else if(thresh_thresh_radbu_->isChecked())
  {
    conf.thresholding_useAdaptiveThreshold_ = false;
    conf.thresholding_blocksize_ = -1;

    conf.thresholding_useThreshold_ = true;

    const QString thresh_str = thresh_thresh_linee_->text().remove(" ");

    bool valid_thr(false);
    conf.thresholding_threshold_ = thresh_str.toInt(&valid_thr);

    if(valid_thr == false)
    {
      NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "get_configuration"
         << ": invalid format for threshold value (" << thresh_str << ", not a integer), no action taken";

      valid_conf = false;

      conf.reset();

      return conf;
    }
  }
  else if(thresh_adathr_radbu_->isChecked())
  {
    conf.thresholding_useThreshold_ = false;
    conf.thresholding_threshold_ = -1;

    conf.thresholding_useAdaptiveThreshold_ = true;

    const QString adathr_str = thresh_adathr_linee_->text().remove(" ");

    bool valid_bks(false);
    conf.thresholding_blocksize_ = adathr_str.toInt(&valid_bks);

    if(valid_bks == false)
    {
      NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "get_configuration"
         << ": invalid format for block-size value (" << adathr_str << ", not a integer), no action taken";

      valid_conf = false;

      conf.reset();

      return conf;
    }
  }
  /// ------------------------------

  /// Template-Matching Angular Scan
  const QStringList angles_prescan_vstr = angles_prescan_linee_->text().split(" ");

  conf.angles_prescan_vec_.clear();

  for(const auto& ang_str : angles_prescan_vstr)
  {
    if(ang_str.isEmpty()){ continue; }

    bool valid_ang(false);
    const double ang_val = ang_str.toDouble(&valid_ang);

    if(valid_ang == false)
    {
      NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "get_configuration"
         << ": invalid format for pre-scan angle value (" << ang_str << ", not a double), no action taken";

      valid_conf = false;

      conf.reset();

      return conf;
    }

    conf.angles_prescan_vec_.emplace_back(ang_val);
  }

  const QString angfinemax_str = angles_finemax_linee_->text().remove(" ");

  bool valid_afm(false);
  conf.angles_finemax_ = angfinemax_str.toDouble(&valid_afm);

  if(valid_afm == false)
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "get_configuration"
       << ": invalid format for fine-scan max-angle value (" << angfinemax_str << ", not a double), no action taken";

    valid_conf = false;

    conf.reset();

    return conf;
  }

  const QString angfinestep_str = angles_finestep_linee_->text().remove(" ");

  bool valid_afs(false);
  conf.angles_finestep_ = angfinestep_str.toDouble(&valid_afs);

  if(valid_afs == false)
  {
    NQLog("AssemblyObjectFinderPatRecWidget", NQLog::Critical) << "get_configuration"
       << ": invalid format for fine-scan step-angle value (" << angfinestep_str << ", not a double), no action taken";

    valid_conf = false;

    conf.reset();

    return conf;
  }
  /// ------------------------------

  valid_conf = true;

  return conf;
}
