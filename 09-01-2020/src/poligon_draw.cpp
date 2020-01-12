#include <QApplication>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QRegion>
#include <QSlider>
#include <QSpinBox>
#include <QTimerEvent>
#include <QWidget>
#include <QtGui>
#include <iostream>
#include <pqxx/pqxx>
#include <vector>

#include "request.cpp"
using namespace std;

class PaintPOLIGON : public QWidget, QWindow {
 public:
  int TimeTrajectory = 0;

  vector<int> max_t = {};

  int maxTime = request_max();
  // int maxTime = 2000;
  int m_timerId;
  bool stop = false;
  int SpeedTime = 1;
  int spin_boxValue;

  PaintPOLIGON(QWidget* pwgt = nullptr) : QWidget(pwgt) {
    QPalette pall1;
    // pall1.setColor (this ->backgroundRole() , Qt::black) ;
    this->setPalette(pall1);
    this->setAutoFillBackground(false);
    m_timerId = QWidget::startTimer(100);
    auto main_layout = new QVBoxLayout;
    auto layoutButt = new QHBoxLayout;
    QWidget::setMinimumWidth(1600);
    setLayout(main_layout);
    auto param_layout = new QFormLayout;
    auto start = new QPushButton("Start");
    QObject::connect(start, &QPushButton::clicked, [this]() { Start(); });
    auto reset = new QPushButton("Reset");
    QObject::connect(reset, &QPushButton::clicked,
                     [this]() { TimeTrajectory = 0; });
    auto stop = new QPushButton("Stop");
    QObject::connect(stop, &QPushButton::clicked, [this]() { Stop(); });
    auto buttonH = new QHBoxLayout;
    auto buttonV = new QVBoxLayout;
    auto speedtime = new QSpinBox;
    auto spin_box = new QSpinBox;
    spin_box->setValue(TimeTrajectory);
    speedtime->setRange(1, 10);
    spin_box->setRange(0, maxTime);
    speedtime->setSingleStep(1);
    spin_box->setSingleStep(1);
    QSlider* slider = new QSlider(Qt::Vertical);
    slider->setRange(0, maxTime);
    speedtime->setValue(1);
    param_layout->addRow("TimeSpeed", speedtime);
    param_layout->addRow("Time, s", spin_box);
    param_layout->addRow(" ", slider);

    QObject::connect(spin_box, SIGNAL(valueChanged(int)), slider,
                     SLOT(setValue(int)));
    QObject::connect(slider, SIGNAL(valueChanged(int)), spin_box,
                     SLOT(setValue(int)));
    QWidget::connect(spin_box, QOverload<int>::of(&QSpinBox::valueChanged),
                     [this](int value) { setTime(value); });
    QWidget::connect(spin_box, QOverload<int>::of(&QSpinBox::valueChanged),
                     [this]() { this->repaint(); });
    QWidget::connect(speedtime, QOverload<int>::of(&QSpinBox::valueChanged),
                     [this](int value) { SetSpeedTime(value); });

    buttonV->addWidget(start, 1);
    buttonH->addStretch(10);
    buttonV->addWidget(stop, 1);
    buttonV->addStretch(10);
    buttonV->addWidget(reset, 1);
    buttonV->addStretch(10);
    layoutButt->addLayout(buttonH);
    layoutButt->addLayout(buttonV);
    layoutButt->addLayout(param_layout);
    main_layout->addLayout(layoutButt);
  }  // End construktor

  int request_max() {
    auto connection_string = "host=127.0.0.1 dbname=eparubets user=eparubets";
    pqxx::connection request(connection_string);
    request.prepare(
        "request_prepeared",
        "select MAX(object_point_time) AS object_point_time from object_point");

    pqxx::nontransaction work(request);
    pqxx::result res = work.exec_prepared("request_prepeared");
    for (const auto& i : res) {
      max_t.push_back(i[0].as<int>());
    }
    return max_t[0];
  }

  //  vector<vector<int>> request_track(int track_number, int time_point,
  //                                    vector<vector<int>>& result,
  //                                    vector<int>& x, vector<int>& y,
  //                                    vector<int>& time) {
  //    auto connection_string = "host=127.0.0.1 dbname=eparubets
  //    user=eparubets"; pqxx::connection request(connection_string);
  //    request.prepare("request_prepeared",
  //                    "select * from object_point WHERE object_point_time <=
  //                    $1 " "AND object_point_parrent =  $2;");
  //    pqxx::nontransaction work(request);
  //    pqxx::result res =
  //        work.exec_prepared("request_prepeared", time_point, track_number);
  //    for (const auto& i : res) {
  //      x.push_back(i[1].as<int>());
  //      y.push_back(i[2].as<int>());
  //      time.push_back(i[3].as<int>());
  //    }
  //    result.push_back(x);
  //    result.push_back(y);
  //    result.push_back(time);
  //    return result;
  //  }  // end function request_track

  void setTime(int t) { TimeTrajectory = t; }
  void Stop() { stop = true; }
  void Start() { stop = false; }
  void SetSpeedTime(int speed) {
    SpeedTime = speed;
    m_timerId = QWidget::startTimer(100 / SpeedTime);
  }

  // bool event(QEvent *event) override {
  // renderNow();
  // if (event->type() == QEvent::UpdateRequest) {
  // renderNow();
  // return true;}
  // return QWindow::event(event);
  //}
  // QTimer * ptimer      = new QTimer ();

  // QTimer *timer = new QTimer(this);
  // connect(timer, &QTimer::timeout, this,
  // QOverload<>::of(&AnalogClock::update)); timer->start(1000);

 signals:
  void timerEvent(QTimerEvent* event) override {
    if (event->timerId() == m_timerId) {
      if (!stop) {
        ++TimeTrajectory;
        emit repaint();
        emit spin_boxADD();
      }
    }
  }

  void exposeEvent(QExposeEvent*) override {
    if (isExposed()) renderNow();
  }

  void paintEvent(QPaintEvent*) override { renderNow(); }

  int spin_boxADD() { return TimeTrajectory; }

 public slots:

  //  void renderLater() { requestUpdate(); }
  void renderNow() {
    try {
      auto connection_string = "host=127.0.0.1 dbname=eparubets user=eparubets";
      pqxx::connection request3(connection_string);
      request3.prepare(
          "select_points_region1",
          "select * from region_point WHERE region_point_parrent = 1;");
      pqxx::connection request4(connection_string);
      request4.prepare(
          "select_points_region2",
          "select * from region_point WHERE region_point_parrent = 2;");
      pqxx::connection request5(connection_string);
      request5.prepare(
          "select_points_region3",
          "select * from region_point WHERE region_point_parrent = 3;");
      pqxx::nontransaction work3(request3);
      pqxx::nontransaction work4(request4);
      pqxx::nontransaction work5(request5);
      pqxx::result res3 = work3.exec_prepared("select_points_region1");
      pqxx::result res4 = work4.exec_prepared("select_points_region2");
      pqxx::result res5 = work5.exec_prepared("select_points_region3");

      QPainterPath path;
      QPainter painter;
      painter.begin(this);
      painter.setRenderHint(QPainter::Antialiasing, true);
      //      painter.setPen(QPen(Qt::black, 4));
      painter.setBackground(QBrush(Qt::black));
      // painter.setViewport(0,0,1900,1000);
      painter.drawPath(path);

      vector<int> x1 = {0};
      vector<int> y1 = {0};
      vector<int> x2 = {0};
      vector<int> y2 = {0};
      vector<int> x3 = {0};
      vector<int> y3 = {0};
      vector<int> zona = {0};
      vector<int> ob_x1 = {0};
      vector<int> ob_y1 = {0};
      vector<int> ob_x2 = {0};
      vector<int> ob_y2 = {0};
      vector<int> ob_x3 = {0};
      vector<int> ob_y3 = {0};
      vector<int> ob_x4 = {0};
      vector<int> ob_y4 = {0};

      for (const auto& regionpoint : res3) {
        zona.push_back(regionpoint[3].as<int>());
        x1.push_back(regionpoint[1].as<int>());
        y1.push_back(regionpoint[2].as<int>());
      }
      for (const auto& regionpoint : res4) {
        zona.push_back(regionpoint[3].as<int>());
        x2.push_back(regionpoint[1].as<int>());
        y2.push_back(regionpoint[2].as<int>());
      }
      for (const auto& regionpoint : res5) {
        zona.push_back(regionpoint[3].as<int>());
        x3.push_back(regionpoint[1].as<int>());
        y3.push_back(regionpoint[2].as<int>());
      }
      painter.setPen(QPen(Qt::blue, 4));
      for (int i = 1; i < x1.size() - 1; ++i) {
        painter.drawLine(x1[i], y1[i], x1[i + 1], y1[i + 1]);
      }
      painter.drawLine(x1[1], y1[1], x1[x1.size() - 1], y1[x1.size() - 1]);
      painter.setPen(QPen(Qt::red, 4));
      for (int i = 1; i < x2.size() - 1; ++i) {
        painter.drawLine(x2[i], y2[i], x2[i + 1], y2[i + 1]);
      }
      painter.drawLine(x2[1], y2[1], x2[x2.size() - 1], y2[x2.size() - 1]);
      painter.setPen(QPen(Qt::white, 4));
      for (int i = 1; i < x3.size() - 1; ++i) {
        painter.drawLine(x3[i], y3[i], x3[i + 1], y3[i + 1]);
      }
      painter.drawLine(x3[1], y3[1], x3[x3.size() - 1], y3[x3.size() - 1]);

      //      vector<int> x = {};
      //      vector<int> y = {};
      //      vector<int> time = {};
      //      vector<vector<int>> res;
      //      vector<vector<int>> request_track(1, 100, &res, &x, &y);

      pqxx::connection request6(connection_string);
      request6.prepare("select_points_object1",
                       "select * from object_point WHERE object_point_time <= "
                       "$1 AND object_point_parrent =1;");
      pqxx::nontransaction work6(request6);
      pqxx::result res6 =
          work6.exec_prepared("select_points_object1", TimeTrajectory);
      for (const auto& objectpoint : res6) {
        ob_x1.push_back(objectpoint[1].as<int>());
        ob_y1.push_back(objectpoint[2].as<int>());
      }
      painter.setPen(QPen(Qt::green, 4));
      for (unsigned long i = 0; i != ob_x1.size(); ++i) {
        painter.drawPoint(ob_x1[i], ob_y1[i]);
      }
      pqxx::connection request7(connection_string);
      request7.prepare("select_points_object2",
                       "select * from object_point WHERE object_point_time <= "
                       "$1 AND object_point_parrent =2;");
      pqxx::nontransaction work7(request7);
      pqxx::result res7 =
          work7.exec_prepared("select_points_object2", TimeTrajectory);
      for (const auto& objectpoint : res7) {
        ob_x2.push_back(objectpoint[1].as<int>());
        ob_y2.push_back(objectpoint[2].as<int>());
      }
      painter.setPen(QPen(Qt::yellow, 6));
      for (unsigned long i = 0; i != ob_x1.size(); ++i) {
        painter.drawPoint(ob_x2[i], ob_y2[i]);
      }
      pqxx::connection request8(connection_string);
      request8.prepare("select_points_object3",
                       "select * from object_point WHERE object_point_time <= "
                       "$1 AND object_point_parrent =3;");
      pqxx::nontransaction work8(request8);
      pqxx::result res8 =
          work8.exec_prepared("select_points_object3", TimeTrajectory);
      for (const auto& objectpoint : res8) {
        ob_x3.push_back(objectpoint[1].as<int>());
        ob_y3.push_back(objectpoint[2].as<int>());
      }
      painter.setPen(QPen(Qt::magenta, 6));
      for (unsigned long i = 0; i != ob_x1.size(); ++i) {
        painter.drawPoint(ob_x3[i], ob_y3[i]);
      }
      painter.end();
    }  // end try
    catch (const pqxx::sql_error& e) {
      std::cout << "SQL error: " << e.what();
      std::cout << "Query was: '" << e.query().c_str() << "'";
    }
  }  // end renderNow
};   // end class PaintPOLIGON
