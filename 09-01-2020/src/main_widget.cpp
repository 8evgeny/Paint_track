#include "_main.h"

using namespace std;

MainWind::MainWind(QWidget* parent) : QWidget(parent) {
  //  QPalette pall1;
  //  pall1.setColor(this->backgroundRole(), Qt::lightGray);
  //  this->setPalette(pall1);
  //  this->setAutoFillBackground(false);

  //  QPainterPath path;

  m_timerId = QWidget::startTimer(100);
  int maxTime = request_max();
  int minTime = request_min();
  TimeTrajectory = request_min();
  //  QWidget::setMinimumWidth(1600);
  //  QGraphicsScene scene(QRect(0, 0, 1920, 1080));
  //  QGraphicsView* pview = new QGraphicsView(&scene);

  //  scene.addPixmap(QPixmap(
  //      "/home/eparubets/sadko/src/desktop/poligon_example/src/image1.jpg"));
  auto main_layout = new QVBoxLayout;

  //  pview->setLayout(main_layout);

  //  pix.load("/home/eparubets/sadko/src/desktop/poligon_example/src/image1.jpg");
  //  lbl.setPixmap(pix);

  //  lbl.show();

  //   pixmap.scaled(ui->label->size().width(),ui->label->size().height(),Qt::IgnoreAspectRatio,Qt::FastTransformation);
  //   ui->label->setPixmap(pixmap);

  QSlider* slider = new QSlider(Qt::Horizontal);
  slider->setRange(minTime, maxTime);
  main_layout->addStretch(5);
  auto speedtime = new QSpinBox;
  auto spin_box = new QSpinBox;
  spin_box->setValue(TimeTrajectory);
  speedtime->setRange(1, 10);
  spin_box->setRange(minTime, maxTime);
  speedtime->setSingleStep(1);
  spin_box->setSingleStep(1);
  speedtime->setValue(5);

  QRadioButton* scen1 = new QRadioButton("Scenary 1");
  QRadioButton* scen2 = new QRadioButton("Scenary 2");
  QRadioButton* scen3 = new QRadioButton("Scenary 3");
  scen1->setChecked(true);
  scenary = 1;
  QObject::connect(scen1, &QPushButton::clicked, [this]() { startScene(1); });
  QObject::connect(scen2, &QPushButton::clicked, [this]() { startScene(2); });
  QObject::connect(scen3, &QPushButton::clicked, [this]() { startScene(3); });

  auto start = new QPushButton("Start");
  auto stop = new QPushButton("Stop");
  auto reset = new QPushButton("Reset");
  auto buttonH = new QHBoxLayout;
  auto radiobutton = new QVBoxLayout;
  auto slider_ = new QHBoxLayout;

  slider_->addWidget(slider);
  radiobutton->addWidget(scen1);
  radiobutton->addWidget(scen2);
  radiobutton->addWidget(scen3);

  buttonH->addWidget(spin_box);
  buttonH->addWidget(speedtime);
  buttonH->addStretch(5);
  buttonH->addWidget(start);
  buttonH->addWidget(stop);
  buttonH->addWidget(reset);

  main_layout->addItem(radiobutton);
  main_layout->addItem(buttonH);
  main_layout->addItem(slider_);
  setLayout(main_layout);

  QObject::connect(start, &QPushButton::clicked, [this]() { Start(); });
  QObject::connect(reset, &QPushButton::clicked, [this]() { Reset(); });
  QObject::connect(stop, &QPushButton::clicked, [this]() { Stop(); });
  QObject::connect(spin_box, SIGNAL(valueChanged(int)), slider,
                   SLOT(setValue(int)));
  QObject::connect(slider, SIGNAL(valueChanged(int)), spin_box,
                   SLOT(setValue(int)));
  QWidget::connect(spin_box, QOverload<int>::of(&QSpinBox::valueChanged),
                   [this](int value) {
                     setTime(value);
                     this->repaint();
                   });  // Start Scene
  QWidget::connect(spin_box, QOverload<int>::of(&QSpinBox::valueChanged),
                   [this]() { this->repaint(); });
  QWidget::connect(speedtime, QOverload<int>::of(&QSpinBox::valueChanged),
                   [this](int value) { SetSpeedTime(value); });

}  // end constructor

void MainWind::Stop() { stop = true; }

void MainWind::Start() { stop = false; }

void MainWind::Reset() {
  TimeTrajectory = request_min();
  stop = false;
};

void MainWind::SetSpeedTime(int speed) {
  //  Speed = speed;
  m_timerId = QWidget::startTimer(1000 / speed);
}

void MainWind::paintEvent(QPaintEvent*) { renderNow(); }

int MainWind::request_max() {
  max_t.clear();
  auto connection_string = "host=127.0.0.1 dbname=eparubets user=eparubets";
  pqxx::connection request(connection_string);
  request.prepare("request_prepeared",
                  "select MAX(object_point_time) AS object_point_time from    "
                  "object_point");
  pqxx::nontransaction work(request);
  pqxx::result res = work.exec_prepared("request_prepeared");
  for (const auto& i : res) {
    max_t.push_back(i[0].as<int>());
  }
  //  cout << max_t[0] << endl;
  return max_t[0];
}

int MainWind::request_min() {
  min_t.clear();
  auto connection_string = "host=127.0.0.1 dbname=eparubets user=eparubets";
  pqxx::connection request(connection_string);
  request.prepare(
      "request_prepeared",
      "select MIN(object_point_time) AS object_point_time from object_point");
  pqxx::nontransaction work(request);
  pqxx::result res = work.exec_prepared("request_prepeared");
  for (const auto& i : res) {
    min_t.push_back(i[0].as<int>());
  }
  return min_t[0];
}

void MainWind::timerEvent(QTimerEvent* event) {
  if (event->timerId() == m_timerId) {
    if (!stop) {
      TimeTrajectory = TimeTrajectory + 4;
      emit repaint();
      emit spin_boxADD(TimeTrajectory);
    }
  }
}

void MainWind::startScene(int number_scen) { scenary = number_scen; }

void MainWind::setTime(int t) {
  switch (scenary) {
    case 1:
      TimeTrajectory = t;
      break;
    case 2:
      TimeTrajectory = t;
      stop = true;
      //      QTimer::singleShot(1000, this, SLOT(requestValues_inTime()));
      break;
    case 3:
      TimeTrajectory = t;
      stop = true;
      break;
  }
}
