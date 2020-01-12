#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <QApplication>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMainWindow>
#include <QPainter>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTableView>
#include <QTimer>
#include <QTimerEvent>
#include <QWidget>
#include <QtWidgets>
#include <iostream>
#include <pqxx/pqxx>
#include <vector>
using namespace std;

class MainWind : public QWidget {
  Q_OBJECT
 public:
  int TimeTrajectory = 0;
  int Time_imaje = 0;
  QImage image;
  bool ImageSelected = false;
  int maxTime;
  int scenary;
  QPainter painter;
  //  QVBoxLayout *main_layout1;
  //  int qqq = 0;
  void startScene(int);

  void VectorFill(vector<int> &, unsigned long poz, int color);

  int minTime;
  int m_timerId;
  bool stop = false;
  int Speed;
  int spin_boxValue;
  //  QLabel lbl;
  //  QPixmap pix;
  vector<int> Values_inTime;
  vector<string> Patch;
  vector<int> ValuesMinMax;
  vector<int> max_t = {};
  vector<int> min_t = {};
  vector<int> x_region;
  vector<int> y_region;
  vector<vector<int>> result1;
  vector<int> x_1 = {0};
  vector<int> y_1 = {0};
  vector<vector<int>> result2;
  vector<int> x_2 = {0};
  vector<int> y_2 = {0};
  vector<vector<int>> result3;
  vector<int> x_3 = {0};
  vector<int> y_3 = {0};
  vector<int> regions = {};

  // struct track{
  //    track(int);
  //   int num;
  //   vector<int> ob_x;
  //   vector<int> ob_y;
  //   vector<int> color;
  // };
  explicit MainWind(QWidget *parent = nullptr);
  void draw_track(int, vector<int> &x, vector<int> &y, vector<int> &color,
                  vector<int> &, vector<int> &, vector<int> &, vector<int> &,
                  vector<int> &, vector<int> &);
  void draw_region(int number, vector<int> &x, vector<int> &y, int color);
  void setTime(int t);
  void Stop();
  void Reset();
  struct pt {
    int x;
    int y;
    pt operator-(const pt &other) {
      pt temp;

      temp.x = this->x - other.x;
      temp.y = this->y - other.y;
      //      temp.x = other.x - this->x;
      //      temp.y = other.y - this->y;

      return temp;
    }
  };
  pt point;
  pt start_point;
  void Start();
  void SetSpeedTime(int t);
  //  void Init();
  pt Point_start(int);  // begin point in track
  bool intersect(pt &, pt &, pt &, pt &);
  int IsPointInside(int);
  QImage selectImagePath(int);
  int request_max();
  int request_min();
  void renderNow();
  //  void draw_point(int, int);
  void paintEvent(QPaintEvent *) override;
  void timerEvent(QTimerEvent *event) override;
  vector<int> &valuesMinMax(int);
  //  void sendSignal();
 signals:
  void spin_boxADD(int);

 public slots:
  int requestValues_inTime(int);

 private:
};

#endif  // MAIN_WINDOW_H
