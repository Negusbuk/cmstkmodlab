
#ifndef _DEFOIMAGELABEL_H
#define _DEFOIMAGELABEL_H

#include <iostream>
#include <utility>

#include <QLabel>
//#include <QGraphicsView>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QPalette>
#include <QStylePainter>
#include <QStyleOptionFocusRect>

#include "DefoPoint.h"
#include "DefoArea.h"
#include "DefoConfigReader.h"
#include "DefoHistogramView.h"



///
/// rubberband with a different color,
/// for displaying selected areas
///
class DefoImageLabelAreaRubberBand : public QRubberBand {

  Q_OBJECT

 public:
  DefoImageLabelAreaRubberBand( Shape s, QWidget* p = 0 ) : QRubberBand( s, p ) {}
  void setName( QString name ) { name_ = name; }
  QString& getName( void ) { return name_; }

 private:
  QString name_;

 protected:

  virtual void paintEvent(QPaintEvent *pe ) {
    QRegion reg( pe->rect() );
    setMask( reg ); // need to replace the mask to fill the entire rect
    QPainter painter(this);
    QPen pen( Qt::yellow, 2 );
    pen.setStyle( Qt::DashLine );
    painter.setPen( pen );
    painter.drawRect( pe->rect() );
    painter.drawText( 3,13, name_ );
  }

  
  
};



// ///
// ///
// ///
// class DefoImageLabelIndexRubberBand : public QRubberBand {
  
//  Q_OBJECT
    
//  public:
//   DefoImageLabelIndexRubberBand( Shape s, QWidget* p = 0 ) : QRubberBand( s, p ) {}
//   void setIndex( std::pair<int,int> i ) { index_ = i; };
    
//  private:
//   std::pair<int,int> index_;

//  protected:
  
//   virtual void paintEvent(QPaintEvent *pe ) {
//     //    Q_UNUSED( pe );
//     QRegion reg( pe->rect() );
//     setMask( reg ); // need to replace the mask to fill the entire rect
//     QPainter painter(this);
//     QPen pen( Qt::magenta, 2 );
//     //    pen.setStyle( Qt::NoPen );
//     painter.setPen( pen );
//     //painter.drawRect( pe->rect() ); //////////////////////
//     QFont font;
//     font.setPointSize( 6 );
//     painter.setFont( font );
//     painter.drawText( 0, 22, QString::number( index_.first ) + "," + QString::number( index_.second ) );
//   }

// };



///
/// qlabel for displaying images
/// with special qrubberband support
///
class DefoImageLabel : public QLabel {

  Q_OBJECT

 public:
  explicit DefoImageLabel( QWidget * parent=0, Qt::WindowFlags f=0 ) : QLabel( parent, f ) { init(); }
  explicit DefoImageLabel( const QString & text, QWidget * parent=0, Qt::WindowFlags f=0 ) : QLabel( text, parent, f ) { init(); }
  ~DefoImageLabel() {}
  void setRotation( bool rot ) { isRotation_ = rot; }
  void displayImageToSize( QImage& );

 public slots:
  void defineArea( void );
  void displayAreas( bool );
  void refreshAreas( std::vector<DefoArea> area );
  void displayIndices( bool );
  void refreshIndices( std::vector<DefoPoint> points );
  void showHistogram( void );

 signals:
  void areaDefined( DefoArea area );
  
 private:
  virtual void paintEvent(QPaintEvent* );
  void mousePressEvent( QMouseEvent* );
  void mouseMoveEvent( QMouseEvent* );
  void mouseReleaseEvent( QMouseEvent* );
  void init( void );
  void createAndSendArea( void );
  void transformToOriginal( QRect& );
  void transformToLocal( QRect& );

  QImage origImage_;
  DefoImageLabelAreaRubberBand *rubberBand_;
  std::vector<DefoImageLabelAreaRubberBand*> areaRubberBands_;
  std::vector<DefoPoint> indexPoints_;
  QPoint myPoint_;
  DefoHistogramView view_;
  bool isView_;
  bool isRotation_; // rotation by -90 deg?
  bool isDefineArea_;
  QSize originalImageSize_;
  unsigned int debugLevel_;
  bool isDisplayAreas_;
  bool isDisplayIndices_;

};
#endif
