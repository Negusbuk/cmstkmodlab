/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QGroupBox>
#include <QVBoxLayout>

#include "Thermo2ScriptWidget.h"

Thermo2ScriptWidget::Thermo2ScriptWidget(const QString& title,
                                         Thermo2ScriptModel* scriptModel,
                                         QWidget *parent)
  : QGroupBox(title, parent),
    scriptModel_(scriptModel)
{
  // Layouts to put everything into place
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);
  //setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QWidget *buttons;
  QHBoxLayout *buttonLayout;

  // script buttons
  buttonLayout = new QHBoxLayout();
  buttons = new QWidget(this);
  //buttons_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  buttons->setLayout(buttonLayout);
  layout->addWidget(buttons);

  // load a script
  openScriptButton_ = new QPushButton("&Open", buttons);
  connect(openScriptButton_, SIGNAL(clicked()), this, SLOT(openScriptButtonClicked()));
  buttonLayout->addWidget(openScriptButton_);

  // save a script
  saveScriptButton_ = new QPushButton("&Save", buttons);
  connect(saveScriptButton_, SIGNAL(clicked()), this, SLOT(saveScriptButtonClicked()));
  buttonLayout->addWidget(saveScriptButton_);

  // save a script with new name
  saveAsScriptButton_ = new QPushButton("&Save as", buttons);
  connect(saveAsScriptButton_, SIGNAL(clicked()), this, SLOT(saveAsScriptButtonClicked()));
  buttonLayout->addWidget(saveAsScriptButton_);

  // buttonLayout->addSpacing(100);

  // save a script
  executeScriptButton_ = new QPushButton("&Execute", buttons);
  connect(executeScriptButton_, SIGNAL(clicked()), this, SLOT(executeScriptButtonClicked()));
  buttonLayout->addWidget(executeScriptButton_);

  // execute script
  abortScriptButton_ = new QPushButton("&Abort", buttons);
  connect(abortScriptButton_, SIGNAL(clicked()), this, SLOT(abortScriptButtonClicked()));
  buttonLayout->addWidget(abortScriptButton_);

  // script buttons
  buttonLayout = new QHBoxLayout();
  buttons = new QWidget(this);
  //buttons_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  buttons->setLayout(buttonLayout);
  layout->addWidget(buttons);

  scriptSnippets_ = new Thermo2ScriptSnippets(buttons);
  buttonLayout->addWidget(scriptSnippets_);

  // execute script
  insertSnippetButton_ = new QPushButton("&Insert snippet", buttons);
  connect(insertSnippetButton_, SIGNAL(clicked()), this, SLOT(insertSnippetButtonClicked()));
  buttonLayout->addWidget(insertSnippetButton_);

  // script editor
  scriptEditor_ = new Thermo2ScriptEdit(this);
  scriptEditor_->setSizePolicy(QSizePolicy::MinimumExpanding,
                               QSizePolicy::MinimumExpanding);
  scriptEditor_->setMinimumWidth(600);
  scriptEditor_->setMinimumHeight(150);
  scriptEditor_->setDocument(scriptModel_->scriptDocument());
  scriptEditor_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scriptEditor_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  layout->addWidget(scriptEditor_, 1);

  connect(scriptModel_->scriptDocument(), SIGNAL(contentsChanged()),
          this, SLOT(scriptChanged()));

  // message display
  messageDisplay_ = new Thermo2MessageDisplay(this);
  messageDisplay_->setMinimumWidth(600);
  messageDisplay_->setMinimumHeight(100);
  messageDisplay_->setMaximumHeight(100);
  messageDisplay_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  messageDisplay_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  messageDisplay_->setReadOnly(true);
  layout->addWidget(messageDisplay_, 2);

  updateGeometry();

  connect(scriptModel_, SIGNAL(clearMessageText()),
          this, SLOT(clearMessageText()));
  connect(scriptModel_, SIGNAL(appendMessageText(const QString &)),
          this, SLOT(appendMessageText(const QString &)));
}

void Thermo2ScriptWidget::openScriptButtonClicked()
{
  QString filename = QFileDialog::getOpenFileName(this,
                                                  "open script",
                                                  "./",
                                                  "Thermo Scripts (*.tsr)",
                                                  0,
                                                  0);
  if (!filename.isNull()) scriptModel_->openScript(filename);
}

void Thermo2ScriptWidget::saveScriptButtonClicked()
{
  QString filename = scriptModel_->currentScriptFilename();
  if (filename.isNull()) 
    this->saveAsScriptButtonClicked();
  else
    scriptModel_->saveScript(filename);
}

void Thermo2ScriptWidget::saveAsScriptButtonClicked()
{
  QString filename = QFileDialog::getSaveFileName(this,
                                                  "save script",
                                                  "./",
                                                  "Thermo Scripts (*.tsr)",
                                                  0,
                                                  0);
  if (filename.isNull()) return;
  if (!filename.endsWith(".tsr")) filename += ".tsr";
  scriptModel_->saveScript(filename);
}

void Thermo2ScriptWidget::executeScriptButtonClicked()
{
  scriptModel_->executeScript();
}

void Thermo2ScriptWidget::abortScriptButtonClicked()
{
  scriptModel_->abortScript();
}

void Thermo2ScriptWidget::scriptChanged()
{

}

void Thermo2ScriptWidget::insertSnippetButtonClicked()
{
  scriptEditor_->moveCursor(QTextCursor::StartOfLine);

  QString snippet = scriptSnippets_->currentData().toString();
  scriptEditor_->insertPlainText(snippet);
}

void Thermo2ScriptWidget::clearMessageText( )
{
  messageDisplay_->setPlainText("");
}

void Thermo2ScriptWidget::appendMessageText(const QString & text)
{
  messageDisplay_->appendPlainText(text);
}
